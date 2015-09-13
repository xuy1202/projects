#coding: utf-8


import geoip2.database


G_GeoHandler = None
G_BgpHandler = None


def Init(geo_path = '/data/GeoInfo/GeoLite2-City.mmdb', bgp_path="/data/GeoInfo/GeoIPASNum2.csv"):
    try:
        global G_GeoHandler
        global G_BgpHandler
        G_GeoHandler = geoip2.database.Reader(geo_path)
        G_BgpHandler = [(int(i), int(j), str(k)) for i, j, k in [
                i.split(",", 2) for i in open(bgp_path).readlines()
            ]
        ]
        return True
    except Exception, e:
        print e
        return False


def ip_to_int(ipstr):
    a, b, c, d = ipstr.split(".")
    a = int(a)
    b = int(b)
    c = int(c)
    d = int(d)
    return (a<<24) + (b<<16) + (c<<8) +d


def int_to_ip(n):
    n = int(n)
    return "%s.%s.%s.%s"%(
        (n & 0xff000000 ) >> 24,
        (n & 0x00ff0000 ) >> 16,
        (n & 0x0000ff00 ) >> 8,
        (n & 0x000000ff ) >> 0
    );


def getBgpInfo(ipstr):
    try:
        global G_BgpHandler
        if G_BgpHandler is None:
            Init()
        ipnum = ip_to_int(ipstr)
    except Exception, e:
        return None
    lo = 0
    hi = len(G_BgpHandler)
    while lo < hi:
        mid = (lo+hi)//2
        if G_BgpHandler[mid][1] < ipnum:
            lo = mid+1
        else:
            hi = mid
    item = G_BgpHandler[lo]
    if item[0] <= ipnum <= item[1]:
        return item[2]
    return ""


def getGeoInfo(ipstr, __cache__={}):
    global G_GeoHandler
    if G_GeoHandler is None:
        Init()
    try:
        ipstr = int_to_ip(ip_to_int(ipstr))

        geodt = __cache__.get(ipstr, {})
        if geodt:
            return geodt

        response = G_GeoHandler.city(ipstr)
        geodt["country"]   = response.country.name
        geodt["city"]      = response.city.name
        geodt["latitude"]  = response.location.latitude
        geodt["longitude"] = response.location.longitude

        __cache__[ipstr] = geodt

        return geodt
    except Exception, e:
        pass
    return {}


