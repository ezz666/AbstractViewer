%module viewer
%typemap(out) bool&   %{ $result = PyBool_FromLong    ( *$1 ); %}
%typemap(out) char&   %{ $result = PyInt_FromLong     ( *$1 ); %}
%typemap(out) short&  %{ $result = PyInt_FromLong     ( *$1 ); %}
%typemap(out) int&    %{ $result = PyInt_FromLong     ( *$1 ); %}
%typemap(out) long&   %{ $result = PyInt_FromLong     ( *$1 ); %}
%typemap(out) float&  %{ $result = PyFloat_FromDouble ( *$1 ); %}
%typemap(out) double& %{ $result = PyFloat_FromDouble ( *$1 ); %}
%feature("autodoc","1");
%include "carrays.i";
%array_class(float, float_array);
%array_class(int, int_array);
%{
#include "shaderprog.hpp"
#include "viewer_template.hpp"
#include "plottable.hpp"
%}
%include "shaderprog.hpp"
%include "viewer_template.hpp"
%include "plottable.hpp"
