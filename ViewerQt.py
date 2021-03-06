# -*- coding: utf-8 -*-
from PyQt4 import QtGui, QtCore, QtOpenGL, Qt
from UniversalViewer import *
from SceneQt import *

KeycodeToKeyname ={
        QtCore.Qt.Key_F1:"F1", QtCore.Qt.Key_F2:"F2", QtCore.Qt.Key_F3:"F3",
        QtCore.Qt.Key_F4:"F4", QtCore.Qt.Key_F5:"F5", QtCore.Qt.Key_F6:"F6", QtCore.Qt.Key_F7:"F7",
        QtCore.Qt.Key_F8:"F8", QtCore.Qt.Key_F9:"F9", QtCore.Qt.Key_F10:"F10", QtCore.Qt.Key_F11:"F11",
        QtCore.Qt.Key_F12:"F12", QtCore.Qt.Key_Left:"LEFT", QtCore.Qt.Key_Up:"UP", QtCore.Qt.Key_Right:"RIGHT",
        QtCore.Qt.Key_Down:"DOWN", QtCore.Qt.Key_PageUp:"PAGE_UP", QtCore.Qt.Key_PageDown:"PAGE_DOWN",
        QtCore.Qt.Key_Home:"HOME", QtCore.Qt.Key_End:"END", QtCore.Qt.Key_Insert:"INSERT"
        }
class FrameQt(QtGui.QWidget):
    def __init__(self, parent=None):
        super(FrameQt, self).__init__(parent)
        #wx.Frame.__init__(self,None, -1, "RunDemo: ",size = (900,900),
                        #name="run a sample")
        self.vbox = QtGui.QVBoxLayout()
        self.setLayout(self.vbox)
        #self.SetSizerAndFit(self.layout)
        self.show()
    def add(self, wdg, proportion= 1):
        self.vbox.addWidget(wdg, proportion)
        self.show()

