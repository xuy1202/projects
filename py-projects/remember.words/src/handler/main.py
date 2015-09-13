#coding: utf-8

import os, sys
import urllib

import tornado.web


GlobalIndex = 0
GlobalDatas = []


class Word(object):
    def __init__(self, w="", m="", s=""):
        global GlobalIndex
        GlobalIndex += 1
        self.index    = GlobalIndex
        self.word     = w
        self.meaning  = m
        self.sentence = s
        self.sentence_lh = ""
        self.sentence_rh = ""


def WordFormat(word_list):
    rets = []
    for word in word_list:
        try:
            word.sentence_lh, word.sentence_rh = word.sentence.lower().split(word.word.lower(), 1)
            word.sentence_lh = word.sentence_lh.capitalize()
            rets.append(word)
        except Exception, e:
            print e
    return rets


class MainHandler(tornado.web.RequestHandler):
    def get(self):
        args = urllib.unquote(self.request.path).strip("/").split("/")
        args = dict(zip(args[::2], args[1::2]))
        begs = int(args.get("begs", 0))
        lens = int(args.get("lens", 3))
        func = args.get("func", "spelling")
        word_list = GlobalDatas[begs:begs+lens]
        nextpage = "/func/%s/begs/%s/lens/%s/"%(func, begs+lens, lens)
        self.render("index.html"
                    , word_list=WordFormat(word_list)
                    , functype=func
                    , nextpage=nextpage)


