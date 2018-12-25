
#include "gridlabd.h"
#include "cmdarg.h"
#include <Python.h>

static char module_docstring[] =
    "This module provides an interface to GridLAB-D.";

static char gridlabd_command_docstring[] =
    "Send a command to the gridlabd instance.";

static char gridlabd_start_docstring[] =
    "Start the gridlabd instance.";

static PyObject *gridlabd_command(PyObject *self, PyObject *args);
static PyObject *gridlabd_start(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"command", gridlabd_command, METH_VARARGS, gridlabd_command_docstring},
    {"start", gridlabd_start, METH_VARARGS, gridlabd_start_docstring},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef gridlabdmodule = {
    PyModuleDef_HEAD_INIT,
    "gridlabd",   /* name of module */
    module_docstring, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    module_methods,
};

static PyObject *gridlabdException;

PyMODINIT_FUNC PyInit_gridlabd(void)
{
    PyObject *mod = PyModule_Create(&gridlabdmodule);
    if ( mod == NULL )
        return NULL;
    gridlabdException = PyErr_NewException("gridlabd.exception",NULL,NULL);
    Py_XINCREF(gridlabdException);
    PyModule_AddObject(mod,"exception",gridlabdException);
    return mod;
}

static void gridlabd_exception(const char *format, ...)
{
    char buffer[1024];
    va_list arg;
    va_start(arg,format);
    vsnprintf(buffer,sizeof(buffer),format,arg);
    PyErr_SetString(gridlabdException,buffer);
    va_end(arg);
}

int argc = 1;
char *argv[1024] = {"gridlabd"};

static PyObject *gridlabd_command(PyObject *self, PyObject *args)
{
    const char *command;
    int code = 0;
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    argv[argc++] = strdup(command);
    return PyLong_FromLong(code);
}

static PyObject *gridlabd_start(PyObject *self, PyObject *args)
{
    const char *command;
    int code = -1;
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    cmdarg_load(argc,argv);
    if ( strcmp(command,"batch") == 0 )
        return PyLong_FromLong(code);
    gridlabd_exception("start mode '%s' is not supported", command);
    Py_UNREACHABLE();
}
