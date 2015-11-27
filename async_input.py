#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import threading
import Queue
import sys
import time
#import atexit
import readline
import rlcompleter
import signal
import os
class AlarmException(Exception):
    pass
def alarm_handler(signum, frame):
    signal.signal(signal.SIGALRM, signal.SIG_IGN)
    raise AlarmException
def pre_input_hook():
    signal.signal(signal.SIGALRM, alarm_handler)
def threaded(f, daemon=True):
# here we have a problem with thread stop
#solution queue + timeout
    def wrapped_f(q, *args, **kwargs):
        '''this function calls the decorated function and puts the
        result in a queue'''
        ret = f(*args, **kwargs)
        q.put(ret)

    def wrap(*args, **kwargs):
        '''this is the function returned from the decorator. It fires off
        wrapped_f in a new thread and returns the thread object with
        the result queue attached'''

        q = Queue.Queue()

        t = threading.Thread(target=wrapped_f, args=(q,)+args, kwargs=kwargs)
        t.daemon = daemon
        t.start()
        t.result_queue = q
        return t
    return wrap

class rl_async_reader:
    def __init__(self, history_path,prompt = ">"):
        self.history_path = history_path
        self.prompt = prompt
        readline.parse_and_bind('tab: complete')
        self.read_history()
        #atexit.register(self.write_history)
        readline.set_pre_input_hook(pre_input_hook)
        self.q = Queue.Queue()
        self.lock = threading.Lock()
    def set_completer(self, namespace):
        self.rlc = rlcompleter.Completer(namespace)
        readline.set_completer(self.rlc.complete)
    def read_history(self):
        if os.path.exists(self.history_path):
            readline.read_history_file(self.history_path)
    def write_history(self):
        readline.write_history_file(self.history_path)
    def get(self):
        try:
            return self.q.get_nowait()
        except (Queue.Empty):
            return ""
    #@threaded
    def __call__(self):
        #signal.signal(signal.SIGALRM, alarm_handler)
        #signal.alarm()
        try:
            while(1):
                self.lock.acquire()
                self.q.put(raw_input(self.prompt))
                self.lock.release()
                time.sleep(0.01)
                #Это пока костыль
        except (AlarmException, EOFError, KeyboardInterrupt):
            self.q.put("exit()")
            self.lock.release()
            self.write_history()
        #readline.insert_text(self.prompt)
        #return sys.stdin.readline()
