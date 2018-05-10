cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)
project(AbstractViewer)

option(PY3 "Create Python3 bindings instead of Python2" ON)
find_package (SWIG REQUIRED)
include(${SWIG_USE_FILE})
if(PY3)
  find_package( PythonLibs 3 REQUIRED )
else(NOT PY3)
  find_package( PythonLibs 2 REQUIRED )
endif(PY3)
include_directories(${PYTHON_INCLUDE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_SWIG_FLAGS "-DPYTHON")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_PATH})
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_PATH})
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_PATH})

set_property(SOURCE viewer.i PROPERTY CPLUSPLUS ON)
SWIG_ADD_LIBRARY(viewer
  TYPE MODULE
  LANGUAGE python
  SOURCES viewer.i plottable.cpp shaderprog.cpp viewer_template.cpp
)
#swig_add_module(viewer python viewer.i plottable.cpp shaderprog.cpp viewer_template.cpp) # DEPRECATED but may be useful for old versions
swig_link_libraries(viewer ${PYTHON_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
configure_file(UniversalViewer.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(SceneWX.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(async_input.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(ViewerWX.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(core.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(f.shader ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(fragment_index.shader ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(v.shader ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(2dv.shader ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(2df.shader ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(vertex_index.shader ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
