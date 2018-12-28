
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
static PyObject *gridlabd_get(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_class(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_object(PyObject *self, PyObject *args);

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
    {"get", gridlabd_get, METH_VARARGS, "Get a list of items"},
    {"get_class", gridlabd_get_class, METH_VARARGS, "Get a GridLAB-D class"},
    {"get_object", gridlabd_get_object, METH_VARARGS, "Get a GridLAB-D object"},
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
enum {
    GMS_NEW = 0, // module has been newly loaded 
    GMS_COMMAND, // module has received at least one command
    GMS_STARTED, // module has started simulation
    GMS_RUNNING, // module simulation is running
    GMS_FAILED,  // module has failed to start
    GMS_SUCCESS, // module has completed successfully
    GMS_CANCELLED, // module simulation was cancelled
} gridlabd_module_status = GMS_NEW;
char *gridlabd_module_status_msg[] = {
    "module is new but not received commands yet", // NEW
    "module has received commands but not started yet", // COMMAND
    "module has started simulation but it is not running yet", // STARTED
    "simulation is running", // RUNNING
    "simulation has failed", // FAILED
    "simulation has completed", // SUCCESS
    "simulation was cancelled", // CANCELLED
};
pthread_t main_thread;
int return_code = 0;

static PyObject *gridlabd_reset(PyObject *self, PyObject *args)
{
    gridlabd_exception("reset not implemented yet");
    return NULL;
}

static PyObject *gridlabd_command(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status > GMS_COMMAND )
    {
        gridlabd_exception("cannot accept command after starting");
        return NULL;
    }
    gridlabd_module_status = GMS_COMMAND;
    const char *command;
    int code = 0;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    if ( argc < (int)(sizeof(argv)/sizeof(argv[0])) )
        argv[argc++] = strdup(command);
    else
    {
        gridlabd_exception("too many commands (limit is %d)", sizeof(argv)/sizeof(argv[0]));
        return NULL;
    }
    return PyLong_FromLong(code);
}

void *gridlabd_main(void *)
{
    int main(int, char*[]);
    gridlabd_module_status = GMS_RUNNING;
    return_code = main(argc,argv);
    gridlabd_module_status = ( return_code==0 ? GMS_SUCCESS : GMS_FAILED );
    exec_mls_done();
    return (void*)&return_code;
}

static PyObject *gridlabd_start(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status < GMS_COMMAND )
    {
        gridlabd_exception("cannot start unless commands have been received");
        return NULL;
    }
    else if ( gridlabd_module_status > GMS_COMMAND )
    {
        gridlabd_exception("gridlabd already started");
        return NULL;
    }
    const char *command;
    int code = -1;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    else if ( strcmp(command,"thread") == 0 || strcmp(command,"pause") == 0 )
    {
        PyEval_InitThreads();
        save_environ();
        exec_mls_create();
        gridlabd_module_status = GMS_STARTED;
        global_multirun_mode = MRM_LIBRARY;
        pthread_create(&main_thread, NULL, gridlabd_main, NULL);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
        while ( gridlabd_module_status == GMS_STARTED )
        {
            usleep(100);
        }
        exec_mls_statewait(~MLS_INIT);
        if ( strcmp(command,"pause") == 0 )
            exec_mls_suspend();
        if ( gridlabd_module_status != GMS_RUNNING )
        {
            gridlabd_exception("start('%s'): %s", command, gridlabd_module_status_msg[gridlabd_module_status]);
            return NULL;
        }
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
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        gridlabd_exception("cannot wait unless running");
        return NULL;
    }
    int code = pthread_join(main_thread, NULL);
    restore_environ();
    return PyLong_FromLong(code);
}
static PyObject *gridlabd_cancel(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        gridlabd_exception("cannot cancel unless running");
        return NULL;
    }
    pthread_cancel(main_thread);
    gridlabd_module_status = GMS_CANCELLED;
    return PyLong_FromLong(0);
}

