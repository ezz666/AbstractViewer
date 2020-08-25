from core import *
from OpenGL import GLUT as glut
# Scene3DGLUT is kind of dummy class since GLUT doesn't have widgets.
# Thus there is no way to add Scene3DGLUT to a window, but to keep
# interface the same we should extract window into a separate class.
class Scene3DGLUT:
    def __init__(self, parent):
        self.MakeCurrent()
        self.V = viewer.Viewer3D()
        glut.glutInitDisplayMode(glut.GLUT_RGBA|glut.GLUT_DOUBLE|glut.GLUT_DEPTH)
        #glut.glutInitWindowSize(self.V.get_width(), self.V.get_height())# window should go to Frame class instead.
        #glut.glutCreateWindow("viewer")
        self._needs_update = False
    def __getattr__(self, key):
        try:
            return self.__dict__[key]
        except KeyError:
            return getattr(self.V, key)
    def autoreshape(self):
        #scale = 1 # dpi scaling
        #size = self.size =  self.GetClientSize()*scale
        self.V.reshape(self.size.width, self.size.height)
    def reshape(self, width, height):
        self.width = width
        self.height = height
        self.V.reshape(self.width, self.height)
    def update(self):
        self._needs_update = True
    def real_redraw(self):
        glut.glutPostRedisplay()
    def MakeCurrent(self):
        pass
    def SwapBuffers(self):
        glut.glutSwapBuffers()
