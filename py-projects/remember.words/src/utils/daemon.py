#coding: utf-8

import os
import sys

UMASK = 0 
WORKINGDIR = '/' 
MAXFD = 1024
REDIRECT_TO = "/dev/null"

def daemonize(func, *a, **ka):
    ''' double-fork to create daemon process '''
    try:
        # fork here, then parent die:
        #     1, let the shell think we are dead
        #     2, children now can create new process session
        pid = os.fork()
    except OSError, e:
        raise Exception, "%s [%d]" % (e.strerror, e.errno)
    if (pid == 0):
        # first child
        os.setsid()
        try:
            # fork here, then parent die:
            #     now child is not session master
            #     so it will have no shell I/O/signal
            pid = os.fork()
        except OSError, e:
            raise Exception, "%s [%d]" % (e.strerror, e.errno)

        if (pid == 0): 
            # second child
            os.chdir(WORKINGDIR)
            os.umask(UMASK)
        else:
            # second father quit
            os._exit(0)
    else:
        # first father quit
        os._exit(0)

    #for handler in logging.root.handlers:
    #    if hasattr(handler, 'stream') and hasattr(handler.stream, 'fileno') and handler.stream.fileno() > 2:
    #        filenos.append( handler.stream.fileno() )
    #for fd in range(0, maxfd):
    #    try:
    #        if fd not in filenos:
    #            os.close(fd)
    #    except OSError:
    #        pass

    #redirect stdin, stdout, stderr to null
    #os.open(REDIRECT_TO, os.O_RDWR)
    #os.dup2(0, 1)
    #os.dup2(0, 2)
    null = open('/dev/null' ,'r+w+b')
    os.dup2(null.fileno(), sys.stdin.fileno())
    os.dup2(null.fileno(), sys.stdout.fileno())
    os.dup2(null.fileno(), sys.stderr.fileno())

    return func(*a, **ka)


