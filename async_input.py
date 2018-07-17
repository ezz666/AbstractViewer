#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import multiprocessing
import six
if six.PY2:
    import Queue as queue                     
else:
    import queue
import sys
import time
#import atexit
import readline
import rlcompleter
import signal
import os
import ast
import traceback
import pydoc
class KillException(Exception):
    pass
def async_process(f):
# here we have a problem with thread stop
#solution queue + timeout
    #def wrapped_f(q,l, *args, **kwargs):
    #    '''this function calls the decorated function and puts the
    #    result in a queue'''
    #    l.acquire()
    #    ret = f(*args, **kwargs)
    #    l.release()
    #    q.put(ret)

    def wrap(*args, **kwargs):
        '''this is the function returned from the decorator. It fires off
        wrapped_f in a new thread and returns the thread object with
        the result queue attached'''
        #queue = multiprocessing.Queue()
        t = multiprocessing.Process(target=f, args=args, kwargs=kwargs)
        t.start()
        return t
    return wrap

def is_statement(s):
    try:
        ast.parse(s)
        last_str = s.split("\n")[-1]
        if  last_str == "" or last_str[0] not in [" ", "	"]:
            return True
        else:
            return False
    except SyntaxError as e:
        if e.msg ==  "EOL while scanning string literal" and s[-1] == '\\': return False
        elif e.msg in ["unexpected EOF while parsing", "EOF while scanning triple-quoted string literal"]:
            return False
        else: raise e

class rl_async_reader:
    def __init__(self,history_path,prompt = ">"):
        self.history_path = history_path
        self.prompt = prompt
        self.pid = os.getpid()
        readline.parse_and_bind('tab: complete')
        self.lock = multiprocessing.Lock()
        self.namespace = {}
        self.KillException = KillException
        self.update_completer(self.namespace)
    def update_completer(self, namespace):
        self.namespace.update(namespace)
        self.rlc = rlcompleter.Completer(self.namespace)
        readline.set_completer(self.rlc.complete)
    def read_history(self):
        if os.path.exists(self.history_path):
            readline.read_history_file(self.history_path)
    def write_history(self):
        readline.write_history_file(self.history_path)
    def kill_handler(self,signum, frame):
        signal.signal(signal.SIGHUP, signal.SIG_IGN)
        raise self.KillException
    def alrm_handler(self, signum, frame):
        while(self.pipe.poll()):
            self.process_message(*self.pipe.recv())
    def display_help(self, helpstring):
        pydoc.pager(helpstring)
    def pre_input_hook(self):
        signal.signal(signal.SIGHUP, self.kill_handler)
        signal.setitimer(signal.ITIMER_REAL, 0.1, 0.3)
        signal.signal(signal.SIGALRM, self.alrm_handler)
    #def execute(self, command):
    #    self.queue.put(command)
    #def message_to_reader(self, command):
    #    self.reverse_queue.put(command)
    def process_message(self, command, args):
        if command == "update_completion":
            self.update_completer(args)
        elif command == "help":
            self.display_help(args)
        elif command == "exit":
            exit()
        else:
            pass
    #def get(self):
    #    try:
    #        return self.queue.get_nowait()
    #    except (queue.Empty):
    #        return ""
    #@threaded
    #@async_process
    def __call__(self, guifunc):
        self.pipe, child_conn = multiprocessing.Pipe(True)
        myguifunc = async_process(guifunc)
        t = myguifunc(child_conn)
        self.update_completer(self.pipe.recv()[1])
        #print("Async input initialized")
        import sys,os,time,signal
        #sys.stdin = os.fdopen(self.stdin_fid)
        import readline
        readline.set_completer(self.rlc.complete)
        self.read_history()
        try:
            self.lock.acquire()
            readline.set_pre_input_hook(self.pre_input_hook)
            while(1):
                try:
                    if six.PY2:
                        s = raw_input(self.prompt)
                        while(not is_statement(s)):
                            self.prompt="."
                            s = s + "\n" + raw_input(self.prompt)
                        self.prompt=">"
                        self.pipe.send(s)
                    else:
                        s = input(self.prompt)
                        while(not is_statement(s)):
                            self.prompt="."
                            s = s + "\n" + input(self.prompt)
                        self.prompt=">"
                        self.pipe.send(s)
                except SyntaxError as e:
                    traceback.print_exc()
                except Exception as e:
                    raise e
        except (self.KillException, EOFError, KeyboardInterrupt):
            self.lock.release()
            self.pipe.send("exit()")
            self.write_history()
