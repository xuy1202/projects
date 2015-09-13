#coding: utf-8

import re
from socket import inet_pton, AF_INET
from struct import unpack
from socket import gethostname, gethostbyname


def hostname():
    return gethostname()
 

def simple_hostname():
    hostname = hostname()
    try:
        parts = hostname.split(".")
        return '%s.%s'%(parts[0], parts[2])
    except:
        return hostname

def inner_ip():
    return gethostbyname(hostname())


def ip_to_int(ipstr):
    a, b, c, d = ipstr.split(".")
    a = int(a)
    b = int(b)
    c = int(c)
    d = int(d)
    return (a<<24) + (b<<16) + (c<<8) +d


def int_to_ipv4(n):
    n = int(n)
    return "%s.%s.%s.%s"%(
        str((n & 0xff000000 ) >> 24),
        str((n & 0x00ff0000 ) >> 16),
        str((n & 0x0000ff00 ) >>  8),
        str((n & 0x000000ff ) >>  0)
    );


def int_to_ipf(n):
    n = int(n)
    return "%s.%s.%s.%s"%(
        str((n & 0xff000000 ) >> 24).rjust(3, '0'),
        str((n & 0x00ff0000 ) >> 16).rjust(3, '0'),
        str((n & 0x0000ff00 ) >>  8).rjust(3, '0'),
        str((n & 0x000000ff ) >>  0).rjust(3, '0')
    );


pattern_single_ip = re.compile("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$")
pattern_prefix_ip = re.compile("^[0-9]{1,3}\.(\.?[0-9]{1,3}){1,3}\.\*$")
pattern_masked_ip = re.compile("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}/[0-9]{1,2}$")


def ip_to_ipf(ipstr):
    l = [i.rjust(3, "0") for i in ipstr.split(".") if i]
    return ".".join(l)

def is_ipv4(ipstr):
    return pattern_single_ip.match(ipstr)

def is_ipv4_with_mask(ipstr):
    return pattern_masked_ip.match(ipstr)


def ip_expand(ipstr, mask_limit=24):
    if pattern_single_ip.match(ipstr):
        ipstrf = int_to_ipf(ip_to_int(ipstr))
        return [ipstrf, ipstrf]
    if pattern_masked_ip.match(ipstr):
        ipstr, mask = ipstr.split("/", 1)
        if int(mask) < mask_limit:
            mask = str(mask_limit)
        ipnum = ip_to_int(ipstr)
        mask  = 32 - int(mask)
        if mask < 32:
            beg = ipnum >> mask << mask
            end = beg + (2**mask)
            return [int_to_ipf(beg), int_to_ipf(end)]
    return []


def is_private_ip(ip):
    f = unpack('!I',inet_pton(AF_INET,ip))[0]
    private = (
        [ 2130706432, 4278190080 ], # 127.0.0.0,   255.0.0.0   
        [ 3232235520, 4294901760 ], # 192.168.0.0, 255.255.0.0
        [ 2886729728, 4293918720 ], # 172.16.0.0,  255.240.0.0 
        [ 167772160,  4278190080 ], # 10.0.0.0,    255.0.0.0
    )
    for net in private:
        if (f & net[1] == net[0]):
            return True
    return False


def tflags_str(flg_num):
    #if((flg_num & 0x01) == 0x01): flg_str += "FIN_"); 
    #if((flg_num & 0x02) == 0x02): flg_str += "SYN_");
    #if((flg_num & 0x04) == 0x04): flg_str += "RST_");
    #if((flg_num & 0x08) == 0x08): flg_str += "PSH_");
    #if((flg_num & 0x10) == 0x10): flg_str += "ACK_");
    #if((flg_num & 0x20) == 0x20): flg_str += "URG_");
    #if((flg_num & 0x40) == 0x40): flg_str += "ECE_");
    #if((flg_num & 0x80) == 0x80): flg_str += "CWR_");
    flg_str = '.'
    if((flg_num & 0x01) == 0x01): flg_str += "F"
    if((flg_num & 0x02) == 0x02): flg_str += "S"
    if((flg_num & 0x04) == 0x04): flg_str += "R"
    if((flg_num & 0x08) == 0x08): flg_str += "P"
    if((flg_num & 0x10) == 0x10): flg_str += "A"
    if((flg_num & 0x20) == 0x20): flg_str += "U"
    if((flg_num & 0x40) == 0x40): flg_str += "E"
    if((flg_num & 0x80) == 0x80): flg_str += "C"
    return flg_str


if __name__ == "__main__":
    print ip_expand("1.1.1.1/24")
    print ip_to_int("1.1.1.1")
    print ip_to_int("255.255.255.255")
    print int_to_ip("16843009")
    print int_to_ip("4294967295")
    c = 0
    for line in open("/home/xuyang-pd/qdev/NSRL-dns/code/src/xdns-webeng/etc/filter_ip.txt"):
        line = line.strip()
        if not line:
            continue
        if line.startswith("#"):
            continue
        iplist = ip_expand(line)
        print len(iplist), iplist
        c += len(iplist)
    print "=================="
    print c



