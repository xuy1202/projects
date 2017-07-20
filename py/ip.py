#coding: utf-8

from socket import inet_ntoa, inet_pton
from socket import htonl, AF_INET
from struct import pack, unpack


def uint2ip(i):
    return inet_ntoa(pack("!I", htonl(i)))


def ip2uint(ip):
    f = unpack('!I', inet_pton(AF_INET, ip))[0]

