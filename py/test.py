#coding: utf-8

import socket
import traceback

import helper
from data import udp_data
from net_proto import ip 
from net_proto import udp
from net_proto import dns
from net_proto import packet
from net_proto import ethernet

#data = helper.reform_from_tcpdump(udp_data.dns_cybatk_com_response)
#data = helper.reform_from_tcpdump(udp_data.dns_www_163_com_response)
data = helper.reform_from_tcpdump(udp_data.dns_baidu_com_response)


P = packet.Packet()
P.parse_from_buffer(data, dns.DNS)
print P




