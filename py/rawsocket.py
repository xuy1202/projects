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

        self.ip_tos = 0
        self.ip_tot_len = 0  # kernel will fill the correct total length
        self.ip_id = 0       # id of this packet
        self.ip_frag_off = 0
        self.ip_ttl = 64
        self.ip_check = 0    # kernel will fill the correct checksum

    def ip_header(self):
        ip_saddr = socket.inet_aton(self.sip)
        ip_daddr = socket.inet_aton(self.dip)
        ip_ver_ihl = int('01000101', 2) # 0100 for ipv4, 0101 for 5(*4 = 20)
        # the ! in the pack format string means network order
        ip_header = struct.pack('!BBHHHBBH4s4s',
            ip_ver_ihl,
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
        #self.tcp_window = socket.htons(5840) # maximum allowed window size
        self.tcp_window = 5840
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



if __name__ == "__main__":
    pass







