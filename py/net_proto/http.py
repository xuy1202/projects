#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import helper


class HTTP(ctypes.Structure):
    PROTO_NUMBER   = 80
    NAME           = "HTTP"
    _SUB_PROTO_MAP = {
    }

    def __init__(self, buffer):
        items = buffer.split('\r\n\r\n', 1)
        if len(items) == 2:
            header, page = items
        else:
            header, page = items[0], ''
        self.headers = header.split('\r\n')
        self._payload = page

    def info(self):
        #helper.hexdump(self._payload)
        #for line in self._payload.splitlines():
            #print line
        for h in self.headers:
            print h
        print repr(self._payload)
        return ''

    def sub_type(self):
        return -1




