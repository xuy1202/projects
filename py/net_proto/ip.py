#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import helper


class IP(ctypes.Structure):
    NAME         = "IP"

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

    def __str__(self):
        return '%s\t->\t%s\t%s'%(helper.uint2ip(self._sip), helper.uint2ip(self._dip), self._proto)

    def __repr__(self):
        return '<%s>'%self.NAME

    def header_length(self):
        return (self._ver_hl & 0x0f) * 4

    def sip(self): return helper.uint2ip(self._sip)
    def dip(self): return helper.uint2ip(self._dip)

    def proto(self):
        return self._proto

    def info(self):
        return 'ttl=%s tos=%s'%(self._ttl, self._tos)



