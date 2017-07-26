#coding: utf-8

import socket
import struct
import ctypes
import traceback

from . import proto

# FROM: https://wiki.wireshark.org/SSL
# 443 https
# 636 ldaps
# 989 ftps-data
# 990 ftps
# 992 telnets
# 993 imaps
# 994 ircs
# 995 pop3s
# 5061 sips


# Encrypted Alert 
class SSL_EncryptedAlert(ctypes.Structure):
    NAME           = "EncryptedAlert"
    PROTO_NUMBER   = 0x15
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL_EncryptedAlert):]

    def __str__(self):
        return '%s'%(self.NAME)


# Application Data 
class SSL_ApplicationData(ctypes.Structure):
    NAME           = "ApplicationData"
    PROTO_NUMBER   = 0x17
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL_ApplicationData):]

    def __str__(self):
        return '%s'%(self.NAME)


# Client Cipher Spec & Encrypted Handshake Message 
class SSL_CipherSpec(ctypes.Structure):
    NAME           = "CipherSpec"
    PROTO_NUMBER   = 0x14
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
        ('_ccs'    , ctypes.c_ubyte ), # change cipher spec, should be 1?
        ('_type'   , ctypes.c_ubyte ),
        ('_version', ctypes.c_uint16),
        ('_length' , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL_CipherSpec):]

    def __str__(self):
        return '%s'%(self.NAME)


# Client Hello
# Server Hello
# Client Key Change
class SSL_Handshake(ctypes.Structure):
    NAME           = "Handshake"
    PROTO_NUMBER   = 0x16
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
        ('_st_len' , ctypes.c_uint32),
        ('_st_ver' , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL_Handshake):]
        try:
            self.deep_parse()
        except:
            pass

    def deep_parse(self):
        self.random = self._payload[:32]
        self._payload = self._payload[32:]

        self.sid_len = ord(self._payload[:1])
        self._payload = self._payload[1:]

        self.sid_str = self._payload[:self.sid_len]
        self._payload = self._payload[self.sid_len:]

        self.cipher_suit_len = struct.unpack('!H', self._payload[:2])[0]
        self._payload = self._payload[2:]

        cipher_suit_dat = self._payload[:self.cipher_suit_len]
        self._payload = self._payload[self.cipher_suit_len:]

        self.compress_dat = self._payload[:2]
        self._payload = self._payload[2:]

        self.extension_len = struct.unpack('!H', self._payload[:2])[0]
        self._payload = self._payload[2:]

        self.extension_dat = self._payload[:self.extension_len]
        self._payload = self._payload[self.extension_len:]

    def handshake_version(self):
        return socket.htons(self._st_ver)

    def handshake_lens(self):
        return socket.htonl(self._st_len) & 0x00ffffff

    # 0x01 = client hello
    # 0x01 = server hello
    def handshake_type(self):
        return socket.htonl(self._st_len) >> 24

    def handshake_type_tag(self):
        t = self.handshake_type()
        if t == 1:
            return 'client-hello'
        elif t == 2:
            return 'server-hello'
        elif t == 16:
            return 'client-key-exchange'
        else:
            return str(t)

    def application_layer_proto_negotiation(self):
        def find_alpn(dat):
            rets = []
            while dat:
                _type = struct.unpack('!H', dat[0:2])[0]
                _lens = struct.unpack('!H', dat[2:4])[0]
                dat = dat[4:]
                _dat, dat = dat[:_lens], dat[_lens:]
                if _type != 0x0010:
                    continue
                alpn_ext_len = struct.unpack('!H', _dat[0:2])[0]
                _dat = _dat[2:2+alpn_ext_len]
                while _dat:
                    alpn_str_len = ord(_dat[0])
                    alpn_proto_s = _dat[1:1+alpn_str_len]
                    rets.append(alpn_proto_s)
                    _dat = _dat[1+alpn_str_len:]
                break
            return ','.join(rets)
        try: return find_alpn(self.extension_dat)
        except: return ''

    # fucking complicated
    def server_name(self):
        def find_server_name_extension(dat):
            while dat:
                _type = struct.unpack('!H', dat[0:2])[0]
                _lens = struct.unpack('!H', dat[2:4])[0]
                dat = dat[4:]
                _dat, dat = dat[:_lens], dat[_lens:]
                if _type != 0x0000:
                    continue
                #name_list_len = struct.unpack('!H', _dat[:2])
                #name_type     = struct.unpack('!H', _dat[2])
                #name_lens     = struct.unpack('!H', _dat[3:5])
                return _dat[5:]
            return ''
        try: return find_server_name_extension(self.extension_dat)
        except: return ''

    def __str__(self):
        rets = '%s %s'%(self.NAME, self.handshake_type_tag())
        t = self.handshake_type()
        if t == 1 or t == 2:
            sn = self.server_name()
            ap = self.application_layer_proto_negotiation()
            if sn: rets += " To:%s"%sn
            if ap: rets += " ALPN:%s"%ap
        return rets


class SSL(ctypes.Structure):
    NAME           = "SSL"
    PROTO_NUMBER   = 0x16
    _SUB_PROTO_MAP = {
    }
    _MSG_PROTO_MAP = {
        SSL_Handshake.PROTO_NUMBER       : SSL_Handshake,
        SSL_CipherSpec.PROTO_NUMBER      : SSL_CipherSpec,
        SSL_ApplicationData.PROTO_NUMBER : SSL_ApplicationData,
        SSL_EncryptedAlert.PROTO_NUMBER  : SSL_EncryptedAlert,
    }
    _pack_   = 1
    _fields_ = [
        ('_type'   , ctypes.c_ubyte ),
        ('_version', ctypes.c_uint16),
        ('_length' , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL):]
        sm = self._MSG_PROTO_MAP.get(self.sub_type(), None)
        if sm:
            sobj = sm(self._payload)
            self.Message = sobj
        else:
            self.Message = None
            setattr(self, proto.ParseError, True)

    # 0x0301 769 == TLS1.0
    # 0x0303 771 == TLS1.0
    def version(self):
        return socket.htons(self._version)

    def length(self):
        return socket.htons(self._length)

    def sub_type(self):
        return self._type

    def info(self):
        return str(self.Message)



