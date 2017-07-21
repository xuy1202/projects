#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import helper


class ARP(ctypes.Structure):
    NAME           = "ARP"
    PROTO_NUMBER   = 0x0806
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
        ('_ar_hrd', ctypes.c_uint16), # hardware type, like 0x0001 for "ETHERNET"
        ('_ar_pro', ctypes.c_uint16), # proto type, like 0x0800 for "IPv4"
        ('_ar_hln', ctypes.c_ubyte ), # hardware address length
        ('_ar_pln', ctypes.c_ubyte ), # proto address length
        ('_ar_op' , ctypes.c_uint16), # opcode: ARP request==1 ARP reply==2 RARP request==3 RARP reply==4
        ('_smac1' , ctypes.c_uint8 ),
        ('_smac2' , ctypes.c_uint8 ),
        ('_smac3' , ctypes.c_uint8 ),
        ('_smac4' , ctypes.c_uint8 ),
        ('_smac5' , ctypes.c_uint8 ),
        ('_smac6' , ctypes.c_uint8),
        ('_sip'   , ctypes.c_uint32),
        ('_dmac1' , ctypes.c_uint8 ),
        ('_dmac2' , ctypes.c_uint8 ),
        ('_dmac3' , ctypes.c_uint8 ),
        ('_dmac4' , ctypes.c_uint8 ),
        ('_dmac5' , ctypes.c_uint8 ),
        ('_dmac6' , ctypes.c_uint8 ),
        ('_dip'   , ctypes.c_uint32),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(ARP):]

    def sub_type(self):
        return socket.htons(self._ar_op)

    def __str__(self):
        from .ethernet import _format_mac 
        sip  = helper.uint2ip(self._sip) 
        dip  = helper.uint2ip(self._dip)
        smac = _format_mac(self._smac1, self._smac2, self._smac3, self._smac4, self._smac5, self._smac6)
        dmac = _format_mac(self._dmac1, self._dmac2, self._dmac3, self._dmac4, self._dmac5, self._dmac6)
        t = self.sub_type()
        if t == 0x01:
            return "<ARP-request> I'm %s(%s), who is %s(%s)"%(sip, smac, dip, dmac)
        if t == 0x02:
            return "<ARP-reply> I'm the %s(%s), tell %s(%s)"%(sip, smac, dip, dmac)
        if t == 0x03:
            return "<RARP-request> I'm %s(%s), who is %s(%s)"%(sip, smac, dip, dmac)
        if t == 0x04:
            return "<RARP-reply> I'm the %s(%s), tell %s(%s)"%(sip, smac, dip, dmac)
        return '<ARP-unknown>'

    def __repr__(self):
        return '<%s>'%self.NAME


