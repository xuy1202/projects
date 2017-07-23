#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import dns
from . import helper


class UDP(ctypes.Structure):
    PROTO_NUMBER   = 17
    NAME           = "UDP"
    _SUB_PROTO_MAP = {
        53: dns.DNS,
    }

    _pack_   = 1
    _fields_ = [
        ('_sport'       , ctypes.c_uint16),
        ('_dport'       , ctypes.c_uint16),
        ('_length'      , ctypes.c_uint16),
        ('_checksum'    , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(UDP):]

    def info(self):
        return 'payload_len=%s'%(len(self._payload))

    def sport(self): return socket.htons(self._sport)
    def dport(self): return socket.htons(self._dport)

    def __str__(self):
        return '<UDP>  %s\t->\t%s'%(socket.htons(self._sport), socket.htons(self._dport))

    def __repr__(self):
        return '<%s>'%self.NAME

    def sub_type(self):
        if self.sport() == 53 or self.dport() == 53:
            return 53
        return -1


