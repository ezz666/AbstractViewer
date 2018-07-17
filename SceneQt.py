# -*- coding: utf-8 -*-
from core import *
from PyQt4 import QtGui, QtCore, QtOpenGL
from PyQt4.QtOpenGL import QGLWidget
class Scene3DQT(QGLWidget):
    width,height= 300,300
    def __init__(self, parent = None, format = None):
        if format == None:
            format = QtOpenGL.QGLFormat()
            format.setVersion(3, 3)
            format.setProfile(QtOpenGL.QGLFormat.CoreProfile)
        #glformat.setSampleBuffers( True )
        super(Scene3DQT, self).__init__(format, parent)
        self.MakeCurrent()
        self.V = viewer.Viewer3D()
    def initializeGL(self):
        pass
        #self.SetMinSize( (300,300) )
    def __getattr__(self, key):
        try:
            return self.__getattribute__(key)
        except AttributeError:
            return getattr(self.V, key)
    #def plot(self, spr):
    #    self.MakeCurrent()
    #    self.V.plot(spr)
    #    self.update()
    def autoreshape(self):
        scale =self.GetContentScaleFactor()
        size = self.size =  self.GetClientSize()*scale
        self.V.reshape(self.size.width, self.size.height)
    def reshape(self, width, height):
        print("Reshape {}x{}".format(width, height))
        self.width = width
        self.height = height
        self.V.reshape(self.width, self.height)
    def update(self):
        pass
        #self.glDraw()
    def MakeCurrent(self):
        return self.makeCurrent()
    def SwapBuffers(self):
        self.swapBuffers()

class Scene2DQT(QGLWidget):
    width, height = 300,10
    def __init__(self, parent = None, format = None):
        if format == None:
            format = QtOpenGL.QGLFormat()
            format.setVersion(3, 3)
            format.setProfile(QtOpenGL.QGLFormat.CoreProfile)
        #glformat.setSampleBuffers( True )
        super(Scene2DQT, self).__init__(format, parent)
        self.MakeCurrent()
        self.V = viewer.Viewer2D()
    def initializeGL(self):
        pass
    def __getattr__(self, key):
        try:
            return self.__getattribute__(key)
        except AttributeError:
            return getattr(self.V, key)
    def autoreshape(self):
        #scale =self.GetContentScaleFactor()
        #size = self.size =  self.GetClientSize()*scale
        self.V.reshape(self.width, self.height)
    def reshape(self, width, height):
        self.width = width
        self.height = height
        self.V.reshape(self.width, self.height)
    def update(self):
        pass
        #self.glDraw()
    def MakeCurrent(self):
        return self.makeCurrent()
    def SwapBuffers(self):
        self.swapBuffers()
