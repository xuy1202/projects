#coding: utf-8

import os, sys
import socket
import traceback
import ctypes

from net_proto import packet
from net_proto import udp
from net_proto import arp
from net_proto import ethernet
from net_proto.helper import Lfix13

import helper
import udp_data

import pcap


PROTO = 0
for n in dir(socket):
    if n.find('SOCK') < 0:
        continue
    #print n
    PROTO |= getattr(socket, n)
PROTO = socket.IPPROTO_FRAGMENT
PROTO = socket.IPPROTO_TCP


def pcap_snif():
    sniffer = pcap.pcapObject()
    sniffer.open_live('eth0', 65535, True, 100) # device, snaplength, promiscous_mode, timeout
    capture_filter = 'arp'
    capture_filter = 'host 106.187.47.79'
    sniffer.setfilter(capture_filter, 0, 0)
    #filename = '/tmp/%s.pcap'%capture_filter.replace(' ', '_')
    #sniffer.dump_open(filename)
    def proc(pktlen, data, timestamp):
        P = packet.Packet()
        P.parse_from_buffer(data, ethernet.ETHERNET)
        ostr = str(P)
        print Lfix13(timestamp), ostr
    while 1:
        sniffer.dispatch(1, proc)
        #sniffer.dispatch(1, None) # dump to file
    sys.exit(0)


def rawsocket():
    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, PROTO)
    #sniffer.bind((host, 0))
    # include the IP headers in the captured packets
    sniffer.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
    while 1:
        data, addr = sniffer.recvfrom(65565)
        #print addr, repr(data)
        P = packet.Packet()
        P.parse_from_buffer(data, ip.IP)
        print P


def tcp_listen():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    host = '127.0.0.1'
    sock.bind((host, 10000))
    sock.listen(10)
    while 1:
        conn, addr = sock.accept()
        conn.settimeout(10)
        while 1:
            try:
                data = conn.recv(1024)
                print 'syn:%s'%repr(data)
                conn.send('ack:%s'%data)
            except socket.timeout, e:
                conn.close()
                break
        print ">>>> next?"
    sock.close()


def udp_scan():
    sniffer = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    #sniffer.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
    #d = helper.reform_from_tcpdump(udp_data.dns_baidu_com_a)
    #p = packet.Packet()
    #p.parse_from_buffer(d)
    #print p
    while 1:
        data, addr = sniffer.recvfrom(65565)
        #print addr, repr(data)
        P = packet.Packet()
        P.parse_from_buffer(data)
        print P


if __name__ == '__main__':
    try:
        pcap_snif()
        rawsocket()
        tcp_listen()
        udp_scan()
    except Exception, e:
        print traceback.format_exc()


