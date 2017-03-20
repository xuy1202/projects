#coding: utf-8


import pbutils
from test import callapp_pb2


def _format_obj():
    obj = callapp_pb2.AllClass()
    obj.cls1.name = u"我爱希望的田野上"
    obj.cls1.age  = 111
    obj.cls1.name_list.extend(['a', 'b', 'c'])
    obj.cls1.type_list.append(callapp_pb2.Class1.kUnknown)
    obj.cls1.type_list.append(callapp_pb2.Class1.kType1)
    obj.cls1.type_list.append(callapp_pb2.Class1.kType2)
    c2 = obj.cls2.add()
    c2.name = "cls2.name1"
    c2.age_list.extend([11, 111])
    c2 = obj.cls2.add()
    c2.name = "cls2.name2"
    c2.age_list.extend([22, 222])
    return obj


def pb2json():
    obj = _format_obj()
    print ">>> Original print: "
    print obj
    print ">>> Optimized print: "
    pbutils.utf8_print(obj)
    ret = pbutils.to_json(obj)
    print ">>> json: "
    print type(ret), ret


def pb2dict():
    obj = _format_obj()
    ret = pbutils.to_dict(obj)
    print ">>> dict: "
    print type(ret), ret


def dict2pb():
    d = {'cls1': {'type_list': [0, 1, 2], 'age': 111, 'name': u'\u6211\u7231\u5e0c\u671b\u7684\u7530\u91ce\u4e0a', 'name_list': ['a', 'b', 'c']}, 'cls2': [{'age_list': [11, 111], 'name': 'cls2.name1'}, {'age_list': [22, 222], 'name': 'cls2.name2'}]}
    o = callapp_pb2.AllClass()
    print ">>> dict2pb before:"
    print o
    pbutils.format_from_dict(d, o)
    print ">>> dict2pb after:"
    print o


def json2pb():
    pass


if __name__ == "__main__":
    pb2json()
    pb2dict()
    dict2pb()


