#codint: utf-8

import time
import sys
import logging
import threading
import traceback

import zmq


logger = logging.getLogger("logsrv.zmqwrap")


class ZmqBase(object):
    _context = zmq.Context()

    def bind(self, addr):
        self._sock.bind(addr)

    def connect(self, addr):
        self._sock.connect(addr)


class SubBase(ZmqBase):
    def __init__(self):
        self._sock = self._context.socket(zmq.SUB)
        self._cb = lambda a: None

    def bind_key(self, skey):
        self._sock.setsockopt(zmq.SUBSCRIBE, skey)

    def register_callback(self, func):
        self._cb = func

    def start(self):
        while 1:
            try:
                item = self._sock.recv_multipart()
                if len(item) != 2:
                    raise RuntimeError("Error-Multipart: " + str(item))
                key, msg = item
                self._cb(key, msg)
            except Exception, e:
                logger.error(str(traceback.format_exc()))


class SubClient(SubBase):
    def __init__(self, addr=None):
        super(SubClient, self).__init__()
        if addr:
            self.connect(addr)


class SubServer(SubBase):
    def __init__(self, addr):
        super(SubServer, self).__init__()
        self.bind(addr)


