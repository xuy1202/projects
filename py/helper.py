#coding: utf-8


def reform_from_tcpdump(s):
    com_list = [l.split()[1:-1] for l in s.splitlines() if l.strip()]
    com_strs = ''.join([''.join(i) for i in com_list])[56:] # ip head 20 + udp head 8
    d = ''
    for i in range(0, len(com_strs), 2):
        d += chr(int(com_strs[i:i+2], 16))
    return d


