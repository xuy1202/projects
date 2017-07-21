#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import helper


class ICMP_REQUEST(ctypes.Structure):
    NAME = "echo_request"

    _pack_   = 1
    _fields_ = [
        ('_id'    , ctypes.c_uint16),
        ('_seq'   , ctypes.c_uint16),
    ]

    def __new__(self, buffer):
        return self.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(ICMP_REQUEST):]

    def __str__(self):
        return 'type=%s id=%s seq=%s payload_len=%s'%(self.NAME, socket.htons(self._id), socket.htons(self._seq), len(self._payload))

    def __repr__(self):
        return '<%s>'%self.NAME


class ICMP_REPLY(ctypes.Structure):
    NAME = "echo_reply"

    _pack_   = 1
    _fields_ = [
        ('_id'    , ctypes.c_uint16),
        ('_seq'   , ctypes.c_uint16),
    ]

    def __new__(self, buffer):
        return self.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(ICMP_REPLY):]

    def __str__(self):
        return 'type=%s id=%s seq=%s payload_len=%s'%(self.NAME, socket.htons(self._id), socket.htons(self._seq), len(self._payload))

    def __repr__(self):
        return '<%s>'%self.NAME


class _ICMP_BASE(ctypes.Structure):
    def __new__(self, buffer):
        return self.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(self):]

    def __str__(self):
        return 'type=%s payload_len=%s'%(self.NAME, len(self._payload))

    def __repr__(self):
        return '<%s>'%self.NAME


class ICMP_TIMEEXCEED(_ICMP_BASE):
    NAME = "time_exceeded_in-transit"

class ICMP_UNREACHABLE(_ICMP_BASE):
    NAME = "unreachable"

class ICMP_PORT_UNREACHABLE(ICMP_UNREACHABLE):
    _pack_   = 1
    _fields_ = [
        # ip header
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
        # top 8 bytes of tcp/udp header
        ('_sport'       , ctypes.c_uint16),
        ('_dport'       , ctypes.c_uint16),
        ('_seq_len_chk' , ctypes.c_uint16),
    ]
    def __str__(self):
        return '%s:dport=%s payload_len=%s'%(self.NAME, socket.htons(self._dport), len(self._payload))


class ICMP(ctypes.Structure):
    PROTO_NUMBER   = 1
    NAME           = "ICMP"
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
        ('_type'    , ctypes.c_ubyte ),
        ('_code'    , ctypes.c_ubyte ),
        ('_checksum', ctypes.c_uint16),
        ('_unused'  , ctypes.c_uint32),
    ]

    def __new__(self, buffer):
        return self.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(ICMP):]

    def __str__(self):
        return '<%s> type:%s, code: %s, checksum: %s, payload_len: %s'%(self.NAME, self._type, self._code, self._checksum, len(self._payload))

    def __repr__(self):
        return '<%s>'%self.NAME

    def info(self):
        tc = '%s_%s'%(hex(self._type), hex(self._code))
        if tc == "0x0_0x0":
            o = ICMP_REPLY(self._payload)
            return str(o)
        if tc == "0x8_0x0":
            o = ICMP_REQUEST(self._payload)
            return str(o)
        if tc == "0xb_0x0":
            o = ICMP_TIMEEXCEED(self._payload)
            return str(o)
        if tc == "0x3_0x3":
            o = ICMP_PORT_UNREACHABLE(self._payload)
            return str(o)
        return 'type_code=%s'%tc

    def sub_type(self):
        #print repr(self._payload)
        return -1


