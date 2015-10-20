#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# we assume that all libs imported in advance, since they all building in one file
# in addition we assume that all neded objects are in defaul namespace
from OpenGL.GLUT import *
import os.path
from PIL import Image
from OpenGL import GL
from aivlib.vctf3 import *
#from collections import MutableMapping
import time
from async_input import *
from viewer import *

KeysList = " `1234567890qwertyuiop[]asdfghjkl;'xcvbnm,,./~!@#$%^&*()_-+QWERTYUIOP{}ASDFGHJKL:\"|\\ZXCVBNM<>?"
SpecialKeysList = [GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3,
        GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7,
        GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
        GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT,
        GLUT_KEY_DOWN, GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN,
        GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT]
#def get_arglist(func):
#    return func.func_code.co_varnames[:func.func_code.co_argcount]
def get_arglist(func, ommit_self = True):
    arglist = func.func_code.co_varnames[:func.func_code.co_argcount]
    if ommit_self and arglist and arglist[0]=="self": return arglist[1:]
    else: return arglist
#def func_run(func, *args, **kwargs):
#    arglist = get_arglist(func)
#    return func(*[kwargs[t] for t in args+arglist ])
def func2string(func):
    arglist = get_arglist(func)
    #print arglist, func.func_name + '('+(" {},"*(len(arglist)-1) + ' {} ')+')' if len(arglist) >=1 else '()'
    argum =(" {},"*(len(arglist)-1) + ' {} ') if arglist else ""
    return func.func_name + '('+argum.format(*arglist)+')'
#(self, func, key,   modificators = [], key_pressed = [],up = False)
DefaultKeyMapping = [ ("next()", " "), ("jump(-1)"," ", ["Shift"]),
        ("keyhelp()","h",["Shift"]), ("exit()", "q",["Shift"]),("keyhelp()","h"), ("exit()", "q"),
        ("rangemove(0.1,True)","+",["Ctrl"]), ("rangemove(0.1,True)","+",["Ctrl","Shift"]),
        ("extendrange(0.2)","-",["Ctrl"]), ("extendrange(0.2)","-",["Ctrl","Shift"]),
        ("rangemove(1/1.1-1.0,True)","{",["Shift"]), ("rangemove(1/1.1-1.0,True)","{"),
        ("rangemove(1/1.1-1.0,False)","}",["Shift"]), ("rangemove(1/1.1-1.0,False)","}"),
        ("rangemove(0.1,True)","[",["Shift"]), ("rangemove(0.1,True)","["),
        ("rangemove(0.1,False)","]",["Shift"]), ("rangemove(0.1,False)","]"),
        ("autoscalecb()","a"), ("autoscalecb()","a",["Shift"]),
        ("autoscale()","A"), ("autoscale()","A",["Shift"]),
        ("saveimage(get_image_name())","s"), ("saveimage(get_image_name())","s",[ "Shift" ]),
        ("toggle_wire()","w"), ("toggle_wire()","w",["Shift"]),
        ("ax^=True","c"), ("ax^=True","c",["Shift"]),
        ("set_item((get_item()+1)%get_appends_len())",">",["Shift"]),
        ("set_item((get_item()+get_appends_len() -1)%get_appends_len())","<",["Shift"]),
        ("rotate(-10,0)", GLUT_KEY_LEFT), ("rotate(10,0)", GLUT_KEY_RIGHT),
        ("rotate(0,-10)", GLUT_KEY_UP), ("rotate(0,10)", GLUT_KEY_DOWN),
        ("zoom(10)", GLUT_KEY_UP,["Ctrl"]), ("zoom(-10)", GLUT_KEY_DOWN, ["Ctrl"]),
        ("move(0,-10)", GLUT_KEY_UP,["Shift"]), ("move(0,10)", GLUT_KEY_DOWN, ["Shift"]),
        ("move(10,0)", GLUT_KEY_RIGHT,["Shift"]), ("move(-10,0)", GLUT_KEY_LEFT, ["Shift"]),
        ("clip_plane_move(0.01,0)", GLUT_KEY_RIGHT,[],["1"]), ("clip_plane_move(-0.01,0)", GLUT_KEY_LEFT, [],["1"]),
        ("clip_plane_move(0.01,1)", GLUT_KEY_RIGHT,[],["2"]), ("clip_plane_move(-0.01,1)", GLUT_KEY_LEFT, [],["2"]),
        ("clip_plane_move(0.01,2)", GLUT_KEY_RIGHT,[],["3"]), ("clip_plane_move(-0.01,2)", GLUT_KEY_LEFT, [],["3"]),
        ("clip_plane_move(0.01,3)", GLUT_KEY_RIGHT,[],["4"]), ("clip_plane_move(-0.01,3)", GLUT_KEY_LEFT, [],["4"]),
        ("clip_plane_move(0.01,4)", GLUT_KEY_RIGHT,[],["5"]), ("clip_plane_move(-0.01,4)", GLUT_KEY_LEFT, [],["5"]),
        ("clip_plane_move(0.01,5)", GLUT_KEY_RIGHT,[],["6"]), ("clip_plane_move(-0.01,5)", GLUT_KEY_LEFT, [],["6"]) ]