class ViewerQt(UniversalViewer, QtGui.QApplication):
    def __init__(self, pipe, argv):
        QtGui.QApplication.__init__(self,argv)
        #super(ViewerQt, self).__init__(argv)
        UniversalViewer.__init__(self, pipe)
        #self.OnInit()
        #wx.App.__init__(self, redirect=False)
        #self.ExitOnFrameDelete=True
        self.argv= argv
        #print("ViewerQt init done")
    def InitGL(self):
        #frame.CreateStatusBar()
        frame = FrameQt()

        # set the frame to a good size for showing the two buttons
        #win.SetFocus()
        #self.window = win
        #frect = frame.GetRect()
        #glarglist = int_array(len(attrib_list))
        #for i, v in enumerate(attrib_list):
        #    glarglist[i] = v
        frame.setFocus()
        #self.cbox = Scene2DWX(frame)
        #self.cbox.GL_init()
        self.V = Scene3DQT(frame)#, glarglist)
        self.V.setFocus()
        self.frame = frame
        UniversalViewer.InitGL(self)
        frame.add(self.V, 300)
        frame.show()
        return True
    def add_pal(self, name, pal_list):
        '''Добавляет палитру с именем name и цветами заданными в виде списка float со значениями от 0 до 1,
        они групируются по 3 формируя цвета, длина округляется до ближайшей снизу кратной 3'''
        self.V.MakeCurrent()
        UniversalViewer.add_pal(self, name, pal_list)
    #def WakeUp(self):
    #    wx.WakeUpIdle()
    def OnPaint(self, event):
        #print("ViewerQt paint")
        self.Draw()
    def Draw(self):
        self.V.MakeCurrent()
        self.display()
        #self.cbox.SetCurrent(self.cbox.context)
        #print("DRAW")
    def Bind(self):
        #self.V.Bind(wx.EVT_PAINT, self.OnPaint)
        #print("Start bind")
        self.V.paintGL = lambda : self.OnPaint(None)
        #self.frame.Bind(wx.EVT_CLOSE, self.OnExitApp)
        #print("Bind paintGL")
        self.V.resizeGL = lambda w,h: self.V.reshape(w,h)
        #print("Bind resize")
        self.aboutToQuit.connect(self.OnExitApp)
        self._timer = QtCore.QTimer(self)
        self._timer.setInterval(0)
        self._timer.timeout.connect(self.OnIdle)
        self.V.mousePressEvent = self.mousePressEvent
        self.V.mouseReleaseEvent = self.mouseReleaseEvent
        self.V.mouseMoveEvent = self.mouseMoveEvent
        self.V.wheelEvent = self.wheelEvent
        #self.frame
        #self.V.Bind(wx.EVT_SIZE, self.OnSize)
        #self.V.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        #self.V.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
        #self.V.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)
        #self.V.Bind(wx.EVT_RIGHT_UP, self.OnRightUp)
        #self.V.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        #self.V.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        #self.V.Bind(wx.EVT_MOUSEWHEEL, self.OnWheelRotate)
        self.KeyDownHandler = self.get_key_function(self.KeyDown)
        self.KeyUpHandler = self.get_key_function(self.KeyUp)
        self.SpecialKeyDownHandler = self.get_key_function(self.SpecialKeyDown)
        self.SpecialKeyUpHandler = self.get_key_function(self.SpecialKeyUp)
        self.V.keyPressEvent = self.OnKeyDown
        self.V.keyReleaseEvent = self.OnKeyUp
        ##self.frame.Bind(wx.EVT_CHAR_HOOK, self.CharHook)
        ##self.V.Bind(wx.EVT_CHAR, self.OnKeyDown)
        #self.V.Bind(wx.EVT_KEY_DOWN,self.OnKeyDown)
        ##print(self.SpecialKeyDown)
        #self.V.Bind(wx.EVT_KEY_UP,self.OnKeyUp)
        #self.V.Bind(wx.EVT_IDLE, self.OnIdle)
        #self.timer = wx.Timer(self)
        #self.V.Bind(wx.EVT_TIMER, self.OnTimer)
        #self.timer.Start(42)
        #print("ViewerQt bind")
    def exit(self):
        "Закрывает окно и завершает програму"
        if (self._closed == True): return
        self._closed = True
        #os.kill(self._t.pid,signal.SIGHUP)
        #self.rl_reader.lock.acquire()
        #self.rl_reader.lock.release()
        #self._t.join()
        #glutLeaveMainLoop()
        #self.frame.Show(True)
        #self.frame.SetFocus()
        #self.frame.Close(True)
        self.reader_pipe.send(("exit", None))
        exit()
        #self.ExitMainLoop()
    def OnTimer(self, evt):
        self.WakeUp()
    def SetWindowTitle(self, string):
        self.frame.setWindowTitle(string)
    def OnExitApp(self):
        self.exit()
    def OnSize(self, event):
        self.V.MakeCurrent()
        self.V.autoreshape()
        #event.Skip()
    def OnIdle(self):
        self.idle()
        self.V.setFocus()
    def OnKey(self,evt):
        k = evt.key()
        sp_key = k in KeycodeToKeyname 
        #print(k)
        if not sp_key:
            if six.PY3:
                k = evt.text().lower()
            else:
                k = str(evt.text()).lower()
        else:
            if k in KeycodeToKeyname:
                k= KeycodeToKeyname[k]
            else: k = "None"
        #x,y = evt.pos().x(), evt.pos().y()
        x,y =0,0
        mod = {"Ctrl":(evt.modifiers() & QtCore.Qt.ControlModifier) == QtCore.Qt.ControlModifier,
                "Shift":(evt.modifiers() & QtCore.Qt.ShiftModifier) == QtCore.Qt.ShiftModifier,
                "Alt":(evt.modifiers() & QtCore.Qt.AltModifier) == QtCore.Qt.AltModifier}
        return k,x,y,mod, sp_key
    def OnKeyDown(self,evt):
        k,x,y,mod,spec= self.OnKey(evt)
        #print(k,mod,spec,"DOWN")
        if spec: self.SpecialKeyDownHandler(k,x,y,mod)
        else: self.KeyDownHandler(k,x,y,mod)
        #evt.Skip()
    def OnKeyUp(self,evt):
        k,x,y,mod, spec = self.OnKey(evt)
        #print(k,mod,spec,"UP")
        if spec: self.SpecialKeyUpHandler(k,x,y,mod)
        else: self.KeyUpHandler(k,x,y,mod)
    #def CharHook(self,evt):
    #    evt.DoAllowNextEvent()
    #    print(evt.GetUnicodeKey(), evt.IsNextEventAllowed())
    #    evt.Skip()
    def OnMouseDown(self,evt):
        #self.V.CaptureMouse()
        pos = evt.pos()
        return pos.x(),pos.y()
    def mousePressEvent(self, evt):
        if evt.buttons() & QtCore.Qt.LeftButton:
            self.OnLeftDown(evt)
        if evt.buttons() & QtCore.Qt.RightButton:
            self.OnRightDown(evt)
    def mouseReleaseEvent(self, evt):
        if not (evt.buttons() & QtCore.Qt.LeftButton):
            self.OnLeftUp(evt)
        if not (evt.buttons() & QtCore.Qt.RightButton):
            self.OnRightUp(evt)
    def OnLeftDown(self, evt):
        self.V.mouse_left_click(*self.OnMouseDown(evt))
        self.bb_auto = False
    def OnLeftUp(self, evt):
        self.V.mouse_left_release(*self.OnMouseDown(evt))
        self.bb_auto = False
    def OnRightDown(self, evt):
        self.V.mouse_right_click(*self.OnMouseDown(evt))
    def OnRightUp(self, evt):
        self.V.mouse_right_release(*self.OnMouseDown(evt))
    def wheelEvent(self,evt):
        self.bb_auto = False
        rot = evt.delta()/120#evt.WheelRotation
        delta = 1.#evt.delta()
        #print("WHEEL", int(abs(rot)/delta))
        if rot>0:
            self.bb_auto = False
            for i in range(int(abs(rot)/delta)):
                self.V.mouse_wheel_up()
            self.V.update()
        elif rot<0:
            self.bb_auto = False
            for i in range(int(abs(rot)/delta)):
                self.V.mouse_wheel_down()
            self.V.update()
    def mouseMoveEvent(self, evt):
        #if evt.Dragging():
        self.V.drag(evt.x(), evt.y())
        self.V.update()
    def MainLoop(self):
        #print("Start Main Loop")
        self._timer.start()
        self.exec_()

