#coding: utf-8

import os, sys
import time
import ctypes
import socket
import struct 
import select
import threading
import traceback
from collections import defaultdict
import logging

from twisted.internet import reactor
from twisted.internet import defer
from twisted.names import client
from twisted.names import dns
from twisted.names import error
from twisted.names import server
from twisted.names import common 


_format = '%(asctime)s %(levelname)s [%(filename)s:%(lineno)d] %(message)s'
logging.basicConfig(
    level=logging.DEBUG,
    format=_format,
    datefmt='%Y-%m-%d %H:%M:%S',
    filename='/tmp/http_proxy.log',
    filemode='w',
)
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter(_format)
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)


_DEFAULT_PORT = 80
_DEFAULT_TMOT = 10
_DEFAULT_HOST = 'cybatk.com'
_DEFAULT_PAGE = '''\
HTTP/1.1 302 FOUND\r
Location: http://%s
Content-Type: text/html; charset=utf-8\r
Access-Control-Allow-Origin: *\r
Content-Encoding: gzip\r
Content-Length: 0\r
Accept-Ranges: bytes\r
\r
'''%_DEFAULT_HOST


class SSL_Handshake(ctypes.Structure):
    NAME           = "Handshake"
    PROTO_NUMBER   = 0x16

    _pack_   = 1
    _fields_ = [
        ('_type'   , ctypes.c_ubyte ),
        ('_version', ctypes.c_uint16),
        ('_length' , ctypes.c_uint16),
        ('_st_len' , ctypes.c_uint32),
        ('_st_ver' , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL_Handshake):]
        try:
            self.deep_parse()
        except Exception, e:
            #print traceback.format_exc()
            pass

    def deep_parse(self):
        self.random = self._payload[:32]
        self._payload = self._payload[32:]

        self.sid_len = ord(self._payload[:1])
        self._payload = self._payload[1:]

        self.sid_str = self._payload[:self.sid_len]
        self._payload = self._payload[self.sid_len:]

        self.cipher_suit_len = struct.unpack('!H', self._payload[:2])[0]
        self._payload = self._payload[2:]

        cipher_suit_dat = self._payload[:self.cipher_suit_len]
        self._payload = self._payload[self.cipher_suit_len:]

        self.compress_dat = self._payload[:2]
        self._payload = self._payload[2:]

        self.extension_len = struct.unpack('!H', self._payload[:2])[0]
        self._payload = self._payload[2:]

        self.extension_dat = self._payload[:self.extension_len]
        self._payload = self._payload[self.extension_len:]

    # 0x0301 769 == TLS1.0
    # 0x0303 771 == TLS1.0
    def version(self):
        return socket.htons(self._version)

    def length(self):
        return socket.htons(self._length)

    def sub_type(self):
        return self._type

    def handshake_version(self):
        return socket.htons(self._st_ver)

    def handshake_lens(self):
        return socket.htonl(self._st_len) & 0x00ffffff

    def handshake_type(self):
        return socket.htonl(self._st_len) >> 24

    def handshake_type_tag(self):
        t = self.handshake_type()
        if t == 1:
            return 'client-hello'
        elif t == 2:
            return 'server-hello'
        elif t == 16:
            return 'client-key-exchange'
        else:
            return str(t)

    def application_layer_proto_negotiation(self):
        def find_alpn(dat):
            rets = []
            while dat:
                _type = struct.unpack('!H', dat[0:2])[0]
                _lens = struct.unpack('!H', dat[2:4])[0]
                dat = dat[4:]
                _dat, dat = dat[:_lens], dat[_lens:]
                if _type != 0x0010:
                    continue
                alpn_ext_len = struct.unpack('!H', _dat[0:2])[0]
                _dat = _dat[2:2+alpn_ext_len]
                while _dat:
                    alpn_str_len = ord(_dat[0])
                    alpn_proto_s = _dat[1:1+alpn_str_len]
                    rets.append(alpn_proto_s)
                    _dat = _dat[1+alpn_str_len:]
                break
            return ','.join(rets)
        try: return find_alpn(self.extension_dat)
        except: return ''

    def server_name(self):
        def find_server_name_extension(dat):
            while dat:
                _type = struct.unpack('!H', dat[0:2])[0]
                _lens = struct.unpack('!H', dat[2:4])[0]
                dat = dat[4:]
                _dat, dat = dat[:_lens], dat[_lens:]
                if _type != 0x0000:
                    continue
                #name_list_len = struct.unpack('!H', _dat[:2])
                #name_type     = struct.unpack('!H', _dat[2])
                #name_lens     = struct.unpack('!H', _dat[3:5])
                return _dat[5:]
            return ''
        try: return find_server_name_extension(self.extension_dat)
        except: return ''

    def __str__(self):
        rets = '%s %s'%(self.NAME, self.handshake_type_tag())
        t = self.handshake_type()
        if t == 1 or t == 2:
            sn = self.server_name()
            ap = self.application_layer_proto_negotiation()
            if sn: rets += " To:%s"%sn
            if ap: rets += " ALPN:%s"%ap
        return rets


