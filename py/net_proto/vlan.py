#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import arp
from . import ip
from . import helper


# 802.1Q
class VLAN(ctypes.Structure):
    NAME           = "VLAN"
    PROTO_NUMBER   = 0x8100
    _SUB_PROTO_MAP = {
        arp.ARP.PROTO_NUMBER: arp.ARP,
        ip.IP.PROTO_NUMBER  : ip.IP,
    }

    _pack_   = 1
    _fields_ = [
        ('_pri_cfi_vid', ctypes.c_uint16), # pri(3)cfi(1)vid(12)
        ('_type'       , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(VLAN):]

    def pri(self):
        return socket.htons(self._pri_cfi_vid) >> 13

    def vid(self):
        return socket.htons(self._pri_cfi_vid) & 0x0fff

    def sub_type(self):
        return socket.htons(self._type)

    def __str__(self):
        return '<%s:%s>'%(self.NAME, self.vid())

    def __repr__(self):
        return '<%s>'%self.NAME


