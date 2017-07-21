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



class Packet(object):
    def __init__(self):
        self.layers = []

    def parse_from_buffer(self, buffer, Proto=ip.IP):
        self.length = len(buffer)
        while Proto:
            pobj = Proto(buffer)
            setattr(self, pobj.NAME, pobj)
            if self.layers:
                setattr(self.layers[-1], pobj.NAME, pobj)
            self.layers.append(pobj)

            buffer = pobj._payload
            Proto = Proto._SUB_PROTO_MAP.get(pobj.sub_type(), None)

    def _str_IP(self):
        return str(self.IP)

    def _str_IP_ICMP(self):
        _f = 'ICMP %s -> %s %s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            RfixIPW(   self.IP.dip()    ),
            self.length
        )
        return _f%(self.IP.info(), self.ICMP.info())
    _str_ETHERNET_IP_ICMP = _str_IP_ICMP

    def _str_IP_UDP(self):
        _f = 'UDP  %s:%s -> %s:%s\t%s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.UDP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.UDP.dport() ),
            self.length
        )
        return _f%(self.IP.info(), self.UDP.info())
    _str_ETHERNET_IP_UDP = _str_IP_UDP

    def _str_IP_TCP(self):
        _f = 'TCP  %s:%s -> %s:%s\t%s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.TCP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.TCP.dport() ),
            self.length
        )
        return _f%(self.IP.info(), self.TCP.info())
    _str_ETHERNET_IP_TCP = _str_IP_TCP

    def __str__(self):
        strname = '_str_' + '_'.join([i.NAME for i in self.layers])
        strfunc = getattr(self, strname, None)
        if strfunc:
            return strfunc()
        else:
            return '\t'.join([str(i) for i in self.layers])


