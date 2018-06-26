from core import *
import wx.glcanvas
class Scene3DWX(wx.glcanvas.GLCanvas):
    def __init__(self, parent):
        attrlist = wx.glcanvas.GLContextAttrs()
        attrlist.PlatformDefaults()#.DebugCtx()
        attrlist.CoreProfile().OGLVersion(3,3).EndList()#CoreProfile().OGLVersion(3,0)

        wx.glcanvas.GLCanvas.__init__(self, parent)
        #print("Attr Size ", attrlist.Size)
        self.context = wx.glcanvas.GLContext(self,None, ctxAttrs = attrlist)
        #print( glGetIntegerv(GL_MAJOR_VERSION),
        #    glGetIntegerv(GL_MINOR_VERSION))
        self.MakeCurrent()
        self.V = viewer.Viewer3D()
        self.SetMinSize( (300,300) )
    def __getattr__(self, key):
        try:
            return self.__getattribute__(key)
        except AttributeError:
            return getattr(self.V, key)

    def autoreshape(self):
        scale =self.GetContentScaleFactor()
        size = self.size =  self.GetClientSize()*scale
        self.V.reshape(self.size.width, self.size.height)
    def reshape(self, width, height):
        self.size.width = width
        self.size.height = height
        self.V.reshape(self.size.width, self.size.height)
    def update(self):
        self.Refresh(False)
    def MakeCurrent(self):
        self.SetCurrent(self.context)

class Scene2DWX(wx.glcanvas.GLCanvas):
    def __init__(self, parent):
        attrlist = wx.glcanvas.GLContextAttrs()
        attrlist.PlatformDefaults()#.DebugCtx()
        attrlist.CoreProfile().OGLVersion(3,3).EndList()#CoreProfile().OGLVersion(3,0)

        wx.glcanvas.GLCanvas.__init__(self, parent)
        #print("Attr Size ", attrlist.Size)
        self.context = wx.glcanvas.GLContext(self,None, ctxAttrs = attrlist)
        self.MakeCurrent()
        #print( glGetIntegerv(GL_MAJOR_VERSION),
        #    glGetIntegerv(GL_MINOR_VERSION))
        self.V = viewer.Viewer2D()
        self.SetMinSize( (300,10) )
    def __getattr__(self, key):
        try:
            return self.__getattribute__(key)
        except AttributeError:
            return getattr(self.V, key)

    def autoreshape(self):
        scale =self.GetContentScaleFactor()
        size = self.size =  self.GetClientSize()*scale
        self.V.reshape(self.size.width, self.size.height)
    def reshape(self, width, height):
        self.size.width = width
        self.size.height = height
        self.V.reshape(self.size.width, self.size.height)
    def update(self):
        self.Refresh(False)
    def MakeCurrent(self):
        self.SetCurrent(self.context)
