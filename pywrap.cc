#include <Python.h>
#include <cstdio>
#include <string>
#ifdef _WIN32
#include "stbfclient4win.h"
#else
#include <fastdfs/stbfclient.h>
#endif

extern "C" {
static PyObject* stbf_tt(PyObject* self, PyObject* args) {
    stbf_test();
    Py_RETURN_TRUE;
}

static PyObject* stbf_stcnf(PyObject* self, PyObject* args) {
    char* fn = nullptr;
    if (PyArg_ParseTuple(args, "s", &fn)) {
        if (stbf_setconf(fn) == 0) Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* stbf_up(PyObject* self, PyObject* args) {
    char* fn = nullptr;
    char buf[1024] = { 0 };
    if (PyArg_ParseTuple(args, "s", &fn)) {
        if (stbf_upload(fn, buf) == 0) {
            std::string str_id(buf);
            return PyBytes_FromStringAndSize(str_id.c_str(), str_id.size());
        }
    }
    Py_RETURN_FALSE;
}

static PyObject* stbf_down(PyObject* self, PyObject* args) {
    char* fdfs_id = nullptr;
    char* locfn = nullptr;
    if (PyArg_ParseTuple(args, "ss", &fdfs_id, &locfn)) {
        if (stbf_download(fdfs_id, locfn) == 0) Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* stbf_del(PyObject* self, PyObject* args) {
    char* fdfs_id = nullptr;
    if (PyArg_ParseTuple(args, "s", &fdfs_id)) {
        if (stbf_delete(fdfs_id) == 0) Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* stbf_rls(PyObject* self, PyObject* args) {
	stbf_release();
	Py_RETURN_TRUE;
}

#define PYFUNC(name) \
    { #name, name, METH_VARARGS, "" }
#define PYENDFUNC \
    { nullptr, nullptr, 0, nullptr }

PyMethodDef* get_all_methods() {
    static PyMethodDef g_python_methods[]{
        PYFUNC(stbf_tt), PYFUNC(stbf_stcnf), PYFUNC(stbf_up),
        PYFUNC(stbf_down), PYFUNC(stbf_del), PYFUNC(stbf_rls), PYENDFUNC,
    };
    return g_python_methods;
}

PyMODINIT_FUNC initlibpywrap() {
    PyObject* module = Py_InitModule("libpywrap", get_all_methods());
    if (module == nullptr) return;
}
}
