#coding: utf-8

import os, sys
import socket
import struct
import ctypes
import traceback

from . import helper


_RtypeFMap = {
    1     : 'A',
    2     : 'NS',
    3     : 'MD',
    5     : 'CNAME',
    6     : 'SOA',
    12    : 'PTR',
    15    : 'MX',
    16    : 'TXT',
    17    : 'RP',
    18    : 'AFSDB',
    24    : 'SIG',
    25    : 'KEY',
    28    : 'AAAA',
    29    : 'LOC',
    33    : 'SRV',
    35    : 'NAPTR',
    36    : 'KX',
    37    : 'CERT',
    39    : 'DNAME',
    42    : 'APL',
    43    : 'DS',
    44    : 'SSHFP',
    45    : 'IPSECKEY',
    46    : 'RRSIG',
    47    : 'NSEC',
    48    : 'DNSKEY',
    49    : 'DHCID',
    50    : 'NSEC3',
    51    : 'NSEC3PARAM',
    52    : 'TLSA',
    55    : 'HIP',
    99    : 'SPF',
    249   : 'TKEY',
    250   : 'TSIG',
    255   : 'ANY',
    257   : 'CAA',
    32768 : 'TA',
    32769 : 'DLV',     
}

_RtypeRmap = {
    'A'         : 1,
    'NS'        : 2,
    'MD'        : 3,
    'CNAME'     : 5,
    'SOA'       : 6,
    'PTR'       : 12,
    'MX'        : 15,
    'TXT'       : 16,
    'RP'        : 17,
    'AFSDB'     : 18,
    'SIG'       : 24,
    'KEY'       : 25,
    'AAAA'      : 28,
    'LOC'       : 29,
    'SRV'       : 33,
    'NAPTR'     : 35,
    'KX'        : 36,
    'CERT'      : 37,
    'DNAME'     : 39,
    'APL'       : 42,
    'DS'        : 43,
    'SSHFP'     : 44,
    'IPSECKEY'  : 45,
    'RRSIG'     : 46,
    'NSEC'      : 47,
    'DNSKEY'    : 48,
    'DHCID'     : 49,
    'NSEC3'     : 50,
    'NSEC3PARAM': 51,
    'TLSA'      : 52,
    'HIP'       : 55,
    'SPF'       : 99,
    'TKEY'      : 249,
    'TSIG'      : 250,
    'IFXR'      : 251,
    'AFXR'      : 252,
    'CAA'       : 257,
    'ANY'       : 255,
    'TA'        : 32768,
    'DLV'       : 32769,
}


def get_type_name(t): return _RtypeFMap.get(t, t)
def get_type_code(t): return _RtypeRmap.get(t, t)


