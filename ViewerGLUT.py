#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from OpenGL import GLUT as glut
from UniversalViewer import *
from SceneGLUT import *
KeycodeToKeyname ={
        glut.GLUT_KEY_F1:"F1", glut.GLUT_KEY_F2:"F2", glut.GLUT_KEY_F3:"F3",
        glut.GLUT_KEY_F4:"F4", glut.GLUT_KEY_F5:"F5", glut.GLUT_KEY_F6:"F6", glut.GLUT_KEY_F7:"F7",
        glut.GLUT_KEY_F8:"F8", glut.GLUT_KEY_F9:"F9", glut.GLUT_KEY_F10:"F10", glut.GLUT_KEY_F11:"F11",
        glut.GLUT_KEY_F12:"F12", glut.GLUT_KEY_LEFT:"LEFT", glut.GLUT_KEY_UP:"UP", glut.GLUT_KEY_RIGHT:"RIGHT",
        glut.GLUT_KEY_DOWN:"DOWN", glut.GLUT_KEY_PAGE_UP:"PAGE_UP", glut.GLUT_KEY_PAGE_DOWN:"PAGE_DOWN",
        glut.GLUT_KEY_HOME:"HOME", glut.GLUT_KEY_END:"END", glut.GLUT_KEY_INSERT:"INSERT"
        }
class FrameGLUT:
    def __init__(self):
        glut.glutInitContextVersion(3,3)
        glut.glutInitContextProfile(glut.GLUT_CORE_PROFILE)
        glut.glutInit([])
        glut.glutInitWindowSize(900, 900)
        self.WINID=glut.glutCreateWindow("RunDemo")
        glut.glutSetWindow(self.WINID)
        self.layout = []
    def add(self, wdg):
        self.layout.append(wdg)
    def SetTitle(self, string):
        glut.glutSetWindowTitle(string)

class ViewerGLUT(UniversalViewer):
    def __init__(self, reader, rargv):
        UniversalViewer.__init__(self, reader)
        self.OnInit()
        self.V = Scene3DGLUT(self.Window)#, glarglist)
        UniversalViewer.InitGL(self)
        self.Window.add(self.V)
        self.argv = rargv
    def OnInit(self):
        Window = FrameGLUT()
        self.Window = Window
    def add_pal(self, name, pal_list):
        '''Добавляет палитру с именем name и цветами заданными в виде списка float со значениями от 0 до 1,
        они групируются по 3 формируя цвета, длина округляется до ближайшей снизу кратной 3'''
        self.V.MakeCurrent()
        UniversalViewer.add_pal(self, name, pal_list)
    #def WakeUp(self):
    #    wx.WakeUpIdle()
    #def OnPaint(self, event):
    #    self.Draw()
    def Draw(self):
        self.V.MakeCurrent()
        self.display()
        #self.cbox.SetCurrent(self.cbox.context)
        #print("DRAW")
    def Bind(self):
        # special functions
        self.KeyDownHandler = self.get_key_function(self.KeyDown)
        self.KeyUpHandler = self.get_key_function(self.KeyUp)
        self.SpecialKeyDownHandler = self.get_key_function(self.SpecialKeyDown)
        self.SpecialKeyUpHandler = self.get_key_function(self.SpecialKeyUp)
        # mouse bindings
        glut.glutMotionFunc(self.OnMoution)
        glut.glutMouseFunc(self.OnMouseButton)
        #Keyboard bindings
        glut.glutKeyboardFunc(self.OnKeyDown)
        glut.glutKeyboardUpFunc(self.OnKeyUp)
        glut.glutSpecialFunc(self.OnSpecialKeyDown)
        glut.glutSpecialUpFunc(self.OnSpecialKeyUp)
        # window bindings
        glut.glutReshapeFunc(self.V.reshape)
        glut.glutIdleFunc(self.idle)
        glut.glutDisplayFunc(self.Draw)
        glut.glutCloseFunc(self.exit)
    def exit(self):
        "Закрывает окно и завершает програму"
        self.reader_pipe.send(("exit", None))
        glut.glutLeaveMainLoop()
    #def OnTimer(self, evt):
    #    self.WakeUp()
    def SetWindowTitle(self, string):
        self.Window.SetTitle(string)
    def reshape(self, width , height):
        self.V.MakeCurrent()
        self.V.reshape(width, height)
        #event.Skip()
    #def OnIdle(self, event):
    #    self.idle()
    #    self.V.SetFocus()
    def OnSpecialKey(self,k, x, y):
        if k in KeycodeToKeyname:
            k= KeycodeToKeyname[k]
        else: k = "None"
        state = glut.glutGetModifiers()
        mod =  {"Shift":bool(state & 0x1), "Ctrl":bool(state & 0x2), "Alt":bool(state & 0x4)}
        return k,x,y,mod
    def OnKey(self,k, x, y):
        k = k.lower()
        state = glut.glutGetModifiers()
        mod =  {"Shift":bool(state & 0x1), "Ctrl":bool(state & 0x2), "Alt":bool(state & 0x4)}
        return k,x,y,mod
    def OnSpecialKeyDown(self,k,x,y):
        k,x,y,mod = self.OnSpecialKey(k,x,y)
        #print(k,mod,spec,"DOWN")
        self.SpecialKeyDownHandler(k,x,y,mod)
    def OnSpecialKeyUp(self,k,x,y):
        k,x,y,mod = self.OnSpecialKey(k,x,y)
        #print(k,mod,spec,"DOWN")
        self.SpecialKeyUpHandler(k,x,y,mod)
    def OnKeyDown(self,k,x,y):
        k,x,y,mod = self.OnKey(k,x,y)
        self.KeyDownHandler(k,x,y,mod)
    def OnKeyUp(self,k,x,y):
        k,x,y,mod = self.OnKey(k,x,y)
        self.KeyUpHandler(k,x,y,mod)
        #evt.Skip()
    #def CharHook(self,evt):
    #    evt.DoAllowNextEvent()
    #    print(evt.GetUnicodeKey(), evt.IsNextEventAllowed())
    #    evt.Skip()
    def OnMouseButton(self, but, ud, x,y):
        "Функция для мышки, служебная функция"
        if but == glut.GLUT_LEFT_BUTTON:
            if ud == glut.GLUT_DOWN:
                self.V.mouse_left_click(x,y)
                self.bb_auto=False
            else:
                self.V.mouse_left_release(x,y)
        elif but == glut.GLUT_RIGHT_BUTTON:
            if ud ==glut.GLUT_DOWN:
                self.V.mouse_right_click(x,y)
            else:
                self.V.mouse_right_release(x,y)
        elif but == 3 and ud == glut.GLUT_DOWN:
            self.bb_auto = False
            self.V.mouse_wheel_up()
            self.V.update()
        elif but==4 and ud == glut.GLUT_DOWN:
            self.bb_auto = False
            self.V.mouse_wheel_down()
            self.V.update()
    def OnMoution(self,x, y):
        self.V.drag(x,y)
        self.V.update()
    def MainLoop(self):
        self.InitGL()
        glut.glutMainLoop()

