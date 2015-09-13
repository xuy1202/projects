#coding: utf-8

import os
from os.path import dirname
from os.path import abspath
import logging
import logging.handlers


class MainConf(object):
    @classmethod
    def single(cls):
        if not hasattr(cls, '__single__'):
            cls.__single__ = cls()
        return cls.__single__

    def __init__(self):
        self.src_path    = dirname(abspath(__file__))
        self.srv_path    = dirname(self.src_path)
        self.log_path    = os.sep.join([self.srv_path, 'log'])
        self.etc_path    = os.sep.join([self.srv_path, 'etc'])
        self.dat_path    = os.sep.join([self.srv_path, 'dat'])
        self.html_path   = os.sep.join([self.src_path, 'www'])
        self.hostname    = os.popen('hostname').read().strip()
        self.pid         = os.getpid()
        self.process     = 1
        self.DEBUG       = False 


tornado_settings = { 
    "static_path"   : MainConf.single().html_path + os.sep + "static",
    "template_path" : MainConf.single().html_path,
    "gzip"  : False,
    "xsrf_cookies": False,
    'debug': MainConf.single().DEBUG,
}


#Log config

if tornado_settings['debug']:
    LOG_LEVEL = logging.DEBUG
else:
    LOG_LEVEL = logging.INFO

LOG_FMT      = '%(asctime)s [%(name)s] %(levelname)s: %(message)s'
LOG_DATE_FMT = "%Y-%m-%d %H:%M:%S"
LOG_PATH     = MainConf.single().log_path


class LogConfig:
    def __init__(self, filename, level, format, datefmt, path=None, stdout = False, syslog = False):
        logging.root.setLevel(level)
        self.setFileHandler(filename, path, format, datefmt)
        if stdout:
            self.setStreamHandler(format, datefmt)
        if syslog:
            self.setSYSLogHandler(format, datefmt, syslog)

    def setFileHandler(self, filename, path, fmt, datefmt):
        logname = "%s.log.%d" %(filename, os.getpid())
        logfile = os.path.join(path, logname)
        file = logging.handlers.TimedRotatingFileHandler(logfile, interval=60)
        formatter = logging.Formatter(fmt=fmt, datefmt=datefmt)
        file.setFormatter(formatter)
        logging.getLogger().addHandler(file)

    def setStreamHandler(self, fmt, datefmt):
        formatter = logging.Formatter(fmt=fmt, datefmt=datefmt)
        stream = logging.StreamHandler()
        stream.setFormatter(formatter)
        logging.getLogger().addHandler(stream)

    def setSYSLogHandler(self, fmt, datefmt, address):
        syslog = logging.handlers.SysLogHandler(address=address)
        formatter = logging.Formatter(fmt=fmt, datefmt=datefmt)
        syslog.setFormatter(formatter)
        logging.getLogger().addHandler(syslog)





