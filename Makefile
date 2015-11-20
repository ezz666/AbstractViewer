name=viewer
headers=shaderprog.hpp viewer_template.hpp plottable.hpp
modules=viewer_template.cpp shaderprog.cpp plottable.cpp
LINKOPT=-lpthread -lglut -lGL -lGLU -lGLEW
#CXXOPT=-DDEBIAN
include aivlib/Makefile
