#!/usr/bin/env python2
import threading
import Queue
import sys
import time
import atexit
import readline
import rlcompleter
import os
def threaded(f, daemon=True):

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
        atexit.register(self.write_history)
    def set_completer(self, namespace):
        self.rlc = rlcompleter.Completer(namespace)
        readline.set_completer(self.rlc.complete)
    def read_history(self):
        if os.path.exists(self.history_path):
            readline.read_history_file(self.history_path)
    def write_history(self):
        readline.write_history_file(self.history_path)
    @threaded
    def async_getline(self):
        try:
            return raw_input(self.prompt)
        except EOFError:
            return EOFError
        #readline.insert_text(self.prompt)
        #return sys.stdin.readline()
