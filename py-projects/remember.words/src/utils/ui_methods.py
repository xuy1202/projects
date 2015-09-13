import urllib
import time
from datetime import datetime


def sec_to_microsec(sh,ts):
    return int(ts)*1000

def ts_to_timetuple(sh, ts):
    t = datetime.fromtimestamp(ts)
    return (t.year, t.month-1, t.day, t.hour, t.minute,t.second)

def ts_to_datetime(sh,ts):
    return datetime.fromtimestamp(ts)

def int_to_yes_or_no(sh, i):

    if i == 0:
        return "No"
    elif i == 1:
        return  "Yes"

def format_nf_graph_link(sh, ip, tbeg, tend):
    pattern = "http://node004v.add.lyct.qihoo.net/flow/ip/%s/start/%s/end/%s/"
    date_beg = str(datetime.fromtimestamp(int(tbeg)-86400))
    date_end = str(datetime.fromtimestamp(int(tend)+ 7200))
    return pattern%(
        ip, urllib.quote(date_beg), urllib.quote(date_end)
    )

def datetime_today(sh):
    t = int(time.time())
    d = datetime.fromtimestamp(t)
    return str(d)

def datetime_yesterday(sh):
    t = int(time.time()) - 86400
    d = datetime.fromtimestamp(t)
    return str(d)


def nfreal_id(sh, d):
    ipv4_num = d["ipv4_num"]
    proto    = d["prot"]
    port     = d["port"]
    return "_".join([ipv4_num, proto, port])


def nfreal_tag(sh, d):
    ipv4_str = d["ipv4_str"]
    proto    = d["prot"]
    port     = d["port"]
    if proto == "0":
        return "_".join([ipv4_str, ])
    if port == '0':
        return "_".join([ipv4_str, proto])
    return "_".join([ipv4_str, proto, port])