class PaletteWidget(Scene2DQT):
    def __init__(self, parent=None):
        self.parent = parent
        Scene2DQT.__init__(self,parent)
        self.GL_init()
        self.cbox = PaletteBox()
        self.spr = Shader()
        path_to_AV = os.path.dirname(__file__)
        self.shader_extern_load(*map(lambda x : os.path.join(path_to_AV,x), ["2dv.shader","2df.shader"]) )
        checkOpenGLerror()
        self.palettes = {}
        checkOpenGLerror()
        #print("widget init")
        #self.add_pal("pal", [1.,0.,0., 1.,.5,0., 1.,1.,0., 0.,1.,0., 0.,1.,1., 0.,0.,1., 1.,0.,1.])
        #self.add_pal("rgb", [1.,0.,0.,0.,1.,0.,0.,0.,1.])
    def toggle(self):
        self.cbox.switch_vertical()
        self.plot()
    def shader_extern_load(self, vertex_string, fragment_string):
        "Загружает шейдеры из файлов"
        self.spr.extern_load(vertex_string, fragment_string)
    def shader_load(self, vertex_string, fragment_string):
        "Загружает шейдеры из строк"
        self.spr.load(vertex_string, fragment_string)
    def add_pal(self, name, pal_list):
        '''Добавляет палитру с именем name и цветами заданными в виде списка float со значениями от 0 до 1,
        они групируются по 3 формируя цвета, длина округляется до ближайшей снизу кратной 3'''
        checkOpenGLerror()
        #print("adding pal {}".format(name))
        truncate3 = lambda x: x - x%3
        nlen = truncate3(len(pal_list))
        pal = float_array(nlen)
        for i, v in enumerate(pal_list[:nlen]): pal[i] = v
        self.MakeCurrent()
        self.palettes[name] = Texture(pal, int(nlen/3))
    def set_pal(self, pal_name):
        "Устанавливает палитру"
        checkOpenGLerror()
        #print("setting pal {}".format(pal_name))
        self.MakeCurrent()
        self.tex = self.palettes[pal_name]
        self.cur_pal = pal_name
        self.cbox.set_texture( self.palettes[self.cur_pal] )
        self.cbox.load_on_device()
    def plot(self):
        #print("Cbox plot")
        self.MakeCurrent()
        #self.cbox._load_on_device = self.cbox.load_on_device
        #def myload():
        #    self.cbox._load_on_device()
        #    self.update()
        #self.cbox.load_on_device = myload
        self.cbox.load_on_device()
    def display(self):
        #print("Cbox display")
        self.V.display()
        self.spr.start()
        self.tex.use_texture(self.spr,"pal")
        self.V.plot(self.spr)
        #self.V.plot(self.spr)
        self.cbox.plot(self.spr)
        self.spr.stop()
        self.SwapBuffers()
    def Draw(self):
        checkOpenGLerror()
        #print("Cbox DRAW")
        self.MakeCurrent()
        self.automove()
        self.display()
    def OnSize(self,evt):
        self.MakeCurrent()
        self.autoreshape()
        self.automove()
        self.update()
    def OnPaint(self):
        #print("CBOX paint")
        self.MakeCurrent()
        self.autoreshape()
        self.Draw()
    def BindAll(self):
        #self.Bind(wx.EVT_PAINT, self.OnPaint)
        #self.Bind(wx.EVT_SIZE, self.OnSize)
        self.paintGL = self.OnPaint
        #self.update = lambda : self.updateGL()
        #self.frame.Bind(wx.EVT_CLOSE, self.OnExitApp)
        self.resizeGL = lambda w,h: self.reshape(w,h)
        #self.aboutToQuit.connect(self.OnExitApp)
        #self._timer = QtCore.QTimer(self)
        #self._timer.setInterval(0)
        #self._timer.timeout.connect(self.OnIdle)
        #self.V.mousePressEvent = self.mousePressEvent
        #self.V.mouseReleaseEvent = self.mouseReleaseEvent
        #self.V.mouseMoveEvent = self.mouseMoveEvent
        #self.V.wheelEvent = self.wheelEvent
        #self.V.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        #self.V.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
        #self.V.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)
        #self.V.Bind(wx.EVT_RIGHT_UP, self.OnRightUp)
        #self.V.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        #self.V.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        #self.V.Bind(wx.EVT_MOUSEWHEEL, self.OnWheelRotate)
        #self.KeyDownHandler = self.get_key_function(self.KeyDown)
        #self.KeyUpHandler = self.get_key_function(self.KeyUp)
        #self.SpecialKeyDownHandler = self.get_key_function(self.SpecialKeyDown)
        #self.SpecialKeyUpHandler = self.get_key_function(self.SpecialKeyUp)
        #self.V.Bind(wx.EVT_CHAR, self.OnKeyDown)
        #self.V.Bind(wx.EVT_KEY_DOWN,self.OnKeyDown)
        #print(self.SpecialKeyDown)
        #self.V.Bind(wx.EVT_KEY_UP,self.OnKeyUp)