class Dispatcher(threading.Thread):
    def __init__(self, client_sock):
        super(Dispatcher, self).__init__()
        self.client_sock = client_sock
        self._timeout = _DEFAULT_TMOT
        self.buffer_to_server = ''
        self.buffer_to_client = ''
        self.info ={
            'src': '%s:%s'%self.client_sock.getpeername(),
        }

    def set_timeout(self, t):
        self.client_sock.settimeout(t)
        self.server_sock.settimeout(t)
        self._timeout = t

    def __del__(self):
        try: self.client_sock.close()
        except: pass
        try: self.server_sock.close()
        except: pass

    def _parse_http_proxy(self, data):
        ''' http proxy
            1, parse header, like 'CONNECT www.google.co.jp:443 HTTP/1.1\r\nHost: ...'
            2, ack like 'http/1.1 200 Connection established'
            3, omit this data
        '''
        # what will happen if host in here is different with host in 'Host: xxx'
        headers = data.split('\r\n', 1)
        _, host, proto = headers[0].split()
        port = 80
        if host.find(':') > 1:
            host, port = host.split(':', 1)
            port = int(port)
        ack = '%s 200 Connection established\r\n\r\n'%proto
        self.client_sock.sendall(ack)
        return (host, port, '')

    def _parse_http_raw(self, data):
        # i think host must in top 10 lines
        headers = data.split('\r\n', 10)
        host = ''
        port = 80
        for h in headers:
            h = h.lower()
            if not h.startswith('host:'):
                continue
            host = h.split(':', 1)[1].strip().lower()
            if host.find(':') > 1:
                host, port = host.split(':', 1)
                port = int(port)
            break
            # what will happen if there's 2 'Host: xxx'
        return (host, port, data)

    def _parse_https_raw(self, data):
        # i think host must in top 10 lines
        #logging.info('http raw:' + repr(data))
        SSLPack = SSL_Handshake(data)
        host = SSLPack.server_name()
        port = 443
        return (host, port, data)

    def parse_host_port(self, data):
        if data.startswith('CONNECT'):
            return self._parse_http_proxy(data)
        elif ord(data[0]) == SSL_Handshake.PROTO_NUMBER:
            return self._parse_https_raw(data)
        else: # can add more proxy support here
            return self._parse_http_raw(data)

    def prepare(self):
        try:
            self.client_sock.setblocking(1)
            self.client_sock.settimeout(self._timeout)
            head_data = self.client_sock.recv(1024)
            domain = host = port = ''
            if head_data:
                domain, port, head_data = self.parse_host_port(head_data)
            self.info['dst.domain'] = domain
            self.info['dst.port'] = port
            self_host = [
                socket.gethostname(),
                socket.gethostbyname(socket.gethostname())
            ]
            if domain in self_host:
                self.client_sock.sendall(_DEFAULT_PAGE)
                raise RuntimeError('FackPageReturned')
            if domain:
                # for test
                #host = '54.76.27.129' # www.hypercomments.com
                host = socket.gethostbyname(domain)
                self.info['dst.host'] = host
            if domain and host and port:
                self.buffer_to_server = head_data # do NOT forget this part data
                self.server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.server_sock.connect((host, port))
                self.info['dst'] = '%s:%s'%self.server_sock.getpeername()
                self.server_sock.setblocking(0)
                self.server_sock.settimeout(self._timeout)
                self.client_sock.setblocking(0)
                logging.info("Dispatch New Peer-Connection Succ: %s"%str(self.info))
                return True
            else:
                raise RuntimeError('ParseHeadError domain:%s ip:%s port:%s'%(domain, host, port))
        except Exception, e:
            #print traceback.format_exc()
            self.info['err'] = str(e)
            logging.error("Dispatch New Peer-Connection Fail: %s"%str(self.info))
            return False

    def loop_trans(self):
        self.info['stat'] = {
            "send_to_client"  : 0,
            "send_to_server"  : 0,
            "recv_from_client": 0,
            "recv_from_server": 0,
        }
        try:
            socks = [self.client_sock, self.server_sock]
            while 1:
                # no data == no needs to check writable
                if self.buffer_to_server or self.buffer_to_client:
                    rlist, wlist, xlist = select.select(socks, socks, socks, self._timeout)
                else:
                    rlist, wlist, xlist = select.select(socks, [], socks)
                if not any([rlist, wlist, xlist]): raise RuntimeError('NoEvents')
                if xlist: raise RuntimeError(str(exceptionals)) 
                #logging.info(str([i.getpeername() for i in rlist])
                #    + str([i.getpeername() for i in wlist])
                #    + str([i.getpeername() for i in xlist])
                #)
                for r in rlist:
                    data = r.recv(65535)
                    if not data:
                        raise RuntimeError('ReadEmpty')
                    if r is self.client_sock:
                        self.buffer_to_server += data
                        self.info['stat']['recv_from_client'] += len(data)
                    if r is self.server_sock:
                        self.buffer_to_client += data
                        self.info['stat']['recv_from_server'] += len(data)

                for w in wlist:
                    if w is self.client_sock and self.buffer_to_client:
                        self.client_sock.sendall(self.buffer_to_client)
                        self.info['stat']['send_to_client'] += len(self.buffer_to_client)
                        self.buffer_to_client = ''
                    if w is self.server_sock and self.buffer_to_server:
                        self.server_sock.sendall(self.buffer_to_server)
                        self.info['stat']['send_to_server'] += len(self.buffer_to_server)
                        self.buffer_to_server = ''
        except (Exception, KeyboardInterrupt), e:
            self.info['reason'] = str(e)
            logging.info("Dispatch Peer-Connection Ends: %s"%str(self.info))

    def run(self):
        return self.prepare() and self.loop_trans()


