#conding: utf-8

import re
from datetime import datetime
import time


_datetime2timestamp = lambda d: int(time.mktime(datetime.strptime(d, "%Y-%m-%d %H:%M:%S").timetuple()))
_ipv42int           = lambda d: (int(d[0]) << 24) + (int(d[1]) << 16) + (int(d[2]) <<  8) + (int(d[3]) <<  0)
_2int               = lambda d: int(d)
_2str               = lambda d: str(d)
_domain             = lambda d: _2str(d) if d.find("re:") != 0 else re.compile(d[3:])


_keymap = {
    'tnow': _datetime2timestamp,
    'tbeg': _datetime2timestamp,
    'tend': _datetime2timestamp,

    'type': _2int,

    'spike': _2int,
    'count': _2int,

    'domain': _domain,
    'domain_forward': _domain,
}


def _stupid_trans(data):
    try:
        return _datetime2timestamp(data)
    except Exception, e:
        pass
    try:
        item = data.split(".")
        if len(item) != 4:
            raise RuntimeError("not ipv4-format")
        return _ipv42int(item)
    except Exception, e:
        pass
    try:
        return _2int(data)
    except Exception, e:
        pass
    return _2str(data)


def stupid_trans(data, key=None):
    if key is None:
        return _stupid_trans(data)
    kfun = _keymap.get(key, None)
    if kfun is None:
        return _stupid_trans(data)
    try:
        return kfun(data)
    except:
        return _stupid_trans(data)




