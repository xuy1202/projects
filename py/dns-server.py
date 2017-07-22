#coding: utf-8

import os, sys
from collections import defaultdict

from twisted.internet import reactor
from twisted.internet import defer
from twisted.names import client
from twisted.names import dns
from twisted.names import error
from twisted.names import server
from twisted.names import common 


R = client.Resolver(resolv='/etc/resolv.conf')


class DynamicResolver(object):

    record = defaultdict(int)

    def _getA(self, name):
        self.record[name] += 1
        #if self.record[name] <= 1:
        #    ip = "45.77.11.22"
        #else:
        #    ip = "127.0.0.1"
        ip = "127.0.0.1"
        ip = "192.168.3.2"
        print name + " ===> " + ip
        answer = dns.RRHeader(
            name = name,
            type = dns.A,
            cls = dns.IN,
            ttl = 0,
            payload = dns.Record_A(address = b'%s' % ip, ttl=0)
        )
        answers = [answer]
        authority = []
        additional = []
        return answers, authority, additional

    def _getAny(self, name):
        answer = dns.RRHeader(
            name = name,
            type = dns.TXT,
            cls = dns.IN,
            ttl = 0,
            payload = dns.Record_TXT("fuck off here", ttl=65535)
        )
        answers = [answer]
        authority = []
        additional = []
        return answers, authority, additional

    def _doDynamicResponse(self, query):
        name = query.name.name
        return self._getA(name)
        if name.endswith('.163.com'):
            return self._getA(name)
        else:
            return R.query(query)

    def query(self, query, timeout=None):
        return defer.succeed(self._doDynamicResponse(query))

    def lookupAllRecords(self, name, timeout=None):
        return defer.succeed(self._getAny(name))


def main():
    factory = server.DNSServerFactory(
        clients=[DynamicResolver(), client.Resolver(resolv='/etc/resolv.conf')]
    )
    reactor.listenTCP(53, factory)
    protocol = dns.DNSDatagramProtocol(controller=factory)
    reactor.listenUDP(53, protocol)
    reactor.run()

if __name__ == '__main__':
    raise SystemExit(main())