# add cutting surfaces


class UniversalViewer:#(MutableMap):
    def __init__(self, argv):
        self.V = Viewer()
        glutInit(argv)
        glutInitWindowSize(self.V.get_width(), self.V.get_height())
        glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH)
        glutCreateWindow("viewer")
        self.V.GL_init()
        self.spr = ShaderProg()
        self.palettes = {} # names for palettes?
        self.add_pal("pal", [1.,0.,0., 1.,.5,0., 1.,1.,0., 0.,1.,0., 0.,1.,1., 0.,0.,1., 1.,0.,1.])
        self.add_pal("rgb", [1.,0.,0.,0.,1.,0.,0.,0.,1.])
        # make it simple to create pal!!
        self.Axis = Axis()
        self.Axis.load_on_device()
        d ={}
        for k in SpecialKeysList:
            #print k
            d[k] = []
        #print d
        self.SpecialKeyUp = dict([ (k, []) for k in SpecialKeysList])
        self.SpecialKeyDown = dict([ (k, []) for k in SpecialKeysList])
        self.KeyUp = dict([(k, []) for k in KeysList])
        self.KeyDown = dict([(k, []) for k in KeysList])
        self.keystates = dict([(k, False) for k in KeysList])
        #self.params = {}
        self.ax = True
        #self.cb_auto = True
        self.bb_auto = True
        self.com_thr = async_getline()
        self.idle_actions = []
        self.title_template = ("UniversalViewer scale:{}  cb {}:{}", ("get_scale()", "get_cbrange()[0]", "get_cbrange()[1]" ))
        self.image_name_template = ("SC{}-V{}_{}_{}.png",("get_frame()","get_scale()","get_view()[0]","get_view()[1]", "get_view()[2]"))
        self.set_pal("pal")
#--------------------------------------------------------------
    def __setattr__(self,name,value):
        self.__dict__[name] = value
    def __setitem__(self, key, value):
        self.__dict__[key] = value
    def __getitem__(self, key):
        try:
            return getattr(self, key)
        except AttributeError:
            raise KeyError(key)

    #def __getitem__(self, key): # getitem would evaluate it immideately, yes recursion is possible
    #    # is not usable for mouse methods
    #    arglist = get_arglist(self.params[key])
    #    return self.params[key](*[self[t] for t in arglist ])
    #def __setitem__(self, key, value):
    #    self.params[key] = value
    #def __iter__(self):
    #    return iter(self.params)
    #def __len__(self):
    #    return len(self.params)
    def execute(self, string, **kwargs):
        #kwargs["__builtins__"]=__builtins__
        kwargs.update(globals())
        if isinstance(string, str): exec(string,kwargs,self)
        else: exec(func2string(string), kwargs, self)
        glutPostRedisplay()
    def evaluate(self, string, **kwargs):
        #kwargs["__builtins__"]=__builtins__
        kwargs.update(globals())
        if isinstance(string, str): return eval(string,kwargs,self)
        else: return eval(func2string(string), kwargs, self)
    def set_key(self, func, key, modificators = [], key_pressed = [], up = False):
        if(( key not in list(KeysList))  and (key not in SpecialKeysList)): print "Unhandled key {}".format(key); return
        KeysDict = (self.KeyDown, self.KeyUp, self.SpecialKeyDown, self.SpecialKeyUp)[ int(key in SpecialKeysList) *2 + int(up)]
        KeysDict[key].append((func, modificators, key_pressed))
