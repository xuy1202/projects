#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import vlan
from . import arp
from . import ip
from . import helper


def _format_mac(p1, p2, p3, p4, p5, p6):
    p1 = (hex(p1)[2:]).upper().rjust(2, '0')
    p2 = (hex(p2)[2:]).upper().rjust(2, '0')
    p3 = (hex(p3)[2:]).upper().rjust(2, '0')
    p4 = (hex(p4)[2:]).upper().rjust(2, '0')
    p5 = (hex(p5)[2:]).upper().rjust(2, '0')
    p6 = (hex(p6)[2:]).upper().rjust(2, '0')
    r = '%s:%s:%s:%s:%s:%s'%(p1, p2, p3, p4, p5, p6)
    if r == '00:00:00:00:00:00':
        r = '?'
    return r


# vlan  : 0x8100
# IP(V4): 0x0800
# ARP   : 0x0806
# PPPoE : 0x8864
# IPV6  : 0x86DD
# MPLS  : 0x8847
class ETHERNET(ctypes.Structure):
    NAME           = "ETHERNET"
    _SUB_PROTO_MAP = {
        vlan.VLAN.PROTO_NUMBER: vlan.VLAN,
        arp.ARP.PROTO_NUMBER  : arp.ARP,
        ip.IP.PROTO_NUMBER    : ip.IP,
    }

    _pack_   = 1
    _fields_ = [
        ('_dmac1', ctypes.c_ubyte),
        ('_dmac2', ctypes.c_ubyte),
        ('_dmac3', ctypes.c_ubyte),
        ('_dmac4', ctypes.c_ubyte),
        ('_dmac5', ctypes.c_ubyte),
        ('_dmac6', ctypes.c_ubyte),
        ('_smac1', ctypes.c_ubyte),
        ('_smac2', ctypes.c_ubyte),
        ('_smac3', ctypes.c_ubyte),
        ('_smac4', ctypes.c_ubyte),
        ('_smac5', ctypes.c_ubyte),
        ('_smac6', ctypes.c_ubyte),
        ('_type', ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(ETHERNET):]

    def smac(self):
        return _format_mac(self._smac1, self._smac2, self._smac3, self._smac4, self._smac5, self._smac6)

    def dmac(self):
        return _format_mac(self._dmac1, self._dmac2, self._dmac3, self._dmac4, self._dmac5, self._dmac6)

    def sub_type(self):
        return socket.htons(self._type)

    def __str__(self):
        return '<%s> %s -> %s'%(self.NAME, self.smac(), self.dmac())

    def __repr__(self):
        return '<%s>'%self.NAME

    def info(self):
        return 'ttl=%s tos=%s'%(self._ttl, self._tos)



