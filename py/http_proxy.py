#coding: utf-8

import os, sys
import time
import socket
import select
import threading
import traceback
from collections import defaultdict
import logging

from net_proto import ssl


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


_DEFAULT_PORT = 443 
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
        port = _DEFAULT_PORT
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
        port = _DEFAULT_PORT
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
        SSLPack = ssl.SSL_Handshake(data)
        host = SSLPack.server_name()
        port = 443
        return (host, port, data)

    def parse_host_port(self, data):
        if data.startswith('CONNECT'):
            return self._parse_http_proxy(data)
        elif ord(data[0]) == ssl.SSL_Handshake.PROTO_NUMBER:
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
            if domain == '192.168.3.2':
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
                raise RuntimeError('ParseHeadError')
        except Exception, e:
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


def main():
    while 1:
        try:
            listen_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            listen_sock.bind(('0.0.0.0', _DEFAULT_PORT))
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


if __name__ == '__main__':
    main()


