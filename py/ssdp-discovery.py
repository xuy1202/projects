#coding: utf-8

import socket

# http://upnp.org/specs/arch/UPnP-arch-DeviceArchitecture-v2.0.pdf
#  TCPPORT.UPNP.ORG:replies to a TCP port on the control point
#  CPUUID.UPNP.ORG: uuid of the control point 
#  CPFN.UPNP.ORG: test

HOST = '239.255.255.250'
HOST = '<broadcast>'
HOST = '36.105.197.117'
PORT = 1900
SIZE = 1500

SSDP_SEARCH_FMT = [ 
    "M-SEARCH * HTTP/1.1",
    "HOST:%s:%s"%(HOST, PORT),
    "MAN:ssdp:discover",
    "MX:1",
    "ST:ssdp:all",
    "USER-AGENT:unix/6.2 UPnP/2.0 T/0",
]
SSDP_SEARCH_MSG = "\r\n".join(SSDP_SEARCH_FMT)

print len(SSDP_SEARCH_MSG), repr(SSDP_SEARCH_MSG)

udpCliSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udpCliSock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

recv_count = 0

while True:
    print udpCliSock.sendto(SSDP_SEARCH_MSG, (HOST, PORT))
    while 1:
        data, ADDR = udpCliSock.recvfrom(SIZE)
        recv_count += 1
        print ">>> %s %s:%s -> %d\n%s"%(recv_count, ADDR[0], ADDR[1], len(data), data)

udpCliSock.close()