class PaletteAdjuster(PaletteWidget):
    def __init__(self, parent=None):
        PaletteWidget.__init__(self, parent)
        self._sprs = [self.spr,Shader()]
        self._cur_spr = 0
        self.switch_spr(1)
        path_to_AV = os.path.dirname(__file__)
        self.shader_extern_load(*map(lambda x : os.path.join(path_to_AV,x), ["2dv_const_c.shader","2df_const_c.shader"]) )
        self.adjuster_widget = PaletteAlphaControl(self.cbox)
        self.switch_spr(0)
    def switch_spr(self, i):
        "Переключает шейдерную программу, служебная функция"
        self.spr = self._sprs[i]
        self._cur_spr = i
    def set_pal(self, pal_name):
        "Устанавливает палитру"
        checkOpenGLerror()
        #print("Adjuster set_pal start")
        PaletteWidget.set_pal(self, pal_name)
        checkOpenGLerror()
        #print("Adjuster set_pal end")
        self.MakeCurrent()
        #print("Adjuster pal load")
        self.load_on_device()
        checkOpenGLerror()
        #print("Adjuster pal loaded")
        self.update()
    def display(self):
        checkOpenGLerror()
        #print("Adjuster display")
        self.V.display()
        self.switch_spr(0)
        self.spr.start()
        self.tex.use_texture(self.spr,"pal")
        self.V.plot(self.spr)
        self.cbox.plot(self.spr)
        self.spr.stop()
        self.switch_spr(1)
        self.spr.start()
        self.V.plot(self.spr)
        self.adjuster_widget.plot(self.spr)
        self.spr.stop()
        checkOpenGLerror()
        self.SwapBuffers()
    def set_alpha(self, color_num, alpha):
        set_alpha__(self,color_num, alpha)
    def set_alpha__(self,color_num, alpha):
        self.MakeCurrent()
        self.adjuster_widget.set_alpha(color_num, alpha)
        self.update()
    def load_on_device(self):
        #print("Adjuster load on device")
        self.MakeCurrent()
        #print("Loading pal")
        self.cbox.load_on_device()
        checkOpenGLerror()
        #print("Loading line")
        self.adjuster_widget.load_on_device()
        checkOpenGLerror()
        #print("Line loaded")
        #self.update()
        #print("widget updated")
    def plot(self):
        flushOpenGLerror()
        #print("Adjuster plot pal =", self.cur_pal)
        #self.MakeCurrent()
        self.load_on_device()
    def OnLeftDown(self, evt):
        #scale = self.GetContentScaleFactor()
        x,y = evt.pos().x(), evt.pos().y()
        #x*=scale
        #y*=scale
        w,h = self.width, self.height
        texlength = self.tex.get_length()
        if self.cbox.get_vertical():
            self.set_alpha( int((h-y)*texlength/h), float(x)/w)
        else:
            self.set_alpha( int(x*texlength/w), float(h-y)/h)
    def BindAll(self):
        PaletteWidget.BindAll(self)
        #self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        self.mousePressEvent = self.OnLeftDown
