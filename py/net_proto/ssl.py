#coding: utf-8

import socket
import struct
import ctypes
import traceback


class SSL_Handshake(ctypes.Structure):
    NAME           = "SSL_Handshake"
    PROTO_NUMBER   = 0x16
    _SUB_PROTO_MAP = {
    }

    _pack_   = 1
    _fields_ = [
        ('_type'   , ctypes.c_ubyte ),
        ('_version', ctypes.c_uint16),
        ('_length' , ctypes.c_uint16),
        ('_st_len' , ctypes.c_uint32),
        ('_st_ver' , ctypes.c_uint16),
    ]

    def __new__(cls, buffer):
        return cls.from_buffer_copy(buffer)

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

    def __init__(self, buffer):
        self._payload = buffer[ctypes.sizeof(SSL_Handshake):]
        try:
            self.deep_parse()
        except:
            pass

    # 0x0301 769 == TLS1.0
    # 0x0303 771 == TLS1.0
    def version(self):
        return socket.htons(self._version)
    def handshake_version(self):
        return socket.htons(self._st_ver)

    def length(self):
        return socket.htons(self._length)

    # 0x01 = client hello
    # 0x01 = server hello
    def handshake_type(self):
        return socket.htonl(self._st_len) >> 24

    def handshake_lens(self):
        return socket.htonl(self._st_len) & 0x00ffffff

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


if __name__ == "__main__":
    dat = '\x16\x03\x01\x00\xcc\x01\x00\x00\xc8\x03\x03\xa7}(\xd0\x85\x96\x08\xd2\n\x7f]\x08\xeaE\xea\xfc\x87~\xfa\xbb\xb2\xfag\xf0g\xcb\xf4\x04`>\x196\x00\x00\x1c\xaa\xaa\xc0+\xc0/\xc0,\xc00\xcc\xa9\xcc\xa8\xc0\x13\xc0\x14\x00\x9c\x00\x9d\x00/\x005\x00\n\x01\x00\x00\x83**\x00\x00\xff\x01\x00\x01\x00\x00\x00\x00\x1a\x00\x18\x00\x00\x15www.hypercomments.com\x00\x17\x00\x00\x00#\x00\x00\x00\r\x00\x14\x00\x12\x04\x03\x08\x04\x04\x01\x05\x03\x08\x05\x05\x01\x08\x06\x06\x01\x02\x01\x00\x05\x00\x05\x01\x00\x00\x00\x00\x00\x12\x00\x00\x00\x10\x00\x0e\x00\x0c\x02h2\x08http/1.1uP\x00\x00\x00\x0b\x00\x02\x01\x00\x00\n\x00\n\x00\x08zz\x00\x1d\x00\x17\x00\x18\xda\xda\x00\x01\x00'
    dat = '\x16\x03\x01\x00\xec\x01\x00\x00\xe8\x03\x03?\x08F\xd3\x95\xc7\xd1\xe6h\xd16\xd6\xe8K\x0c\xbcf\xbd\xaa\x9e\xce0\x15\x9a\xfc\x14\xcem\x04}\x9c\xf7 \xd8\xc6St\xb8\xe6\xa3\xf9\x84Sz\x86D\xe8\r\x17u0\x17\xe1\xfa?B\xb2DC.\xfa\xfb\x1f\x8c\xa1\x00\x1c::\xc0+\xc0/\xc0,\xc00\xcc\xa9\xcc\xa8\xc0\x13\xc0\x14\x00\x9c\x00\x9d\x00/\x005\x00\n\x01\x00\x00\x83JJ\x00\x00\xff\x01\x00\x01\x00\x00\x00\x00\x1a\x00\x18\x00\x00\x15www.hypercomments.com\x00\x17\x00\x00\x00#\x00\x00\x00\r\x00\x14\x00\x12\x04\x03\x08\x04\x04\x01\x05\x03\x08\x05\x05\x01\x08\x06\x06\x01\x02\x01\x00\x05\x00\x05\x01\x00\x00\x00\x00\x00\x12\x00\x00\x00\x10\x00\x0e\x00\x0c\x02h2\x08http/1.1uP\x00\x00\x00\x0b\x00\x02\x01\x00\x00\n\x00\n\x00\x08\n\n\x00\x1d\x00\x17\x00\x18\xda\xda\x00\x01\x00'
    SSLPack = SSL_Handshake(dat)
    print repr(SSLPack.server_name())


