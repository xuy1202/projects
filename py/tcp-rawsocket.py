#coding: utf-8

import os, sys
import time
import socket
import struct
import traceback


class IpParser(object):
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


class TcpParser(IpParser):
    ip_proto = socket.IPPROTO_TCP

    def checksum(self, msg):
        s = 0
        for i in range(0, len(msg), 2):
            w = ord(msg[i]) + (ord(msg[i+1]) << 8 )
            s = s + w
        s = (s>>16) + (s & 0xffff);
        s = s + (s >> 16);
        s = ~s & 0xffff
        return s
 
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
        psh = struct.pack('!4s4sBBH' , sip, dip, placeholder , self.ip_proto, tcp_length);
        psh = psh + tcp_header + self.user_data;
        tcp_check = self.checksum(psh)
        # tcp_header with checksum
        tcp_header = struct.pack('!HHLLBBH'  , self.sport, self.dport,
            self.tcp_seq, self.tcp_ack_seq, self.tcp_offset_res,
            tcp_flags, self.tcp_window
        ) + struct.pack('H' , tcp_check) + struct.pack('!H' , tcp_urg_ptr)
        return tcp_header
 
    def packet(self):
        packet = self.ip_header() + self.tcp_header() + self.user_data
        return packet



if __name__ == "__main__":
    dip = socket.gethostbyname("baidu.com")
    dport = 80
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
        # tell kernel not to put in headers, since we are providing it, when using IPPROTO_RAW this is not necessary
        #sock.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

        T = TcpParser()
        T.sip = '0.0.0.0'
        T.sport = 0
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

 

