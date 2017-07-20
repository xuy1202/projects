#coding: utf-8

import os, sys
import socket
import traceback

from net_proto import packet


def main():
    while 1:
        sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_UDP)
        #sniffer.bind((host, 0))
        # include the IP headers in the captured packets
        sniffer.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
        data, addr = sniffer.recvfrom(65565)
        #print addr, repr(data)
        P = packet.Packet()
        P.parse_from_buffer(data)
        print P


if __name__ == '__main__':
    try:
        main()
    except Exception, e:
        print traceback.format_exc()


