#include <Python.h>

static char module_docstring[] =
    "This module provides an interface to GridLAB-D.";

static char gridlabd_run_docstring[] =
    "Run a gridlabd instance.";

static PyObject *gridlabd_run(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"run", gridlabd_run, METH_VARARGS, gridlabd_run_docstring},
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

static PyObject *GridlabdError;

PyMODINIT_FUNC PyInit_gridlabd(void)
{
    PyObject *mod = PyModule_Create(&gridlabdmodule);
    if ( mod == NULL )
        return NULL;
    GridlabdError = PyErr_NewException("gridlabd.error",NULL,NULL);
    Py_XINCREF(GridlabdError);
    PyModule_AddObject(mod,"error",GridlabdError);
    return mod;
}

static PyObject *gridlabd_run(PyObject *self, PyObject *args)
{
    const char *command;
    int code = 0;

    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;

    PyErr_SetString(GridlabdError,"gridlabd command failed");

    return PyLong_FromLong(code);

    // PyObject *vector_a, *vector_b, *yerr_obj;

    // /* Parse the input tuple */
    // if (!PyArg_ParseTuple(args, "OO", &vector_a, &vector_b, &yerr_obj))
    //     return NULL;

    // PyObject* sequence_a;
    // PyObject* sequence_b;
    // PyObject *vector = PyList_New(PyList_Size(vector_a));
    // long len;
    // int i;

    // sequence_a = PySequence_Fast(vector_a, "expected a sequence");
    // sequence_b = PySequence_Fast(vector_b, "expected a sequence");
    // len = PySequence_Size(vector_a);
    // for (i = 0; i < len; i++) {
    //     PyObject* a_i = PySequence_Fast_GET_ITEM(sequence_a, i);
    //     PyObject* b_i = PySequence_Fast_GET_ITEM(sequence_b, i);
    //     PyObject* result = PyNumber_Subtract(a_i, b_i);
    //     PyList_SetItem(vector, i, result);
    // }

    // /* Build the output tuple */
    // PyObject *ret = Py_BuildValue("O", vector);
    // return ret;
}