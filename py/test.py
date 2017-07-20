#coding: utf-8


class F(object):
    def __new__(cls, *a, **ka):
        print 'in F.__new__'
        return object.__new__(cls, *a, **ka)

    def __init__(self):
        self.f = 'f'
        pass

    def getf(self):
        return self.f


class S(F):
    def __new__(cls, *a, **ka):
        print 'in S.__new__'
        return object.__new__(cls, *a, **ka)

    def __init__(self):
        super(S, self).__init__()
        pass

s = S()
print s
print s.getf()