class DNS(ctypes.Structure):
    NAME           = "DNS"
    PROTO_NUMBER   = -1 
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(self):]
        self.parse_records()

    def sub_type(self): return -1

    def transaction_id(self): return socket.htons(self._transaction_id)
    def flags(self)         : return socket.htons(self._flags)
    def query_len(self)     : return socket.htons(self._query_len)
    def response_len(self)  : return socket.htons(self._response_len)
    def authority_len(self) : return socket.htons(self._authority_len)
    def additional_len(self): return socket.htons(self._additional_len)

    def rcode(self):
        rc = self.flags() & 0x0f
        return rc

    def tag(self):
        return 'R' if (self.flags() >> 15) else 'Q'

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
                part, _ = self._read_name(self._payload[pointer - ctypes.sizeof(DNS_UDP):])
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
        ttls  = struct.unpack('!I', buff[0:4])[0]
        dlen  = struct.unpack('!H', buff[4:6])[0]
        rdata = buff[6:6+dlen]
        buff  = buff[6+dlen:]
        rtype = query[0]
        if rtype == 1: # A
            rdata = helper.uint2ip(struct.unpack('I', rdata)[0])
        elif rtype in [2,5]: # NS,CNAME
            rdata, _ = self._read_name(rdata)
        elif rtype == 6: # SOA
            primary_name_server, index = self._read_name(rdata)
            rdata = rdata[index:]
            responsible_authority_mailbox, index = self._read_name(rdata)
            rdata = rdata[index:]
            rdata = '%s %s %d %d %d %d %d'%(
                primary_name_server, responsible_authority_mailbox,
                struct.unpack('!I', rdata[ 0:4 ])[0], # serial number
                struct.unpack('!I', rdata[ 4:8 ])[0], # refresh interval
                struct.unpack('!I', rdata[ 8:12])[0], # retry interval
                struct.unpack('!I', rdata[12:16])[0], # expire limit
                struct.unpack('!I', rdata[16:20])[0], # minumum ttl
            )
        elif rtype == 15: # MX
            prefe = struct.unpack('!H', rdata[0:2])[0] # preference
            mx_name, _ = self._read_name(rdata[2:])
            rdata = "%d %s"%(prefe, mx_name)
        elif rtype == 16: # TXT
            tlen = ord(rdata[0]) # TXT length
            rdata = rdata[1:]    # TXT
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
        except Exception, e:
            #print traceback.format_exc()
            self.parse_error = True

    def qtypes(self):
        return ','.join(list(set([str(get_type_name(i[0])) for i in self.querys])))

    def qnames(self):
        return ','.join(list(set([i[2] for i in self.querys])))

    def where_are_datas(self):
        datas = self.responses
        if not datas: datas = self.authoritys
        if not datas: datas = self.additionals
        return datas

    def rtypes(self):
        rtypes = ','.join(list(set([str(get_type_name(i[0])) for i in self.where_are_datas()])))
        if rtypes:
            return rtypes
        else:
            return self.qtypes()

    def rnames(self):
        rnames = ','.join(list(set([i[2] for i in self.where_are_datas()])))
        if rnames:
            return rnames
        else:
            return self.qnames()

    def ttls(self):
        return ','.join(list(set([str(i[3]) for i in self.where_are_datas()])))

    def rdatas(self):
        rcode = self.rcode()
        if rcode == 0 and self.where_are_datas():
            return ','.join(list(set([i[4] for i in self.where_are_datas()])))
        elif rcode == 0:
            return 'EMPTY'
        else:
            return 'e%d'%rcode

    def detail(self):
        print self.querys
        print self.responses
        print self.authoritys
        print self.additionals

    def info(self):
        tag = self.tag()
        qs = '; '.join(['%s %s'%(get_type_name(q[0]), q[2]) for q in self.querys])
        if tag == 'Q':
            info = qs
        else:
            rs = '; '.join(['%s->%s:%s'%(q[2], get_type_name(q[0]), q[4]) for q in self.responses])
            a1 = '; '.join(['%s->%s:%s'%(q[2], get_type_name(q[0]), q[4]) for q in self.authoritys])
            a2 = '; '.join(['%s->%s:%s'%(q[2], get_type_name(q[0]), q[4]) for q in self.additionals])
            info = '%s RS:{%s} AU:{%s} AD:{%s}'%(qs, rs, a1, a2)
            #info = '%s %s -> %s ttl=%s'%(self.rtypes(), self.rnames(), self.rdatas(), self.ttls())
        return '%s tid=%s %s'%(self.tag(),
            self.transaction_id(),
            info,
        )

    def __str__(self):
        info = self.info()
        return '<DNS> ' + info
        return '%s %s tid=%s, flags=%s, query=%s, response=%s, authority=%s, additional=%s'%(self.tag(),
            info,
            self.transaction_id(),
            self.flags(),
            self.query_len(),
            self.response_len(),
            self.authority_len(),
            self.additional_len(),
        )


class DNS_UDP(DNS):
    _fields_ = [
        ('_transaction_id' , ctypes.c_uint16),
        ('_flags'          , ctypes.c_uint16),
        ('_query_len'      , ctypes.c_uint16),
        ('_response_len'   , ctypes.c_uint16),
        ('_authority_len'  , ctypes.c_uint16),
        ('_additional_len' , ctypes.c_uint16),
    ]


class DNS_TCP(DNS):
    _fields_ = [
        ('_package_len'    , ctypes.c_uint16),
        ('_transaction_id' , ctypes.c_uint16),
        ('_flags'          , ctypes.c_uint16),
        ('_query_len'      , ctypes.c_uint16),
        ('_response_len'   , ctypes.c_uint16),
        ('_authority_len'  , ctypes.c_uint16),
        ('_additional_len' , ctypes.c_uint16),
    ]