class ListenServ(threading.Thread):
    def __init__(self, listen_port):
        super(ListenServ, self).__init__()
        self._port = listen_port

    def run(self):
        while 1:
            try:
                listen_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                laddr = ('0.0.0.0', self._port)
                logging.info("Listen On: %s"%(laddr,))
                listen_sock.bind(('0.0.0.0', self._port))
                listen_sock.listen(10)
                th_list = []
                while 1:
                    client_sock, addr = listen_sock.accept()
                    th = Dispatcher(client_sock)
                    th.setDaemon(True)
                    th.start()
                    th_list.append(th)
                    th_list = [i for i in th_list if i.isAlive()]
                    logging.info("Thread Pool Size: %d"%(len(th_list)))
            except Exception, e:
                try: listen_sock.close()
                except: pass
                logging.error(str(traceback.format_exc()))
                time.sleep(1)
            except KeyboardInterrupt, e:
                print 'User Interrupt'
                break


class DynamicResolver(object):
    def __init__(self):
        self.RealDns = client.Resolver(resolv='/etc/resolv.conf')

    def _getA(self, name):
        ip = socket.gethostbyname("soulmate.im")
        logging.info("DnsHijack %s -> %s"%(name, ip))
        answer = dns.RRHeader(
            name = name,
            type = dns.A,
            cls = dns.IN,
            ttl = 600,
            payload = dns.Record_A(address = b'%s'%ip, ttl=600)
        )
        answers = [answer]
        authority = []
        additional = []
        return answers, authority, additional

    def _doDynamicResponse(self, query):
        name = query.name.name
        return self._getA(name)

        if name.endswith('.163.com'):
            return self._getA(name)
        else:
            return self.RealDns.query(query)

    def query(self, query, timeout=None):
        return defer.succeed(self._doDynamicResponse(query))

    def lookupAllRecords(self, name, timeout=None):
        return defer.succeed(self._getAny(name))


def main():
    # start http[s] service
    for port in [80, 443]:
        th = ListenServ(port)
        th.setDaemon(True)
        th.start()
    # start DNS server
    #clients=[DynamicResolver(), client.Resolver(resolv='/etc/resolv.conf')]
    factory = server.DNSServerFactory(
        clients=[DynamicResolver()]
    )
    reactor.listenTCP(53, factory)
    protocol = dns.DNSDatagramProtocol(controller=factory)
    reactor.listenUDP(53, protocol)
    reactor.run()


# TODO: change to thread-pool
if __name__ == '__main__':
    try:
        main()
    except Exception, e:
        logging.error(str(traceback.format_exc()))