static PyObject *gridlabd_pause(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        gridlabd_exception("cannot pause unless running");
        return NULL;
    }
    exec_mls_resume(global_clock);
    exec_mls_statewait(MLS_PAUSED);
    return PyLong_FromLong(0);
}
static PyObject *gridlabd_pauseat(PyObject *self, PyObject *args)
{
     if ( gridlabd_module_status != GMS_RUNNING )
    {
        gridlabd_exception("cannot pause unless running");
        return NULL;
    }
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
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        gridlabd_exception("cannot resume unless running");
        return NULL;
    }
    exec_mls_resume(TS_NEVER);
    return PyLong_FromLong(0);
}

static PyObject *gridlabd_get_global(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot get unless running");
        return NULL;
    }
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
   if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot set unless running");
        return NULL;
    }
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
    if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot get unless running");
        return NULL;
    }
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
   if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot set unless running");
        return NULL;
    }
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
   if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot save unless running");
        return NULL;
    }
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

static PyObject *gridlabd_get(PyObject *self, PyObject *args)
{
   if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot get unless running");
        return NULL;
    }
    char *type;
    PyObject *data = NULL;
    restore_environ();
    if ( PyArg_ParseTuple(args,"s", &type) )
    {
        if ( strcmp(type,"objects") == 0 )
        {
            data = PyList_New(0);
            OBJECT *obj;
            for ( obj = object_get_first() ; obj != NULL ; obj = object_get_next(obj) )
            {
                if ( obj->name )
                    PyList_Append(data,PyBytes_FromString(obj->name));
                else
                {
                    char name[1024];
                    snprintf(name,sizeof(name),"%s:%d",obj->oclass->name,obj->id);
                    PyList_Append(data,PyBytes_FromString(name));
                }
            }
        }
        else if ( strcmp(type,"classes") == 0 )
        {
            data = PyList_New(0);
            CLASS *oclass;
            for ( oclass = class_get_first_class() ; oclass != NULL ; oclass = oclass->next )
            {
                PyList_Append(data,PyBytes_FromString(oclass->name));
            }
        }
        else if ( strcmp(type,"modules") == 0 )
        {
            data = PyList_New(0);
            MODULE *mod;
            for ( mod = module_get_first() ; mod != NULL ; mod = mod->next )
            {
                PyList_Append(data,PyBytes_FromString(mod->name));
            }
        }
        else if ( strcmp(type,"globals") == 0 )
        {
            data = PyList_New(0);
            GLOBALVAR *var;
            for ( var = global_find(NULL) ; var != NULL ; var = var->next )
            {
                PyList_Append(data,PyBytes_FromString(var->prop->name));
            } 
        }
        else
            gridlabd_exception("get(type='%s'): type '%s' is not valid", type);
    }
    return data;
}

static PROPERTY *get_first_property(OBJECT *obj)
{
    return obj->oclass->pmap;
}
static PROPERTY *get_next_property(PROPERTY *prop)
{
    PROPERTY *next = prop->next;
    if ( next == NULL )
        return prop->oclass->parent ? prop->oclass->parent->pmap : NULL;
    else if ( next->oclass == prop->oclass )
        return next;
    else
        return NULL;
}

