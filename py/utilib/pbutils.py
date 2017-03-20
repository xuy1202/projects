#coding: utf-8

import json
import os
import traceback

from google.protobuf import message
from google.protobuf.internal import containers
ProtobufMessage = message.Message
ProtobufRepeatedC = containers.RepeatedCompositeFieldContainer
ProtobufRepeatedS = containers.RepeatedScalarFieldContainer


__DEBUG__ = os.environ.get("__DEBUG__", False) 



def _do_basic_type(obj):
    if isinstance(obj, (str, unicode)):
        return obj
    if isinstance(obj, (int, long)):
        return obj
    if isinstance(obj, (float)):
        return obj
    if isinstance(obj, (bool)):
        return obj
    return repr(obj)


def utf8_print(obj, depth=0):
    suojin = ' '*depth*4
    for field, item in obj.ListFields():
        name = field.name.strip()
        if isinstance(item, (int, float, long, bool, str)):
            print '%s%s: %s'%(suojin, name, item)
        elif isinstance(item, (unicode, )):
            print '%s%s: %s'%(suojin, name, item.encode('utf-8'))
        elif hasattr(item, 'extend') and hasattr(item, 'sort'):
            if len(item)>0 and hasattr(item[0], "ListFields"):
                for sub in item:
                    print '%s%s: {'%(suojin, name)
                    utf8_print(sub, depth+1)
                    print '%s}'%suojin
            else:
                print '%s%s: %s'%(suojin, name, str(item))
        else:
            print '%s%s: {'%(suojin, name)
            utf8_print(item, depth+1)
            print '%s}'%suojin


def to_dict(obj):
    ret_dict = {}
    if hasattr(obj, "ListFields"):
        for d, o in obj.ListFields():
            ret_dict[d.name] = to_dict(o)
    else:
        if isinstance(obj, ProtobufRepeatedS) and len(obj) > 0:
            return [to_dict(i) for i in obj]
        if isinstance(obj, ProtobufRepeatedC) and len(obj) > 0:
            return [to_dict(i) for i in obj]
        return _do_basic_type(obj)
    return ret_dict


def to_json(obj, **ka):
    d = to_dict(obj)
    return json.dumps(d, **ka)


def format_from_json(json_str, obj):
    try:
        d = json.loads(json_str)
        if format_from_dict(d, obj):
            return True
        return False
    except Exception, e:
        if __DEBUG__:
            print traceback.format_exc()
        print("format_from_json error: %s"%str(e))
        return False


def format_from_dict(data_dict, obj):
    try:
        for k in data_dict:
            if not hasattr(obj, k):
                continue
            v = data_dict[k]
            if isinstance(v, dict):
                if not format_from_dict(v, getattr(obj, k)):
                    return False
                continue
            if isinstance(v, (list, tuple)):
                if len(v) == 0:
                    continue
                attr = getattr(obj, k)
                if hasattr(attr, 'add') and isinstance(v[0], dict):
                    for item in v:
                        so = attr.add()
                        if not format_from_dict(item, so):
                            return False
                    continue
                if hasattr(attr, 'append') and isinstance(v[0], (str, unicode, int, long, float, bool)):
                    for item in v:
                        attr.append(item)
                    continue
            # fucking codecs:
            #     we'd better control strict encoding here
            #     but we just try-retry for compatible
            if isinstance(k, unicode):
                k = k.encode('utf-8', 'ignore')
            try:
                setattr(obj, k, v)
            except:
                if isinstance(v, unicode):
                    v = v.encode('utf-8', 'ignore')
                setattr(obj, k, v)
        return True
    except Exception, e:
        if __DEBUG__:
            print traceback.format_exc()
        print("format_from_dict error: %s"%str(e))
        return False


def overlap_merge(lh, rh):
    if type(lh) != type(rh):
        return False
    lh_d = [(k.name, k, v) for k, v in lh.ListFields()]
    rh_d = [(k.name, k, v) for k, v in rh.ListFields()]
    for k, f, v in lh_d:
        if isinstance(v, ProtobufRepeated):
            old = getattr(rh, k)
            rh.ClearField(k)
            for i in v:
                o = getattr(rh, k).add()
                o.MergeFrom(i)
            continue
        if isinstance(v, ProtobufMessage):
            so_lh = getattr(lh, k)
            so_rh = getattr(rh, k)
            if not overlap_merge(so_lh, so_rh):
                return False
            continue
        setattr(rh, k, v)
    return True



if __name__ == "__main__":
    pass



