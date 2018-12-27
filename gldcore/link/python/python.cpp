
#include <pthread.h>
#include "gridlabd.h"
#include <Python.h>
#include "exec.h"
#include "save.h"
#include "cmdarg.h"

void gridlabd_exception(const char *format, ...);
static PyObject *gridlabd_reset(PyObject *self, PyObject *args);
static PyObject *gridlabd_command(PyObject *self, PyObject *args);
static PyObject *gridlabd_start(PyObject *self, PyObject *args);
static PyObject *gridlabd_wait(PyObject *self, PyObject *args);
static PyObject *gridlabd_cancel(PyObject *self, PyObject *args);
static PyObject *gridlabd_pause(PyObject *self, PyObject *args);
static PyObject *gridlabd_pauseat(PyObject *self, PyObject *args);
static PyObject *gridlabd_resume(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_global(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_value(PyObject *self, PyObject *args);
static PyObject *gridlabd_save(PyObject *self, PyObject *args);
static PyObject *gridlabd_set_global(PyObject *self, PyObject *args);
static PyObject *gridlabd_set_value(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"command", gridlabd_command, METH_VARARGS, "Send a command argument to the GridLAB-D instance"},
    {"start", gridlabd_start, METH_VARARGS, "Start the GridLAB-D instance"},
    {"wait", gridlabd_wait, METH_VARARGS, "Wait for the GridLAB-D instance to stop"},
    {"cancel", gridlabd_cancel, METH_VARARGS, "Cancel the GridLAB-D instance"},
    {"pause", gridlabd_pause, METH_VARARGS, "Pause the GridLAB-D instance"},
    {"pauseat",gridlabd_pauseat, METH_VARARGS, "Pause the GridLAB-D instance at a specified time"},
    {"resume",gridlabd_resume, METH_VARARGS, "Resume the GridLAB-D instance"},
    {"save", gridlabd_save, METH_VARARGS, "Dump model to a file"},
    {"get_global", gridlabd_get_global, METH_VARARGS, "Get a GridLAB-D global variable"},
    {"get_value", gridlabd_get_value, METH_VARARGS, "Get a GridLAB-D object property"},
    {"set_global", gridlabd_set_global, METH_VARARGS, "Set a GridLAB-D global variable"},
    {"set_value", gridlabd_set_value, METH_VARARGS, "Set a GridLAB-D object property"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef gridlabdmodule = {
    PyModuleDef_HEAD_INIT,
    "gridlabd",   /* name of module */
    "GridLAB-D simulation", /* module documentation, may be NULL */
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

void gridlabd_exception(const char *format, ...)
{
    char buffer[1024];
    va_list arg;
    va_start(arg,format);
    vsnprintf(buffer,sizeof(buffer),format,arg);
    PyErr_SetString(gridlabdException,buffer);
    va_end(arg);
}

extern "C" char **environ;
char **saved_environ = NULL;
void save_environ(void)
{
    saved_environ = (char**)malloc(sizeof(char*)*1024);
    int i;
    for ( i = 0 ; i < 1023 ; i++ )
    {
        if ( environ[i] == NULL )
            break;
        saved_environ[i] = strdup(environ[i]);
    }
    saved_environ[i] = NULL;
}
void restore_environ(void)
{
    if ( saved_environ )
        environ = saved_environ;
}

int argc = 1;
char *argv[1024] = {"gridlabd"};
bool is_started = false;
bool is_stopped = false;
pthread_t main_thread;
int return_code = 0;

static PyObject *gridlabd_reset(PyObject *self, PyObject *args)
{
    gridlabd_exception("reset not implemented yet");
    return NULL;
}

static PyObject *gridlabd_command(PyObject *self, PyObject *args)
{
    const char *command;
    int code = 0;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    if ( argc < (int)(sizeof(argv)/sizeof(argv[0])) )
        argv[argc++] = strdup(command);
    else
        gridlabd_exception("too many commands (limit is %d)", sizeof(argv)/sizeof(argv[0]));
    return PyLong_FromLong(code);
}

void *gridlabd_main(void *)
{
    int main(int, char*[]);
    int code = main(argc,argv);
    exec_mls_done();
    is_stopped = true;
    return (void*)code;
}

static PyObject *gridlabd_start(PyObject *self, PyObject *args)
{
    const char *command;
    int code = -1;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    else if ( is_started )
    {
        gridlabd_exception("gridlabd already started");
        return NULL;
    }
    else if ( strcmp(command,"thread") == 0 )
    {
        PyEval_InitThreads();
        save_environ();
        is_started = true;
        global_multirun_mode = MRM_LIBRARY;
        pthread_create(&main_thread, NULL, gridlabd_main, NULL);
        printf("gridlabd_state('thread'): global_mainloopstate=%d, is_stopped=%d\n", global_mainloopstate, is_stopped);
        if ( ! is_stopped )
        {
            exec_mls_statewait(~MLS_INIT);
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
        }
        return PyLong_FromLong(0);
    }
    else if ( strcmp(command,"pause") == 0 )
    {
        PyEval_InitThreads();
        save_environ();
        is_started = true;
        global_multirun_mode = MRM_LIBRARY;
        pthread_create(&main_thread, NULL, gridlabd_main, NULL);
        printf("gridlabd_state('pause'): global_mainloopstate=%d, is_stopped=%d\n", global_mainloopstate, is_stopped);
        exec_mls_statewait(~MLS_INIT);
        global_mainloopstate = MLS_PAUSED;
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
        return PyLong_FromLong(0);
    }
    else if ( strcmp(command, "wait") == 0 )
    {
        return PyLong_FromLong((long)gridlabd_main(NULL));
    }
    else
        gridlabd_exception("start mode '%s' is not recognized", command);
    return NULL;
}
static PyObject *gridlabd_wait(PyObject *self, PyObject *args)
{
    if ( ! is_stopped )
    {
        int code = pthread_join(main_thread, NULL);
        restore_environ();
    }
    return PyLong_FromLong(0);
}
static PyObject *gridlabd_cancel(PyObject *self, PyObject *args)
{
    if ( is_started && ! is_stopped )
    {
        pthread_cancel(main_thread);
    }
    return PyLong_FromLong(0);
}

static PyObject *gridlabd_pause(PyObject *self, PyObject *args)
{
    exec_mls_resume(global_clock);
    exec_mls_statewait(MLS_PAUSED);
    return PyLong_FromLong(0);
}
static PyObject *gridlabd_pauseat(PyObject *self, PyObject *args)
{
    char *value;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &value) )
        return NULL;
    TIMESTAMP ts = convert_to_timestamp(value);
    exec_mls_resume(ts);
    if ( global_mainloopstate != MLS_RUNNING )
        exec_mls_statewait(MLS_RUNNING);
    exec_mls_statewait(MLS_PAUSED);
    return PyLong_FromLong(0);
}
static PyObject *gridlabd_resume(PyObject *self, PyObject *args)
{
    exec_mls_resume(TS_NEVER);
    return PyLong_FromLong(0);
}

static PyObject *gridlabd_get_global(PyObject *self, PyObject *args)
{
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &name) )
        return NULL;
    char value[1024];
    exec_rlock_sync();
    char *result = global_getvar(name,value,sizeof(value));
    exec_runlock_sync();
    if ( result == NULL )
    {
        gridlabd_exception("unable to get global '%s'",name);
        return NULL;
    }
    return PyBytes_FromString(value);
}