static PyObject *gridlabd_get_class(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot get unless running");
        return NULL;
    }
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return NULL;
    CLASS *oclass;
    for ( oclass = class_get_first_class() ; oclass != NULL ; oclass = oclass->next )
    {
        if ( strcmp(oclass->name,name) == 0 )
            break;
    }
    if ( oclass == NULL )
    {
        gridlabd_exception("class '%s' not found", name);
        return NULL;
    }
    PyObject *data = PyDict_New();
    PyDict_SetItemString(data,"class.object_size",PyBytes_FromFormat("%lu",(unsigned long)oclass->size));
    PyDict_SetItemString(data,"class.trl",PyBytes_FromFormat("%lu",(unsigned long)oclass->trl));
    PyDict_SetItemString(data,"profiler.numobjs",PyBytes_FromFormat("%lu",(unsigned long)oclass->profiler.numobjs));
    PyDict_SetItemString(data,"profiler.clocks",PyBytes_FromFormat("%lu",(unsigned long)oclass->profiler.clocks));
    PyDict_SetItemString(data,"profiler.count",PyBytes_FromFormat("%lu",(unsigned long)oclass->profiler.count));
    if ( oclass->module != NULL )
        PyDict_SetItemString(data,"class.module",PyBytes_FromString(oclass->module->name));
    if ( oclass->parent != NULL )
        PyDict_SetItemString(data,"class.parent",PyBytes_FromString(oclass->parent->name));
    PROPERTY *prop;
    for ( prop = oclass->pmap ; prop != NULL && prop->oclass==oclass ; prop = prop->next )
    {
        PyObject *property = PyDict_New();
        PROPERTYSPEC *spec = property_getspec(prop->ptype);
        if ( spec->size > 0 && spec->size < 1024 )
        {
            PyDict_SetItemString(property,"type",PyBytes_FromString(spec->name));
            char access[1024] = "";
            switch ( prop->access ) {
            case PA_PUBLIC: strcpy(access,"PUBLIC"); break;
            case PA_REFERENCE: strcpy(access,"REFERENCE"); break;
            case PA_PROTECTED: strcpy(access,"PROTECTED"); break;
            case PA_PRIVATE: strcpy(access,"PRIVATE"); break;
            case PA_HIDDEN: strcpy(access,"HIDDEN"); break;
            case PA_N: strcpy(access,"NONE"); break;
            default:
                if ( prop->access & PA_R ) strcat(access,"R");
                if ( prop->access & PA_W ) strcat(access,"W");
                if ( prop->access & PA_S ) strcat(access,"S");
                if ( prop->access & PA_L ) strcat(access,"L");
                if ( prop->access & PA_H ) strcat(access,"H");
                break;
            }
            PyDict_SetItemString(property,"access",PyBytes_FromString(access));
            if ( prop->keywords != NULL )
            {
                PyObject *keywords = PyDict_New();
                KEYWORD *key;
                for ( key = prop->keywords ; key != NULL ; key = key->next )
                    PyDict_SetItemString(keywords,key->name,PyBytes_FromFormat("%p",(unsigned long long)key->value));
                PyDict_SetItemString(property,"keywords",keywords);
            }
            PyDict_SetItemString(data,prop->name,property);
        }
    }
    return data;
}

static int get_property_value(OBJECT *obj, PROPERTY *prop, char *buffer, size_t len)
{
    void *addr = property_addr(obj,prop);
    return property_write(prop,addr,buffer,len);
}

static PyObject *gridlabd_get_object(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status < GMS_RUNNING )
    {
        gridlabd_exception("cannot get unless running");
        return NULL;
    }
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return NULL;
    OBJECT *obj = object_find_name(name);
    if ( obj == NULL )
    {
        gridlabd_exception("object '%s' not found", name);
        return NULL;
    }

    PyObject *data = PyDict_New();
    PyDict_SetItemString(data,"name",PyBytes_FromString(obj->name));
    // TODO add other header data

    exec_rlock_sync();
    PROPERTY *prop;
    for ( prop = get_first_property(obj) ; prop != NULL ; prop = get_next_property(prop) )
    {
        PROPERTYSPEC *spec = property_getspec(prop->ptype);
        if ( spec->size > 0 && spec->size < 1024 )
        {
            char value[1024] = "";
            if ( get_property_value(obj,prop,value,sizeof(value)) > 0 )
                PyDict_SetItemString(data,prop->name,PyBytes_FromString(value));
        }
    }
    exec_runlock_sync();
    return data;
}
