import wx
#from wx import glcanvas
from UniversalViewer import *
from SceneWX import *

KeycodeToKeyname ={
        wx.WXK_F1:"F1", wx.WXK_F2:"F2", wx.WXK_F3:"F3",
        wx.WXK_F4:"F4", wx.WXK_F5:"F5", wx.WXK_F6:"F6", wx.WXK_F7:"F7",
        wx.WXK_F8:"F8", wx.WXK_F9:"F9", wx.WXK_F10:"F10", wx.WXK_F11:"F11",
        wx.WXK_F12:"F12", wx.WXK_LEFT:"LEFT", wx.WXK_UP:"UP", wx.WXK_RIGHT:"RIGHT",
        wx.WXK_DOWN:"DOWN", wx.WXK_PAGEUP:"PAGE_UP", wx.WXK_PAGEDOWN:"PAGE_DOWN",
        wx.WXK_HOME:"HOME", wx.WXK_END:"END", wx.WXK_INSERT:"INSERT"
        }
class FrameWX(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self,None, -1, "RunDemo: ",size = (900,900),
                        name="run a sample")
        self.layout = wx.BoxSizer(wx.VERTICAL)
        self.SetSizerAndFit(self.layout)
    def add(self, wdg, size = 0, proportion= 1):
        self.layout.Add(wdg, proportion, wx.EXPAND|wx.ALL, size)

class ViewerWX(UniversalViewer, wx.App):
    def __init__(self, reader, rargv):
        UniversalViewer.__init__(self, reader)
        wx.App.__init__(self, redirect=False)
        self.ExitOnFrameDelete=True
        #self.argv= argv
    def OnInit(self):
        #frame.CreateStatusBar()
        frame = FrameWX()

        # set the frame to a good size for showing the two buttons
        #win.SetFocus()
        #self.window = win
        #frect = frame.GetRect()
        #glarglist = int_array(len(attrib_list))
        #for i, v in enumerate(attrib_list):
        #    glarglist[i] = v
        frame.Show(True)
        frame.SetFocus()
        #self.cbox = Scene2DWX(frame)
        #self.cbox.GL_init()
        self.V = Scene3DWX(frame)#, glarglist)
        self.V.SetFocus()
        self.frame = frame
        frame.add(self.V, 0, 30)
        self.SetTopWindow(frame)
        #self.Bind()
        return True
    def add_pal(self, name, pal_list):
        '''Добавляет палитру с именем name и цветами заданными в виде списка float со значениями от 0 до 1,
        они групируются по 3 формируя цвета, длина округляется до ближайшей снизу кратной 3'''
        self.V.MakeCurrent()
        UniversalViewer.add_pal(self, name, pal_list)
    def WakeUp(self):
        wx.WakeUpIdle()
    def OnPaint(self, event):
        self.Draw()
    def Draw(self):
        self.V.MakeCurrent()
        self.display()
        #self.cbox.SetCurrent(self.cbox.context)
        #print("DRAW")
    def Bind(self):
        self.V.Bind(wx.EVT_PAINT, self.OnPaint)
        self.frame.Bind(wx.EVT_CLOSE, self.OnExitApp)
        self.V.Bind(wx.EVT_SIZE, self.OnSize)
        self.V.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        self.V.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
        self.V.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)
        self.V.Bind(wx.EVT_RIGHT_UP, self.OnRightUp)
        self.V.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        self.V.Bind(wx.EVT_MOTION, self.OnMouseMotion)
        self.V.Bind(wx.EVT_MOUSEWHEEL, self.OnWheelRotate)
        self.KeyDownHandler = self.get_key_function(self.KeyDown)
        self.KeyUpHandler = self.get_key_function(self.KeyUp)
        self.SpecialKeyDownHandler = self.get_key_function(self.SpecialKeyDown)
        self.SpecialKeyUpHandler = self.get_key_function(self.SpecialKeyUp)
        #self.frame.Bind(wx.EVT_CHAR_HOOK, self.CharHook)
        #self.V.Bind(wx.EVT_CHAR, self.OnKeyDown)
        self.V.Bind(wx.EVT_KEY_DOWN,self.OnKeyDown)
        #print(self.SpecialKeyDown)
        self.V.Bind(wx.EVT_KEY_UP,self.OnKeyUp)
        self.V.Bind(wx.EVT_IDLE, self.OnIdle)
        self.timer = wx.Timer(self)
        self.V.Bind(wx.EVT_TIMER, self.OnTimer)
        self.timer.Start(42)
    def exit(self):
        "Закрывает окно и завершает програму"
        if (self._closed == True): return
        self._closed = True
        #os.kill(self.rl_reader.pid,signal.SIGHUP)
        #self.rl_reader.lock.acquire()
        #self.rl_reader.lock.release()
        #self._t.join()
        #glutLeaveMainLoop()
        self.frame.Show(True)
        self.frame.SetFocus()
        self.frame.Close(True)
        self.ExitMainLoop()
    def OnTimer(self, evt):
        self.WakeUp()
    def SetWindowTitle(self, string):
        self.frame.SetTitle(string)
    def OnExitApp(self, evt):
        self.exit()
    def OnSize(self, event):
        self.V.MakeCurrent()
        self.V.autoreshape()
        #event.Skip()
    def OnIdle(self, event):
        self.idle()
        self.V.SetFocus()
    def OnKey(self,evt):
        k = evt.GetUnicodeKey()
        sp_key=(k==0)
        if not sp_key:
            k = chr(k).lower()
        else:
            k = evt.GetKeyCode()
            if k in KeycodeToKeyname:
                k= KeycodeToKeyname[k]
            else: k = "None"
        x,y = evt.GetPosition()
        mod = {"Ctrl":evt.ControlDown(), "Shift":evt.ShiftDown(), "Alt":evt.AltDown()}
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
        return evt.GetPosition()
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
    def OnWheelRotate(self,evt):
        self.bb_auto = False
        rot = evt.WheelRotation
        delta = evt.WheelDelta
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
    def OnMouseMotion(self, evt):
        if evt.Dragging():
            self.V.drag(*self.OnMouseDown(evt))
            self.V.update()

