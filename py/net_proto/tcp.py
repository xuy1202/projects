#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import dns
from . import http
from . import helper


class TCP(ctypes.Structure):
    PROTO_NUMBER   = 6
    NAME           = "TCP"
    _SUB_PROTO_MAP = {
        53: dns.DNS_TCP,
        80: http.HTTP,
        #443: https.HTTPS,
    }

    _pack_   = 1
    _fields_ = [
        ('_sport'                  , ctypes.c_uint16),
        ('_dport'                  , ctypes.c_uint16),
        ('_sequence_number'        , ctypes.c_uint32),
        ('_acknowledgement_number' , ctypes.c_uint32),
        ('_offset_tflags'          , ctypes.c_uint16),
        ('_windows'                , ctypes.c_uint16),
        ('_checksum'               , ctypes.c_uint16),
        ('_urgent_pinger'          , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def header_length(self):
        offset_tflags = socket.htons(self._offset_tflags)
        length = (offset_tflags >> 12) * 4
        return length

    def __init__(self, buffer):
        self._payload = buffer[self.header_length():]

    def sport(self): return socket.htons(self._sport)
    def dport(self): return socket.htons(self._dport)

    def __str__(self):
        return '<TCP>\t%s\t->\t%s'%(socket.htons(self.sport), socket.htons(self.dport))

    def __repr__(self):
        return '<%s>'%self.NAME

    def seq(self): return socket.htonl(self._sequence_number)
    def ack(self): return socket.htonl(self._acknowledgement_number)
    def win(self): return socket.htons(self._windows)

    def flags(self):
        offset_tflags = socket.htons(self._offset_tflags)
        flags = ''
        flags += '.F'[(offset_tflags & int('000001', 2)) >> 0]
        flags += '.S'[(offset_tflags & int('000010', 2)) >> 1]
        flags += '.R'[(offset_tflags & int('000100', 2)) >> 2]
        flags += '.P'[(offset_tflags & int('001000', 2)) >> 3]
        flags += '.A'[(offset_tflags & int('010000', 2)) >> 4]
        flags += '.U'[(offset_tflags & int('100000', 2)) >> 5]
        return flags

    def info(self):
        return 'flags=%s seq=%s ack=%s win=%s payload_len=%s'%(
            self.flags(),
            self.seq(),
            self.ack(),
            self.win(),
            len(self._payload)
        )
        #return 'header_len=%s payload_len=%s'%(self.header_length(), len(self._payload))
        #return 'payload=%s'%(self._payload)

    def sub_type(self):
        # handshack without payload
        if len(self._payload) >= ctypes.sizeof(dns.DNS_TCP) and (self.sport() == 53 or self.dport() == 53):
            return 53 
        # len(GET / HTTP/1.0) == 14
        if len(self._payload) >= 14 and 80 in [self.sport(), self.dport()]:
            return 80
        return -1