#-------------------------------------------------------------
    def states(self):
        state = glutGetModifiers()
        return {"Shift":bool(state & 0x1), "Ctrl":bool(state & 0x2), "Alt":bool(state & 0x4)}
    def add_pal(self, name, pal_list):
        truncate3 = lambda x: x - x%3
        nlen = truncate3(len(pal_list))
        pal = float_array(nlen)
        for i, v in enumerate(pal_list[:nlen]): pal[i] = v
        self.palettes[name] = Texture(pal, nlen/3)
    def eval_template(self, template, args):
        return template.format(*map(self.evaluate , args))
    def get_image_name(self):
        return self.eval_template(self.image_name_template[0], self.image_name_template[1])
    def get_title(self):
        return self.eval_template(self.title_template[0], self.title_template[1])
    def get_key_function(self, KeyDict):
        def KeyHandler(k, x, y):
            #print k
            if KeyDict == self.KeyDown :
                self.keystates[k] = True
            elif KeyDict == self.KeyUp:
                self.keystates[k] = False
                self.keystates[k.upper()] = False
            if k not in KeyDict or not KeyDict[k]: return
            mod = self.states()
            for func, modifiers, key_pressed in KeyDict[k]:
                execute = True
                #print func, k
                for m in ["Ctrl", "Shift", "Alt"]:
                    execute &= (mod[m] == (m in modifiers))
                    #print mod[m] == (m in modifiers)
                    if not execute: break
                #print execute, modifiers, mod
                for kp in KeysList:
                    if (kp == k): continue
                    execute &= (self.keystates[kp] == (kp in key_pressed))
                    if not execute: break
                #print execute, key_pressed
                if execute:
                    #func_run(func, x=x, y = y, **self.params)
                    self.execute(func, x=x,y=y)
                    break
            glutPostRedisplay()
        return KeyHandler
    def help(self, myobject=__module__):
        "Показать справку на обьект"
        help(myobject)
    def set_pal(self, pal_name):
        self.tex = self.palettes[pal_name]
        self.cur_pal = pal_name
    def set_view(self,pitch, yaw, roll):
        self.V.set_view(pitch,yaw,roll)
    def set_pos(self, x, y, z):
        self.V.set_pos(x,y,z)
    def set_scale(self, sc):
        self.V.set_scale(sc)
    def set_xrange__(self, lower, upper):
        #self.bb_auto = False
        self.V.set_bounding_box(0, lower, False), self.V.set_bounding_box(0, upper, True)
    def set_yrange__(self, lower, upper):
        #self.bb_auto = False
        self.V.set_bounding_box(1, lower, False), self.V.set_bounding_box(1, upper, True)
    def set_zrange__(self, lower, upper):
        #self.bb_auto = False
        self.V.set_bounding_box(2, lower, False), self.V.set_bounding_box(2, upper, True)
    def set_xrange(self, lower, upper):
        self.bb_auto = False
        self.set_xrange__(lower,upper)
    def set_yrange(self, lower, upper):
        self.bb_auto = False
        self.set_yrange__(lower,upper)
    def set_zrange(self, lower, upper):
        self.bb_auto = False
        self.set_zrange__(lower,upper)
    def get_scale(self):
        return self.V.get_scale()
    def get_width(self):
        return self.V.get_width()
    def get_height(self):
        return self.V.get_height()
    def get_pal(self):
        return self.cur_pal
    def get_xrange(self):
        return self.V.get_bounding_box(0, False), self.V.get_bounding_box(0, True)
    def get_yrange(self):
        return self.V.get_bounding_box(1, False), self.V.get_bounding_box(1, True)
    def get_zrange(self):
        return self.V.get_bounding_box(2, False), self.V.get_bounding_box(2, True)
    def get_pos(self):
        p = float_array(3)
        self.V.get_pos(p)
        return (p[0], p[1], p[2])
    def get_view(self):
        p,y,r = float_array(1),float_array(1),float_array(1)
        self.V.get_view(p,y,r)
        return (p[0],y[0],r[0])
    def toggle_wire(self):
        self.V.togglewire()
    def set_wire(self, b_tf):
        self.V.set_wire(b_tf)
    def get_wire(self):
        return self.V.get_wire()
    def toggle_axis(self):
        self.ax^=1
    def rotate(self, x, y):
        x_min = 0 if x>=0 else -x
        y_min = 0 if y>=0 else -y
        self.V.mouse_click(GLUT_RIGHT_BUTTON, GLUT_DOWN, x_min,y_min )
        self.V.drag(x+x_min,y)
        self.V.mouse_click(GLUT_RIGHT_BUTTON, GLUT_UP, x+x_min,y_min)
    def move(self, x, y):
        self.bb_auto=False
        x_min = 0 if x>=0 else -x
        y_min = 0 if y>=0 else -y
        self.V.mouse_click(GLUT_LEFT_BUTTON,GLUT_DOWN,x_min,y_min)
        self.V.drag(x,y)
        self.V.mouse_click(GLUT_LEFT_BUTTON,GLUT_UP,x,y)
    def zoom(self, x):
        self.bb_auto=False
        if x<0: self.V.mouse_click(4,GLUT_DOWN,0,0)
        else: self.V.mouse_click(3,GLUT_DOWN,0,0)
    def idle(self):
        if (not self.com_thr.is_alive()):
            command = self.com_thr.result_queue.get()
            try:
                self.execute(command)
            except (NameError, SyntaxError):
                print "command %s not found"%command
            except:
                print "Unexpected error:", sys.exc_info()[0]
                exit()
            self.com_thr = async_getline()
        cur_time = time.time()
        for i, (name, last_time, interval, action) in enumerate(self.idle_actions):
            if cur_time-last_time>=interval:
                self.execute(action)
                self.idle_actions[i][1] = time.time()
    def add_idle_action(self, name, interval,action):
        for i,act in enumerate(self.idle_actions):
            if name ==act[0]:
                self.idle_actions[i]=[name,time.time(), interval,action]
                break
        else:
            self.idle_actions.append([name,time.time(), interval,action])
    def del_idle_action(self,name):
        for i, (aname, lt, interv, action) in enumerate(self.idle_actions):
            if name == aname:
                del self.idle_actions[i]
    def clear_idle_actions(self):
        del self.idle_actions
        self.idle_actions = []
    def plot(self,surf):
       self.Surf = surf
       self.Surf.load_on_device()
    #def subplot(*args);
    #    self.Surfaces += args
    def dump_params(self, **kwargs):
        pars = {}
        for name,set_func in kwargs.items():
            if name == set_func:
                pars[name] = (self[name],set_func)
            else:
                result = self[name]()
                if not isinstance(result,list) and not isinstance(result, tuple):
                    result = (result,)
                pars[name] = (result, set_func)
        return pars
    def load_params(self, **params):
        for name, val_func in params.items():
            #print name, val_func
            if name == val_func[1]:
                self[name] = val_func[0]
            else: self[val_func[1]](*val_func[0])
    def del_idle_act(self,name):
        for i,act in enumerate(self.idle_actions):
            if name ==act[0]:
                del act
                break
    def clip_plane_move(self, val,pl):
        self.bb_auto = False
        self.V.clip_plane_move(val,pl)
    def autoscale(self):
        bb_min, bb_max = self.Surf.autobox()
        self.set_xrange(bb_min[0], bb_max[0])
        self.set_yrange(bb_min[1], bb_max[1])
        self.set_zrange(bb_min[2], bb_max[2])
        self.bb_auto = True
        self.V.automove()
    def autoscalecb(self):
        #self.cb_auto = True
        self.Surf.auto_cbrange()
    def display(self):
        if self.bb_auto:
            self.autoscale()
        self.V.display()
        self.Surf.display(self.V, self.spr, self.tex)
        #com = self.V.get_command()
        #com = sys.stdin.readline()
        #if (com):
        #    print "inpu command %s"%com
        if self.ax:
            self.V.axis_switch()
            self.spr.render(self.Axis, self.V, self.palettes["rgb"])
            self.V.axis_switch()
        glutSetWindowTitle(self.get_title())#self.execute(self.title_template))
        glutSwapBuffers()
    def exit(self):
        exit(0)
    def keyhelp(self):