class PaletteWidget(Scene2DWX):
    def __init__(self, parent):
        self.parent = parent
        Scene2DWX.__init__(self,parent)
        self.GL_init()
        self.cbox = PaletteBox()
        self.spr = Shader()
        path_to_AV = os.path.dirname(__file__)
        self.shader_extern_load(*map(lambda x : os.path.join(path_to_AV,x), ["2dv.shader","2df.shader"]) )
        checkOpenGLerror()
        self.palettes = {}
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
        truncate3 = lambda x: x - x%3
        nlen = truncate3(len(pal_list))
        pal = float_array(nlen)
        for i, v in enumerate(pal_list[:nlen]): pal[i] = v
        self.MakeCurrent()
        self.palettes[name] = Texture(pal, int(nlen/3))
    def set_pal(self, pal_name):
        "Устанавливает палитру"
        #print("setting pal {}".format(pal_name))
        self.tex = self.palettes[pal_name]
        self.cur_pal = pal_name
        self.MakeCurrent()
        self.cbox.set_texture( self.palettes[self.cur_pal] )
        self.plot()
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
        #print("Cbox DRAW")
        self.MakeCurrent()
        self.automove()
        self.display()
    def OnSize(self,evt):
        self.MakeCurrent()
        self.autoreshape()
        self.automove()
        self.update()
    def OnPaint(self, event):
        #print("CBOX paint")
        self.MakeCurrent()
        self.autoreshape()
        self.Draw()
    def BindAll(self):
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_SIZE, self.OnSize)
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
    def __init__(self, parent):
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
        PaletteWidget.set_pal(self, pal_name)
        self.MakeCurrent()
        self.adjuster_widget.load_on_device()
        self.update()
    def display(self):
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
        self.cbox.load_on_device()
        #print("Loading line")
        self.adjuster_widget.load_on_device()
        #print("Line loaded")
        #self.update()
        #print("widget updated")
    def plot(self):
        #print("Adjuster plot")
        self.load_on_device()
    def OnLeftDown(self, evt):
        scale = self.GetContentScaleFactor()
        x,y = evt.GetPosition()
        x*=scale
        y*=scale
        w,h = self.size.width, self.size.height
        texlength = self.tex.get_length()
        if self.cbox.get_vertical():
            self.set_alpha( int((h-y)*texlength/h), float(x)/w)
        else:
            self.set_alpha( int(x*texlength/w), float(h-y)/h)
    def BindAll(self):
        PaletteWidget.BindAll(self)
        self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)

