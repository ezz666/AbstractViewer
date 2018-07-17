# ifndef TWOANDTHREE_H
# define TWOANDTHREE_H
#include "Python.h"
#include "patchlevel.h"

# if PY_MAJOR_VERSION==2
PyObject* MyPyBytes_FromStringAndSize(const char *v, Py_ssize_t len){
  return PyString_FromStringAndSize(v, len);
}
int MyPyBytes_Check(PyObject *o){
  return PyString_Check(o);
}
char* MyPyString_AsString(PyObject *string){
  return PyString_AsString(string);
}
PyObject* MyBytes_FromUnicode(PyObject * string){
  return string;
}
# elif PY_MAJOR_VERSION==3
PyObject* MyPyBytes_FromStringAndSize(const char *v, Py_ssize_t len){
  return PyBytes_FromStringAndSize(v, len);
}
int MyPyBytes_Check(PyObject *o){
  return PyBytes_Check(o);
}
char* MyPyString_AsString(PyObject *string){
  return PyBytes_AsString(string);
}
PyObject* MyBytes_FromUnicode(PyObject * string){
  return PyUnicode_AsUTF8String(string);
}
# endif //PY_MAJOR_VERSION
# endif // TWOANDTHREE_H
