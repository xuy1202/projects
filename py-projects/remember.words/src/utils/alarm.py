# coding: utf-8

import os
import sys
import time
import traceback
import urllib2
import urllib


AlarmServer  = "http://alarms.ops.qihoo.net:8360/intfs/alarm_intf"
AlarmGroup   = "AlarmDefaultGroup"


def send_alarm(group, content):
    try:
        hostname = os.popen("hostname").read().strip()
        alarm = '%s?group_name=%s&subject=%s:%s'%(AlarmServer, group, hostname, urllib.quote(content))
        req = urllib2.Request(alarm)
        urllib2.urlopen(req).read()
    except Exception, e:
        print traceback.format_exc()


if __name__ == '__main__':
    hostname = os.popen("hostname").read().strip()
    print hostname
    send_alarm("nlps_dev_monitor", str(time.time()))


