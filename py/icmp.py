#coding: utf-8

import ctypes
import socket
import traceback
import struct

import rawsocket
import helper
import udp_data

from net_proto import ip, icmp, tcp, udp
from net_proto import package


try:
    i = 8
    print i, bin(i)
    print socket.htons(i), bin(socket.htons(i))
    #d = 'E\x00\x00T\x00\x00@\x008\x01\r\xd4\xdc\xb59\xd9\n\x10\x147\x00\x00C5$-\x00\x01\xfa[pY\x00\x00\x00\x00a\x14\x0e\x00\x00\x00\x00\x00\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./01234567'

    #P = package.Package()
    #P.parse_from_buffer(d)
    #print P

    #ip_h = ip.IP(d)
    #print ip_h
    #print ip_h.header_length()
    #print ip_h.proto(), type(ip_h.proto())
    #icmp = icmp.ICMP(d[ip_h.header_length():])
    #print icmp

    #U = rawsocket.UdpParser()
    #U.sip = '1.2.3.4'
    #U.sport = 1234

    #U.dip = socket.gethostbyname(socket.gethostname())
    #U.dport = 1900 

    #U.user_data = helper.reform_from_tcpdump(udp_data.dns_cpsc_gov_any)
    #d = U.packet()

    #print  IP(d)
    #print UDP(d)

    #T = rawsocket.TcpParser()
    #T.sip = '1.2.3.4'
    #T.sport = 1234
    #T.dip = socket.gethostbyname(socket.gethostname())
    #T.dport = 1900
    #T.tcp_syn = 1
    #T.user_data = "test"
    #d = T.packet()
    #print  IP(d)
    #print TCP(d)
    #print socket.htons(TCP(d).windows)
except Exception, e:
    print traceback.format_exc()



