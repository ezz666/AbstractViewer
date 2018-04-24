import wx
from wx import glcanvas
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
class ViewerWX(UniversalViewer, wx.App):
    def __init__(self, argv):
        wx.App.__init__(self, redirect=False)
        self.ExitOnFrameDelete=True
        #self.argv= argv
    def OnInit(self):
        frame = wx.Frame(None, -1, "RunDemo: ",size=(-1,-1),  pos=(0,0),
                        style=wx.DEFAULT_FRAME_STYLE, name="run a sample")
        #frame.CreateStatusBar()



        # set the frame to a good size for showing the two buttons
        #win.SetFocus()
        #self.window = win
        #frect = frame.GetRect()
        #glarglist = int_array(len(attrib_list))
        #for i, v in enumerate(attrib_list):
        #    glarglist[i] = v
        frame.Show(True)
        frame.SetFocus()
        self.V = SceneWX(frame)#, glarglist)
        self.frame = frame
        self.SetTopWindow(frame)
        self.AbstractInit()
        #self.Bind()
        return True
    def OnPaint(self, event):
        self.Draw()
    def Draw(self):
        self.V.SetCurrent(self.V.context)
        self.display()
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
    def SetWindowTitle(self, string):
        self.frame.SetTitle(string)
    def OnExitApp(self, evt):
        self.exit()
    def OnSize(self, event):
        self.V.reshape()
        #event.Skip()
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
        evt.Skip()
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
    def OnLeftUp(self, evt):
        self.V.mouse_left_release(*self.OnMouseDown(evt))
    def OnRightDown(self, evt):
        self.V.mouse_right_click(*self.OnMouseDown(evt))
    def OnRightUp(self, evt):
        self.V.mouse_right_release(*self.OnMouseDown(evt))
    def OnWheelRotate(self,evt):
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

