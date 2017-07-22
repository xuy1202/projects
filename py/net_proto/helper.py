#coding: utf-8

import socket
import struct
import ctypes
import traceback
from functools import partial

from socket import inet_ntoa, inet_pton
from socket import htonl, AF_INET
from struct import pack, unpack


def uint2ip(i):
    return inet_ntoa(pack("!I", htonl(i)))

def ip2uint(ip):
    f = unpack('!I', inet_pton(AF_INET, ip))[0]

def LfixIPW(ip):
    return ip.ljust(15, ' ')

def RfixIPW(ip):
    return ip.rjust(15, ' ')

def LfixPORTW(port):
    return str(port).ljust(5, ' ')

def RfixPORTW(port):
    return str(port).rjust(5, ' ')

def Lfix(w, s):
    return str(s).ljust(w, ' ')
Lfix4  = partial(Lfix, 4)
Lfix13 = partial(Lfix, 13)

def Rfix(w, s):
    return str(s).rjust(w, ' ')
Rfix4  = partial(Rfix, 4)
Rfix13 = partial(Rfix, 13)