#тут не хватает нормального форматирования для комбинаций клавиш
        for kd in [self.KeyDown, self.SpecialKeyDown, self.KeyUp, self.SpecialKeyUp]:
            for key, flist in kd.items():
                for func, modifiers, key_pressed in flist:
                    print key, '	',func, modifiers, key_pressed
    def shader_load(self, vertex_string, fragment_string):
        self.spr.extern_load(vertex_string, fragment_string)
    def saveimage(self,name):
        #from OpenGL import GL
        x,y = self.get_width(), self.get_height()
        GL.glReadBuffer(GL.GL_FRONT)
        buffer = ( GL.GLubyte * (3*x*y) )(0)
        glutPostRedisplay()
        GL.glReadPixels(0, 0, x-3, y-3, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, buffer)
        time.sleep(0.5)
        image = Image.frombytes(mode="RGB", size=(x-3, y-3), data=buffer)
        image = image.transpose(Image.FLIP_TOP_BOTTOM)
        print "saved to "+name
        image.save(name)
        image.close()
        del buffer, image
    def set_title(self,template):
        self.title_template = template
    def __call__(self): # start main loop
        glutSetWindowTitle(self.get_title())#self.exec(self.title_template.format(self.params)))
        glutDisplayFunc(self.display)
        glutMotionFunc(self.V.drag)
        glutMouseFunc(self.V.mouse_click)
        glutKeyboardFunc(self.get_key_function( self.KeyDown))
        glutKeyboardUpFunc(self.get_key_function( self.KeyUp))
        glutSpecialFunc(self.get_key_function( self.SpecialKeyDown))
        glutSpecialUpFunc(self.get_key_function( self.SpecialKeyUp))
        glutReshapeFunc(self.V.reshape)
        glutIdleFunc(self.idle)
        glutMainLoop()

