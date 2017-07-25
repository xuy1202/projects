#coding: utf-8

import os, sys
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
        self._payload = buffer
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

    def _str_ICMP_IP(self):
        _f = 'ICMP %s -> %s %s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            RfixIPW(   self.IP.dip()    ),
            self.length
        )
        return _f%(self.IP.info(), self.ICMP.info())
    _str_ICMP_IP_ETHERNET = _str_ICMP_IP

    def _str_UDP_IP(self):
        _f = 'UDP  %s:%s -> %s:%s\t%s\t[%%s] [%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.UDP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.UDP.dport() ),
            self.length
        )
        return _f%(self.IP.info(), self.UDP.info())
    _str_UDP_IP_ETHERNET = _str_UDP_IP

    def _str_TCP_IP(self):
        _f = 'TCP  %s:%s -> %s:%s\t%s\t[%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.TCP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.TCP.dport() ),
            self.length
        )
        return _f%(self.TCP.info())
    _str_TCP_IP_ETHERNET = _str_TCP_IP

    def _str_DNS_UDP(self):
        _f = 'DNS  %s:%s -> %s:%s\t%s\t[%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.UDP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.UDP.dport() ),
            self.length
        )
        return _f%(self.DNS.info())
    _str_DNS_UDP_IP          = _str_DNS_UDP
    _str_DNS_UDP_IP_ETHERNET = _str_DNS_UDP

    def _str_DNS_TCP(self):
        _f = 'DNS  %s:%s -> %s:%s\t%s\t[%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.TCP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.TCP.dport() ),
            self.length
        )
        return _f%(self.DNS.info())
    _str_DNS_TCP_IP          = _str_DNS_TCP
    _str_DNS_TCP_IP_ETHERNET = _str_DNS_TCP

    def _str_HTTP_TCP(self):
        _f = 'HTTP %s:%s -> %s:%s\t%s\t[%%s]'%(
            RfixIPW(   self.IP.sip()    ),
            LfixPORTW( self.TCP.sport() ),
            RfixIPW(   self.IP.dip()    ),
            LfixPORTW( self.TCP.dport() ),
            self.length
        )
        return _f%(self.TCP.info())
    _str_HTTP_TCP_IP          = _str_HTTP_TCP
    _str_HTTP_TCP_IP_ETHERNET = _str_HTTP_TCP

    def __str__(self):
        spayload = self._payload
        for l in self.layers:
            perr = getattr(l, 'parse_error', None)
            if perr:
                sys.stderr.write(">>> Parse Error<%s>[%s]\n"%(l.NAME, repr(spayload)))
            else:
                spayload = l._payload
        strname = '_str_' + '_'.join([i.NAME for i in self.layers[::-1]])
        strfunc = getattr(self, strname, None)
        if strfunc:
            return strfunc()
        else:
            return '\t'.join([str(i) for i in self.layers])



