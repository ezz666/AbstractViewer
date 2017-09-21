SHELL:=/bin/bash
PY_INCLUDE:=/usr/include/python2.7
CXX:=g++
modules:=plottable,shaderprog,viewer_template
CXX_OPT:=-fopenmp -O3 -fPIC -g -std=c++11 -DPYTHON -I$(PY_INCLUDE) -Wall
LINK_OPT:= -shared -lpthread -lglut -lGL -lGLU -lGLEW
all: _viewer.so viewer.py
viewer_wrap.cxx viewer.py: viewer.i $(shell echo {$(modules)}.hpp)
	swig -python -c++ -DPYTHON -DPYLIKE_PRINT_R viewer.i
viewer_wrap.o: viewer_wrap.cxx
	$(CXX) $(CXX_OPT) -o $@ -c $<
%.o: %.cpp
	$(CXX) $(CXX_OPT) -o $@ -c $<
_viewer.so: $(shell echo {$(modules)}.o) viewer_wrap.o
	$(CXX) $(LINK_OPT) -o $@ $^ 

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
