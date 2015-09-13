#coding: utf-8

import os, sys
import logging
import optparse
import traceback

import tornado.ioloop
import tornado.web
import tornado.httpserver

import conf
from utils import ui_methods
from utils import daemon
from utils import runtime
from utils import iputil

from handler import main as main_handler


url_patterns = [
        (r"/.*", main_handler.MainHandler),
]


def Init():
    for line in open("test.txt"):
        w, m, s = line.strip().split(" ", 2)
        main_handler.GlobalDatas.append(main_handler.Word(w, m, s))
    return True


def main(Port=8080):
    #runtime.Hostname = iputil.hostname()
    Init()
    runtime.HttpPort = Port

    application = tornado.web.Application(url_patterns
        , ui_methods = ui_methods
        , **conf.tornado_settings
    )
    http_server = tornado.httpserver.HTTPServer(application)
    if options.inner.lower() == 'true':
        http_server.bind(Port, iputil.inner_ip())
    else:
        http_server.bind(Port)
    http_server.start(conf.MainConf.single().process)
    logger.info("PyServer Listen On: %s"%Port)
    tornado.ioloop.IOLoop.instance().start()


if __name__ == "__main__":
    global options

    parser = optparse.OptionParser()
    parser.add_option('-d', '--daemon', 
                    action="store_true", dest="daemon", help="run as daemon")
    parser.add_option('-p', '--port', 
                    action="store", default=80, dest="port", type="int", 
                    help="listen port. default:%default")
    parser.add_option('-v', '--verbose', 
                    action="store_true", dest="verbose", help="set log level to debug")
    parser.add_option('-I', '--inner',
        action="store", dest="inner", type="string", default="false", help="")
    parser.add_option('-M', '--modules'
        , action="store", dest="modules"
        , type = "string", default="", help="set log level to debug"
    )

    options, args = parser.parse_args()

    if options.verbose:
        log_level = logging.DEBUG
    else:
        log_level = conf.LOG_LEVEL

    log_stdout = False if options.daemon else True
    conf.LogConfig("websrv", log_level, conf.LOG_FMT, conf.LOG_DATE_FMT, conf.LOG_PATH, log_stdout)
    logger = logging.getLogger("websrv.server")
    
    port =  args[0] if len(args) else options.port
    if options.daemon:
        conf.MainConf.single().DEBUG = False
        daemon.daemonize(main, port)
    else:
        main(port)


