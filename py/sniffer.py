#coding: utf-8

import os, sys
import socket
import traceback
import ctypes

from net_proto import packet
from net_proto import udp
from net_proto import arp
from net_proto import dns
from net_proto import ethernet
from net_proto.helper import Lfix13

import helper
from data import udp_data

import pcap


def pcap_snif():
    capture_filter = 'arp'
    capture_filter = 'host 106.187.47.79'
    capture_filter = 'icmp'
    capture_filter = 'tcp and port 53'
    capture_filter = 'tcp and port 80'

    def proc(pktlen, data, timestamp):
        P = packet.Packet()
        P.parse_from_buffer(data, ethernet.ETHERNET)
        ostr = str(P)
        if ostr:
            print Lfix13(timestamp), ostr

    if getattr(pcap, 'pcap', None):
        #sniffer = pcap.pcap('en0', immediate=True)
        sniffer = pcap.pcap(immediate=True)
        if capture_filter:
            sniffer.setfilter(capture_filter)
        sniffer.setnonblock(True)
        for timestamp, data in sniffer:
            proc(0, data, timestamp)
    else:
        sniffer = pcap.pcapObject()
        # device, snaplength, promiscous_mode, timeout
        sniffer.open_live('eth0', 65535, True, 100)
        if capture_filter:
            sniffer.setfilter(capture_filter, 0, 0)
        #filename = '/tmp/%s.pcap'%capture_filter.replace(' ', '_')
        #sniffer.dump_open(filename)
        while 1:
            sniffer.dispatch(1, proc)
            #sniffer.dispatch(1, None) # dump to file
    sys.exit(0)


def rawsocket():
    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_UDP)
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


