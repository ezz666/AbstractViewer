SHELL:=/bin/bash
#PY_INCLUDE:=/usr/include/python2.7
PY_INCLUDE:=/usr/include/python3.6m
CXX:=g++
modules:=plottable,shaderprog,viewer_template
CXXOPT:=-fopenmp -O3 -fPIC -g -std=c++11 -DPYTHON -I$(PY_INCLUDE) # `wx-config-gtk3 --cxxflags` -Wall
LINKOPT:= -shared -lpthread -lGL -lGLU -lGLEW # `wx-config-gtk3 --libs --gl-libs`
all: _viewer.so viewer.py
viewer_wrap.cxx viewer.py: viewer.i $(shell echo {$(modules)}.hpp)
	swig -python -c++ -DPYTHON viewer.i
viewer_wrap.o: viewer_wrap.cxx
	$(CXX) $(CXXOPT) -o $@ -c $<
%.o: %.cpp
	$(CXX) $(CXXOPT) -o $@ -c $<
_viewer.so: $(shell echo {$(modules)}.o) viewer_wrap.o
	$(CXX) $(LINKOPT) -o $@ $^ 

#--------------------------------------------------------------------------------------------------------
#   extras to makefile
#--------------------------------------------------------------------------------------------------------

mkextras:=$(firstword $(MAKEFILE_LIST)).extras
$(shell echo '# This file is generated automatically, do not edit it!' > $(mkextras))
$(shell echo '# The file contains additional dependencies and rules for building your project.' >> $(mkextras))
$(shell for m in *.cpp; do $(CXX) $(CXXOPT) -M $$m >> $(mkextras); done)
include $(mkextras)

clean:
	rm -f *.o *_wrap.o *_wrap.cxx _viewer.so viewer.py
