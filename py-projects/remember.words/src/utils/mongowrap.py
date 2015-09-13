#!/usr/bin/env python
import sys, os
import json
import logging
import urllib2
import traceback
from pymongo import MongoReplicaSetClient, MongoClient


logger = logging.getLogger("logsrv.mongowrap")


def get_app_conf():
    try:
        conf = urllib2.urlopen("http://als5.sec.bjcc.qihoo.net/qconf_mongo/").read()
        cond = json.loads(conf)
        return cond
    except Exception, e:
        logger.error(str(traceback.format_exc()))
        return {}


class MongoInterface(object):
    def __init__(self, dbname, tablename):
        self.conn      = None
        self.db        = None
        self.table     = None
        self.dbname    = dbname
        self.tablename = tablename

    def __del__(self):
        try:
            self.db    = None
            self.table = None
            self.conn.disconnect()
            self.conn  = None
        except Exception, e:
            print traceback.format_exc()

    def connect(self):
        try:
            self.conf = get_app_conf()
            conn = self.conf.get("MONGO_CONNECTION", None)
            repl = self.conf.get("MONGO_REPLICASET", None)
            if conn and repl:
                logger.info('MongoInterface Instance: %s-%s'%(conn, repl))
                self.conn = MongoReplicaSetClient(conn, replicaSet=repl)
                self.db = getattr(self.conn, self.dbname)
                self.table = getattr(self.db, self.tablename)
                return True
        except Exception, e:
            logger.error(str(traceback.format_exc()))
            return False

    def collection(self):
        return self.table

    def insert(self, d):
        try:
            self.table.insert(d)
            return True
        except Exception, e:
            logger.error(str(traceback.format_exc()))
            self.connect()
        return False

    def update(self, d):
        try:
            self.table.update(d)
            return True
        except Exception, e:
            logger.error(str(traceback.format_exc()))
            self.connect()
        return False

    def find(self, d):
        try:
            return self.table.find(d)
        except Exception, e:
            logger.error(str(traceback.format_exc()))
            self.connect()
        return None 


