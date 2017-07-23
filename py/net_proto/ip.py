#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import icmp
from . import tcp
from . import udp
from . import helper
from .helper import LfixIPW, RfixIPW
from .helper import LfixPORTW, RfixPORTW


class IP(ctypes.Structure):
    NAME           = "IP"
    PROTO_NUMBER   = 0x0800
    _SUB_PROTO_MAP = {
        icmp.ICMP.PROTO_NUMBER: icmp.ICMP,
        tcp.TCP.PROTO_NUMBER  : tcp.TCP,
        udp.UDP.PROTO_NUMBER  : udp.UDP,
    }

    _pack_   = 1
    _fields_ = [
        ('_ver_hl'      , ctypes.c_ubyte ),
        ('_tos'         , ctypes.c_ubyte ),
        ('_total_length', ctypes.c_uint16),
        ('_id'          , ctypes.c_uint16),
        ('_flag_frag'   , ctypes.c_uint16),
        ('_ttl'         , ctypes.c_ubyte ),
        ('_proto'       , ctypes.c_ubyte ),
        ('_checksum'    , ctypes.c_uint16),
        ('_sip'         , ctypes.c_uint32),
        ('_dip'         , ctypes.c_uint32),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(IP):]

    def header_length(self):
        return (self._ver_hl & 0x0f) * 4

    def sip(self): return helper.uint2ip(self._sip)
    def dip(self): return helper.uint2ip(self._dip)

    def proto(self):
        return self._proto
        #return socket.htons(self._proto)

    def sub_type(self):
        return self.proto()

    def info(self):
        return 'ttl=%s tos=%s'%(self._ttl, self._tos)

    def __str__(self):
        return '<%s> %s -> %s'%(self.NAME, RfixIPW(self.sip()), RfixIPW(self.dip()))

    def __repr__(self):
        return '<%s>'%self.NAME

