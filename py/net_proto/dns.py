#coding: utf-8

import os, sys
import socket
import struct
import ctypes
import traceback

from . import helper


class DNS(ctypes.Structure):
    NAME           = "DNS"
    PROTO_NUMBER   = -1 
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
        ('_transaction_id' , ctypes.c_uint16),
        ('_flags'          , ctypes.c_uint16),
        ('_query_len'      , ctypes.c_uint16),
        ('_response_len'   , ctypes.c_uint16),
        ('_authority_len'  , ctypes.c_uint16),
        ('_additional_len' , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(DNS):]

    def sub_type(self): return -1

    def transaction_id(self): return socket.htons(self._transaction_id)
    def flags(self)         : return socket.htons(self._flags)
    def query_len(self)     : return socket.htons(self._query_len)
    def response_len(self)  : return socket.htons(self._response_len)
    def authority_len(self) : return socket.htons(self._authority_len)
    def additional_len(self): return socket.htons(self._additional_len)

    def tag(self):
        return 'response' if (self.flags() >> 15) else 'query'

    def _read_name(self, data):
        parts = []
        index = 0
        while 1:
            plens = ord(data[index])
            index += 1
            if plens == 0: # 0x00 ending
                break
            elif plens >= 192: #0b(11......) means pointer compress
                pointer = ((((plens << 8) + ord(data[index])) << 2) & 0xffff) >> 2
                part, _ = self._read_name(self._payload[pointer - ctypes.sizeof(DNS):])
                parts.append(part)
                index += 1
                break
            else:
                part  = data[index:index+plens]
                index += plens
                parts.append(part)
        return '.'.join(parts), index

    def _parse_one_query(self, data):
        rname, index = self._read_name(data)
        rtype  = struct.unpack('!H', data[index+0:index+2])[0]
        rclass = struct.unpack('!H', data[index+2:index+4])[0]
        data = data[index+4:]
        return [rtype, rclass, rname], data

    def _parse_one_response(self, buff):
        query, buff = self._parse_one_query(buff)
        ttls = struct.unpack('!I', buff[0:4])[0]
        dlen = struct.unpack('!H', buff[4:6])[0]
        rdata = buff[6:6+dlen]
        buff = buff[6+dlen:]
        rtype = query[0]
        if rtype == 1: # A
            rdata = helper.uint2ip(struct.unpack('I', rdata)[0])
        elif rtype == 2 or rtype == 5: # NS or CNAME
            rdata, _ = self._read_name(rdata)
        else:
            pass
        return query + [ttls, rdata], buff 

    def parse_records(self):
        self.querys = []
        self.responses = []
        self.authoritys = []
        self.additionals = []
        try:
            data = self._payload
            qcount = 0
            while qcount < self.query_len():
                query, data = self._parse_one_query(data)
                self.querys.append(query)
                qcount += 1
            rcount = 0
            rtotal = self.response_len() + self.authority_len() + self.additional_len()
            r_list = []
            while rcount < rtotal:
                response, data = self._parse_one_response(data)
                r_list.append(response)
                rcount += 1
            self.responses  , r_list = r_list[:self.response_len()  ], r_list[self.response_len()  :]
            self.authoritys , r_list = r_list[:self.authority_len() ], r_list[self.authority_len() :]
            self.additionals, r_list = r_list[:self.additional_len()], r_list[self.additional_len():]
            return True
        except:
            return False

    def qnames(self):
        return ','.join(list(set([i[2] for i in self.querys])))

    def rnames(self):
        return ','.join(list(set([i[2] for i in self.responses])))

    def rdatas(self):
        return ','.join(list(set([i[4] for i in self.responses])))

    def __str__(self):
        self.parse_records()
        tag = self.tag()
        if tag == 'query':
            info = self.qnames()
        else:
            info = '%s -> %s'%(self.rnames(), self.rdatas())
        return 'DNS:%s tid=%s %s'%(self.tag(),
            self.transaction_id(),
            info,
        )
        return 'DNS:%s %s tid=%s, flags=%s, query=%s, response=%s, authority=%s, additoonal=%s'%(self.tag(),
            info,
            self.transaction_id(),
            self.flags(),
            self.query_len(),
            self.response_len(),
            self.authority_len(),
            self.additional_len(),
        )