static PyObject *gridlabd_set_global(PyObject *self, PyObject *args)
{
    char *name;
    char *value;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "ss", &name, &value) )
        return NULL;
    exec_wlock_sync();
    STATUS result = global_setvar(name,value);
    exec_wunlock_sync();
    if ( result == FAILED )
    {
        gridlabd_exception("unable to set global '%s' to value '%s'",name,value);
        return NULL;
    }
    return PyBytes_FromString(value);
}

static PyObject *gridlabd_get_value(PyObject *self, PyObject *args)
{
    char *name;
    char *property;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "ss", &name, &property) )
        return NULL;
    OBJECT *obj = object_find_name(name);
    if ( obj == NULL )
    {
        gridlabd_exception("object '%s' not found", name);
        return NULL;
    }

    char value[1024];
    exec_rlock_sync();
    int len = object_get_value_by_name(obj,property,value,sizeof(value));
    exec_runlock_sync();
    if ( len < 0 )
    {
        gridlabd_exception("object '%s' property '%s' not found", name, property);
        return NULL;
    }
    return PyBytes_FromString(value);
}

static PyObject *gridlabd_set_value(PyObject *self, PyObject *args)
{
    char *name;
    char *property;
    char *value;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "sss", &name, &property, &value) )
        return NULL;
    OBJECT *obj = object_find_name(name);
    if ( obj == NULL )
    {
        gridlabd_exception("object '%s' not found", name);
        return NULL;
    }

    exec_wlock_sync();
    int len = object_set_value_by_name(obj,property,value);
    exec_wunlock_sync();
    if ( len < 0 )
    {
        gridlabd_exception("cannot set object '%s' property '%s' to value '%s'", name, property, value);
        return NULL;
    }
    return PyBytes_FromString(value);
}

static PyObject *gridlabd_save(PyObject *self, PyObject *args)
{
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return NULL;
    exec_rlock_sync();
    int len = saveall(name);
    exec_runlock_sync();
    if ( len <= 0 )
    {
       gridlabd_exception("uname to save '%s'", name);
       return NULL;
    }
    return PyLong_FromLong(len);
}
