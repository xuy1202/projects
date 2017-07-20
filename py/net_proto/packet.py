#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import ip
from . import icmp
from . import tcp
from . import udp

from .helper import uint2ip
from .helper import LfixIPW, RfixIPW
from .helper import LfixPORTW, RfixPORTW


_PROTO_MAP = {
    icmp.ICMP.PROTO_NUMBER: icmp.ICMP,
    tcp.TCP.PROTO_NUMBER  : tcp.TCP,
    udp.UDP.PROTO_NUMBER  : udp.UDP,
}


class Packet(object):
    def __init__(self):
        self.layers = []

    def parse_from_buffer(self, buffer):
        self.length = len(buffer)

        self.IP   = ip.IP(buffer)
        self.layers.append(self.IP)

        subt = _PROTO_MAP.get(self.IP.proto(), None)
        if subt:
            o = subt(buffer[self.IP.header_length():])
            setattr(self, o.NAME, o)
            self.layers.append(o)

    def _str_IP_ICMP(self):
        _f = 'ICMP %s -> %s %s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            RfixIPW(   self.IP.dip()    ),
            self.length
        )
        return _f%(self.IP.info(), self.ICMP.info())

    def _str_IP_UDP(self):
        _f = 'UDP  %s:%s -> %s:%s\t%s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.UDP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.UDP.dport() ),
            self.length
        )
        return _f%(self.IP.info(), self.UDP.info())

    def _str_IP_TCP(self):
        _f = 'TCP  %s:%s -> %s:%s\t%s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.TCP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.TCP.dport() ),
            self.length
        )
        return _f%(self.IP.info(), self.TCP.info())

    def __str__(self):
        strname = '_str_' + '_'.join([i.NAME for i in self.layers])
        strfunc = getattr(self, strname, None)
        if strfunc:
            return strfunc()
        else:
            return str(self.layers)


