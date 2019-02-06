
#include <pthread.h>
#include "gridlabd.h"
#include <Python.h>
#include "cmdarg.h"
#include "load.h"
#include "exec.h"
#include "save.h"

static PyObject *gridlabd_exception(const char *format, ...);

static PyObject *gridlabd_title(PyObject *self, PyObject *args);
static PyObject *gridlabd_version(PyObject *self, PyObject *args);
static PyObject *gridlabd_copyright(PyObject *self, PyObject *args);
static PyObject *gridlabd_credits(PyObject *self, PyObject *args);
static PyObject *gridlabd_license(PyObject *self, PyObject *args);

static PyObject *gridlabd_reset(PyObject *self, PyObject *args);
static PyObject *gridlabd_command(PyObject *self, PyObject *args);

static PyObject *gridlabd_start(PyObject *self, PyObject *args);
static PyObject *gridlabd_wait(PyObject *self, PyObject *args);
static PyObject *gridlabd_cancel(PyObject *self, PyObject *args);
static PyObject *gridlabd_pause(PyObject *self, PyObject *args);
static PyObject *gridlabd_pauseat(PyObject *self, PyObject *args);
static PyObject *gridlabd_resume(PyObject *self, PyObject *args);

static PyObject *gridlabd_module(PyObject *self, PyObject *args);
static PyObject *gridlabd_add(PyObject *self, PyObject *args);
static PyObject *gridlabd_load(PyObject *self, PyObject *args);
static PyObject *gridlabd_save(PyObject *self, PyObject *args);

static PyObject *gridlabd_get(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_global(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_value(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_class(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_object(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_transform(PyObject *self, PyObject *args);
static PyObject *gridlabd_get_schedule(PyObject *self, PyObject *args);

static PyObject *gridlabd_set_global(PyObject *self, PyObject *args);
static PyObject *gridlabd_set_value(PyObject *self, PyObject *args);

static PyObject *gridlabd_convert_unit(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"title", gridlabd_title, METH_VARARGS, "Get the software title"},
    {"version", gridlabd_version, METH_VARARGS, "Get the software version"},
    {"copyright", gridlabd_copyright, METH_VARARGS, "Get the software copyrights"},
    {"credits", gridlabd_credits, METH_VARARGS, "Get the software credits"},
    {"license", gridlabd_license, METH_VARARGS, "Get the software license"},
    // simulation control
    {"command", gridlabd_command, METH_VARARGS, "Send a command argument to the GridLAB-D instance"},
    {"start", gridlabd_start, METH_VARARGS, "Start the GridLAB-D instance"},
    {"wait", gridlabd_wait, METH_VARARGS, "Wait for the GridLAB-D instance to stop"},
    {"cancel", gridlabd_cancel, METH_VARARGS, "Cancel the GridLAB-D instance"},
    {"pause", gridlabd_pause, METH_VARARGS, "Pause the GridLAB-D instance"},
    {"pauseat",gridlabd_pauseat, METH_VARARGS, "Pause the GridLAB-D instance at a specified time"},
    {"resume",gridlabd_resume, METH_VARARGS, "Resume the GridLAB-D instance"},
    // model editing
    {"module", gridlabd_module, METH_VARARGS, "Load a python GridLAB-D module"},
    {"add", gridlabd_add, METH_VARARGS, "Add an element to the current model"}, 
    {"load", gridlabd_load, METH_VARARGS, "Load model from a file"},
    {"save", gridlabd_save, METH_VARARGS, "Save model to a file"},
    // gets
    {"get", gridlabd_get, METH_VARARGS, "Get a list of items"},
    {"get_global", gridlabd_get_global, METH_VARARGS, "Get a GridLAB-D global variable"},
    {"get_value", gridlabd_get_value, METH_VARARGS, "Get a GridLAB-D object property"},
    {"get_class", gridlabd_get_class, METH_VARARGS, "Get a GridLAB-D class"},
    {"get_object", gridlabd_get_object, METH_VARARGS, "Get a GridLAB-D object"},
    {"get_transform", gridlabd_get_transform, METH_VARARGS, "Get a GridLAB-D filter"},
    {"get_schedule", gridlabd_get_schedule, METH_VARARGS, "Get a GridLAB-D schedule"},
    // sets
    {"set_global", gridlabd_set_global, METH_VARARGS, "Set a GridLAB-D global variable"},
    {"set_value", gridlabd_set_value, METH_VARARGS, "Set a GridLAB-D object property"},
    // utilities
    {"convert_unit", gridlabd_convert_unit, METH_VARARGS, "Convert units of a float, complex or string"},

    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef gridlabdmodule = {
    PyModuleDef_HEAD_INIT,
    "gridlabd",   /* name of module */
    "Python GridLAB-D simulation", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    module_methods,
};

static PyObject *gridlabd_title(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s", PACKAGE_NAME);
}

static PyObject *gridlabd_version(PyObject *self, PyObject *args)
{
    return Py_BuildValue("{s:i,s:i,s:i,s:i,s:s}", 
        "major", global_version_major, 
        "minor", global_version_minor, 
        "patch", global_version_patch,
        "build", global_version_build,
        "branch", global_version_branch);
}

static PyObject *gridlabd_copyright(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s",
        PACKAGE_NAME " " PACKAGE_VERSION "\n"
        "\n"
        "Copyright (C) 2008-2017, Battelle Memorial Institute.\n"
        "Copyright (C) 2016-2019, The Board of Trustees of the Leland Stanford Junior University.\n"
        "All Rights Reserved.\n"
         "For additional information, see http://www.gridlabd.us/.\n");
}

static PyObject *gridlabd_credits(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s",
        PACKAGE_NAME " " PACKAGE_VERSION "\n"
        "\n"
        "GridLAB-D was developed with funding from the US Department of Energy and the California Energy Commission.\n"
        "For additional information, see http://www.gridlabd.us/.\n");
}

static PyObject *gridlabd_license(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s",
        PACKAGE_NAME " " PACKAGE_VERSION "\n"
        "Copyright (C) 2008-2017, Battelle Memorial Institute.\n"
        "Copyright (C) 2016-2019, The Board of Trustees of the Leland Stanford Junior University.\n"
        "All Rights Reserved.\n"
        "\n"
        "License Version 2.0, January 2019\n"
        "http://www.gridlabd.us/\n"
        "\n"
        "1. The Copyright Holders hereby grant permission to any person or entity\n"
        "   lawfully obtaining a copy of this software and associated documentation\n" 
        "   files (hereinafter \"the Software\") to redistribute and use the Software\n"
        "   in source andbinary forms, with or without modification.  Such person or\n"
        "   entity may use, copy, modify, merge, publish, distribute, sublicense,\n"
        "   and/or sell copies of the Software, and may permit others to do so,\n"
        "   subject to the following conditions:\n"
        "   - Redistributions of source code must retain the above copyright\n"
        "     notice, this list of conditions and the following disclaimers.\n"
        "   - Redistributions in binary form must reproduce the above copyright\n"
        "     notice, this list of conditions and the following disclaimer in\n"
        "     the documentation and/or other materials provided with the\n"
        "     distribution.\n"
        "   - Other than as used herein, the names of the Copyright Holders\n"
        "     shall not be used in any form whatsoever without their express\n"
        "     written consent.\n"
        "2. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n"
        "   \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n"
        "   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n"
        "   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BATTELLE OR\n"
        "   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n"
        "   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n"
        "   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n"
        "   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY\n"
        "   OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
        "   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
        "   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
        "3. The Software was originally produced by Battelle under Contract No.\n"
        "   DE-AC05-76RL01830 with the Department of Energy.  The U.S. Government\n"
        "   is granted for itself and others acting on its behalf a nonexclusive,\n"
        "   paid-up, irrevocable worldwide license in this data to reproduce,\n"
        "   prepare derivative works, distribute copies to the public, perform\n"
        "   publicly and display publicly, and to permit others to do so.  The\n"
        "   specific term of the license can be identified by inquiry made to\n"
        "   Battelle or DOE.  Neither the United States nor the United States\n"
        "   Department of Energy, nor any of their employees, makes any warranty,\n"
        "   express or implied, or assumes any legal liability or responsibility\n"
        "   for the accuracy, completeness or usefulness of any data, apparatus,\n"
        "   product or process disclosed, or represents that its use would not\n"
        "   infringe privately owned rights.\n"
        );
}

static PyObject *gridlabdException;
static PyObject *this_module = NULL;

class Callback {
    static const char *name;
public:
    inline Callback(const char *str) { name = str; };
    inline ~Callback(void) { name = NULL; };
    static inline bool is_active(void) { return name==NULL; };
};
const char * Callback::name = NULL;

PyMODINIT_FUNC PyInit_gridlabd(void)
{
    this_module = PyModule_Create(&gridlabdmodule);
    if ( this_module == NULL )
        return NULL;
    gridlabdException = PyErr_NewException("gridlabd.exception",NULL,NULL);
    Py_XINCREF(gridlabdException);
    PyModule_AddObject(this_module,"exception",gridlabdException);

    // adjustments for python modules
    global_glm_save_options = GSO_MINIMAL;

    // important constants needed by python modules
    PyModule_AddObject(this_module,"NEVER",PyLong_FromLong(TS_NEVER));

    return this_module;
}

static PyObject *gridlabd_exception(const char *format, ...)
{
    char buffer[1024];
    va_list arg;
    va_start(arg,format);
    vsnprintf(buffer,sizeof(buffer),format,arg);
    PyErr_SetString(gridlabdException,buffer);
    va_end(arg);
    return NULL;
}

//
// NOTE: environ is damanged by multithreading
//
// save and restore are necessary to preserve
// python's environment while gridlabd makes
// changes necessary for its operation
//
extern "C" char **environ;
static char **saved_environ = NULL;
static void save_environ(void)
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
static void restore_environ(void)
{
    if ( saved_environ )
        environ = saved_environ;
}

static int argc = 1;
static char *argv[1024] = {"gridlabd"};
static enum {
    GMS_NEW = 0, // module has been newly loaded 
    GMS_COMMAND, // module has received at least one command
    GMS_STARTED, // module has started simulation
    GMS_RUNNING, // module simulation is running
    GMS_FAILED,  // module has failed to start
    GMS_SUCCESS, // module has completed successfully
    GMS_CANCELLED, // module simulation was cancelled
} gridlabd_module_status = GMS_NEW;
static char *gridlabd_module_status_msg[] = {
    "module is new but not received commands yet", // NEW
    "module has received commands but not started yet", // COMMAND
    "module has started simulation but it is not running yet", // STARTED
    "simulation is running", // RUNNING
    "simulation has failed", // FAILED
    "simulation has completed", // SUCCESS
    "simulation was cancelled", // CANCELLED
};
static pthread_t main_thread;
static int return_code = 0;

static PyObject *gridlabd_reset(PyObject *self, PyObject *args)
{
    return gridlabd_exception("reset not implemented yet");
}

//
// >>> gridlabd.command(str cmdarg)
//
// Returns: (long) argument number
//
static PyObject *gridlabd_command(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status > GMS_COMMAND )
    {
        return gridlabd_exception("cannot accept command after starting");
    }
    gridlabd_module_status = GMS_COMMAND;
    const char *command;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    if ( argc < (int)(sizeof(argv)/sizeof(argv[0])) )
    {
        argv[argc] = strdup(command);
        return PyLong_FromLong(argc++);
    }
    else
    {
        return gridlabd_exception("too many commands (limit is %d)", sizeof(argv)/sizeof(argv[0]));
    }
}

static void *gridlabd_main(void *)
{
    int main(int, char*[]);
    gridlabd_module_status = GMS_RUNNING;
    return_code = main(argc,argv);
    gridlabd_module_status = ( return_code==0 ? GMS_SUCCESS : GMS_FAILED );
    exec_mls_done();
    return (void*)&return_code;
}

//
// >>> gridlabd.create(blockname,blockdata)
//
// Return: temporary GLM filename
//
static FILE *glmfh = NULL;
static char glmname[1024] = "";
static void cleanup_glm(void)
{
    if ( strcmp(glmname,"") != 0 )
    {
        unlink(glmname);
    }    
}
static PyObject *gridlabd_add(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status > GMS_STARTED )
    {
        return gridlabd_exception("cannot add after start");
    }
    if ( glmfh == NULL )
    {
        snprintf(glmname,sizeof(glmname)-1,"tmp_%05u.glm",getpid());
        glmfh = fopen(glmname,"w");
        if ( glmfh == NULL )
        {
            return gridlabd_exception("unable to create model file '%s'",glmname);
        }
        atexit(cleanup_glm);
        argv[argc++] = glmname;
        gridlabd_module_status = GMS_COMMAND;
    }

    char *block;
    PyObject *data;
    if ( !PyArg_ParseTuple(args,"sO", &block, &data) )
        return NULL;
    if ( strcmp(block,"global") == 0 )
    {
        PyObject *type = PyDict_GetItemString(data,"type");
        if ( type == NULL )
        {
            return gridlabd_exception("global type not specified");
        }
        PyObject *unit = PyDict_GetItemString(data,"unit");
        PyObject *name = PyDict_GetItemString(data,"name");
        if ( name == NULL )
        {
            return gridlabd_exception("global name not specified");
        }
        PyObject *value = PyDict_GetItemString(data,"value");
        if ( value == NULL )
        {
            return gridlabd_exception("global value not specified");
        }
        fprintf(glmfh,"global ");
        PyObject_Print(type,glmfh,Py_PRINT_RAW);
        fprintf(glmfh," ");
        PyObject_Print(name,glmfh,Py_PRINT_RAW);
        if ( unit ) 
        {
            fprintf(glmfh,"[");
            PyObject_Print(unit,glmfh,Py_PRINT_RAW);
            fprintf(glmfh,"]");
        }
        fprintf(glmfh," ");
        PyObject_Print(value,glmfh,Py_PRINT_RAW);
        fprintf(glmfh,";\n");
    }
    else if ( strcmp(block,"clock") == 0 )
    {
        fprintf(glmfh,"clock {\n");
        Py_ssize_t pos = 0;
        PyObject *key, *value;
        while ( PyDict_Next(data,&pos,&key,&value) )
        {
            fprintf(glmfh,"\t");
            PyObject_Print(key,glmfh,Py_PRINT_RAW);
            fprintf(glmfh," \"");
            PyObject_Print(value,glmfh,Py_PRINT_RAW);
            fprintf(glmfh,"\";\n");
        }
        fprintf(glmfh,"}\n");
    }
    else if ( strcmp(block,"module") == 0 )
    {
        PyObject *name = PyDict_GetItemString(data,"name");
        if ( name == NULL )
        {
            return gridlabd_exception("module name not specified");
        }
        fprintf(glmfh,"module ");
        PyObject_Print(name,glmfh,Py_PRINT_RAW);
        fprintf(glmfh," {\n");
        Py_ssize_t pos = 0;
        PyObject *key, *value;
        while ( PyDict_Next(data,&pos,&key,&value) )
        {
            if ( PyObject_RichCompareBool(key,Py_BuildValue("s","name"),Py_EQ) )
                continue;
            fprintf(glmfh,"\t");
            PyObject_Print(key,glmfh,Py_PRINT_RAW);
            fprintf(glmfh," \"");
            PyObject_Print(value,glmfh,Py_PRINT_RAW);
            fprintf(glmfh,"\";\n");
        }
        fprintf(glmfh,"}\n");
    }
    else if ( strcmp(block,"class") == 0 )
    {
        PyObject *name = PyDict_GetItemString(data,"name");
        if ( name == NULL )
        {
            return gridlabd_exception("class name not specified");
        }
        fprintf(glmfh,"class ");
        PyObject_Print(name,glmfh,Py_PRINT_RAW);
        fprintf(glmfh," {\n");
        Py_ssize_t pos = 0;
        PyObject *key, *value;
        while ( PyDict_Next(data,&pos,&key,&value) )
        {
            if ( PyObject_RichCompareBool(key,Py_BuildValue("s","name"),Py_EQ) )
                continue;
            fprintf(glmfh,"\t");
            PyObject_Print(key,glmfh,Py_PRINT_RAW);
            fprintf(glmfh," ");
            PyObject_Print(value,glmfh,Py_PRINT_RAW);
            fprintf(glmfh,";\n");
        }
        fprintf(glmfh,"}\n");
    }
    else if ( strcmp(block,"object") == 0 )
    {
        PyObject *oclass = PyDict_GetItemString(data,"class");
        if ( oclass == NULL )
        {
            return gridlabd_exception("object class not specified");
        }
        fprintf(glmfh,"object ");
        PyObject_Print(oclass,glmfh,Py_PRINT_RAW);
        fprintf(glmfh," {\n");
        Py_ssize_t pos = 0;
        PyObject *key, *value;
        while ( PyDict_Next(data,&pos,&key,&value) )
        {
            if ( PyObject_RichCompareBool(key,Py_BuildValue("s","class"),Py_EQ) )
                continue;
            fprintf(glmfh,"\t");
            PyObject_Print(key,glmfh,Py_PRINT_RAW);
            fprintf(glmfh," \"");
            PyObject_Print(value,glmfh,Py_PRINT_RAW);
            fprintf(glmfh,"\";\n");
        }
        fprintf(glmfh,"}\n");
    }
    else
    {
        return gridlabd_exception("block/class not recognized");
    }
    fflush(glmfh);
    return Py_BuildValue("s",glmname);
}

//
// >>> gridlabd.start('thread')
// >>> gridlabd.start('pause')
// >>> gridlabd.start('wait')
//
// Returns: (long) 0 on success, non-zero on failure
//
static PyObject *gridlabd_start(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status < GMS_COMMAND )
    {
        if ( object_get_count() == 0 )
        {
            return gridlabd_exception("cannot start unless commands received or objects defined");
        }
        else
        {
            argv[argc++] = "-e";
            argv[argc++] = "batch";
        }
    }
    else if ( gridlabd_module_status > GMS_COMMAND )
    {
        return gridlabd_exception("gridlabd already started");
    }
    const char *command;
    int code = -1;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &command) )
        return NULL;
    if ( glmfh != NULL )
        fclose(glmfh);
    if ( strcmp(command,"thread") == 0 || strcmp(command,"pause") == 0 )
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
            return gridlabd_exception("start('%s'): %s", command, gridlabd_module_status_msg[gridlabd_module_status]);
        }
        return PyLong_FromLong(0);
    }
    else if ( strcmp(command, "wait") == 0 )
    {
        return PyLong_FromLong((long)gridlabd_main(NULL));
    }
    else
        return gridlabd_exception("start mode '%s' is not recognized", command);
    return NULL;
}

//
// >>> gridlabd.wait()
//
// Returns: (long) exit code
//
static PyObject *gridlabd_wait(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        return gridlabd_exception("cannot wait unless running");
    }
    int code = pthread_join(main_thread, NULL);
    restore_environ();
    return PyLong_FromLong(code);
}

//
// >>> gridlabd.cancel()
//
// Returns: (long) 0
//
static PyObject *gridlabd_cancel(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        return gridlabd_exception("cannot cancel unless running");
    }
    pthread_cancel(main_thread);
    gridlabd_module_status = GMS_CANCELLED;
    return PyLong_FromLong(0);
}

//
// >>> gridlabd.pause()
//
// Returns: (long) global_clock
//
static PyObject *gridlabd_pause(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        return gridlabd_exception("cannot pause unless running");
    }
    exec_mls_resume(global_clock);
    exec_mls_statewait(MLS_PAUSED);
    return PyLong_FromLong(global_clock);
}

//
// >>> gridlabd.pauseat(datetime)
//
// Returns: (long) global_clock
//
static PyObject *gridlabd_pauseat(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        return gridlabd_exception("cannot pause unless running");
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
    return PyLong_FromLong(global_clock);
}

//
// >>> gridlabd.resume()
// 
// Returns: (long) 0
//
static PyObject *gridlabd_resume(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status != GMS_RUNNING )
    {
        return gridlabd_exception("cannot resume unless running");
    }
    exec_mls_resume(TS_NEVER);
    return PyLong_FromLong(0);
}

//
// >>> gridlabd.save(filename)
//
// Returns: (long) bytes written
//
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
       return gridlabd_exception("uname to save '%s'", name);
    }
    return PyLong_FromLong(len);
}

//
// >>> gridlabd.get('classes')
// >>> gridlabd.get('modules')
// >>> gridlabd.get('globals')
// >>> gridlabd.get('objects')
// >>> gridlabd.get('transforms')
// >>> gridlabd.get('schedules')
//
// Returns: (long) list of item names, empty if none found
//
static PyObject *gridlabd_get(PyObject *self, PyObject *args)
{
    char *type;
    PyObject *data = NULL;
    restore_environ();
    if ( PyArg_ParseTuple(args,"s", &type) )
    {
        if ( strcmp(type,"objects") == 0 )
        {
            data = PyList_New(NULL);
            OBJECT *obj;
            for ( obj = object_get_first() ; obj != NULL ; obj = object_get_next(obj) )
            {
                if ( obj->name )
                    PyList_Append(data,Py_BuildValue("s",obj->name));
                else
                {
                    char name[1024];
                    snprintf(name,sizeof(name),"%s:%d",obj->oclass->name,obj->id);
                    PyList_Append(data,Py_BuildValue("s",name));
                }
            }
        }
        else if ( strcmp(type,"classes") == 0 )
        {
            data = PyList_New(NULL);
            CLASS *oclass;
            for ( oclass = class_get_first_class() ; oclass != NULL ; oclass = oclass->next )
            {
                PyList_Append(data,Py_BuildValue("s",oclass->name));
            }
        }
        else if ( strcmp(type,"modules") == 0 )
        {
            data = PyList_New(NULL);
            MODULE *mod;
            for ( mod = module_get_first() ; mod != NULL ; mod = mod->next )
            {
                PyList_Append(data,Py_BuildValue("s",mod->name));
            }
        }
        else if ( strcmp(type,"globals") == 0 )
        {
            data = PyList_New(NULL);
            GLOBALVAR *var;
            for ( var = global_find(NULL) ; var != NULL ; var = var->next )
            {
                PyList_Append(data,Py_BuildValue("s",var->prop->name));
            } 
        }
        else if ( strcmp(type,"transforms") == 0 )
        {
            data = PyList_New(NULL);
            TRANSFORM *transform = NULL;
            while ( (transform = transform_getnext(NULL)) != NULL )
            {
                if ( transform->function_type == XT_FILTER )
                    PyList_Append(data,Py_BuildValue("s",transform->tf->name));
            } 
            return data;
        }
        else if ( strcmp(type,"schedules") == 0 )
        {
            data = PyList_New(NULL);
            SCHEDULE *sch;
            for ( sch = schedule_getfirst() ; sch != NULL ; sch = schedule_getnext(sch) )
            {
                PyList_Append(data,Py_BuildValue("s",sch->name));
            }
            return data;
        }
        else
            return gridlabd_exception("get(type='%s'): type '%s' is not valid", type);
    }
    return data;
}

//
// >>> gridlabd.get_global(name)
//
// Returns: (str) value
//
static PyObject *gridlabd_get_global(PyObject *self, PyObject *args)
{
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "s", &name) )
        return NULL;
    char value[1024];
    if ( ! Callback::is_active() ) exec_rlock_sync();
    char *result = global_getvar(name,value,sizeof(value));
    if ( ! Callback::is_active() ) exec_runlock_sync();
    if ( result == NULL )
    {
        return gridlabd_exception("unable to get global '%s'",name);
    }
    return Py_BuildValue("s",value);
}

//
// >>> gridlabd.set_global(name,value)
//
// Returns: (str) old_value
//
static PyObject *gridlabd_set_global(PyObject *self, PyObject *args)
{
    char *name;
    char *value;
    restore_environ();
    if ( ! PyArg_ParseTuple(args, "ss", &name, &value) )
        return NULL;
    char previous[1024]="";
    exec_wlock_sync();
    if ( ! global_getvar(name,previous,sizeof(previous)) )
    {
        exec_wunlock_sync();
        return gridlabd_exception("unable to get old value of global '%s'",name);
    }
    STATUS result = global_setvar(name,value);
    exec_wunlock_sync();
    if ( result == FAILED )
    {
        return gridlabd_exception("unable to set global '%s' to value '%s'",name,value);
    }
    return Py_BuildValue("s",previous);
}

//
// >>> gridlabd.get_value(name,property)
//
// Returns: (str) value
//
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
        return gridlabd_exception("object '%s' not found", name);
    }

    char value[1024];
    exec_rlock_sync();
    int len = object_get_value_by_name(obj,property,value,sizeof(value));
    exec_runlock_sync();
    if ( len < 0 )
    {
        return gridlabd_exception("object '%s' property '%s' not found", name, property);
    }
    return Py_BuildValue("s",value);
}

//
// >>> gridlabd.set_value(name,property,value)
//
// Returns: (str) old_value
//
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
        return gridlabd_exception("object '%s' not found", name);
    }

    char previous[1024]="";
    exec_wlock_sync();
    int len = object_get_value_by_name(obj,property,value,sizeof(value));
    if ( len < 0 )
    {
        exec_wunlock_sync();
        return gridlabd_exception("unable to get old value of '%s.%s'", name,property);
    }
    len = object_set_value_by_name(obj,property,value);
    exec_wunlock_sync();
    if ( len < 0 )
    {
        return gridlabd_exception("cannot set object '%s' property '%s' to value '%s'", name, property, value);
    }
    return Py_BuildValue("s",previous);
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

//
// >>> gridlabd.get_class(name)
//
// Returns: (dict) class data
//
static PyObject *gridlabd_get_class(PyObject *self, PyObject *args)
{
    if ( gridlabd_module_status < GMS_RUNNING )
    {
        return gridlabd_exception("cannot get unless running");
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
        return gridlabd_exception("class '%s' not found", name);
    }
    PyObject *data = PyDict_New();
    PyDict_SetItemString(data,"class.object_size",Py_BuildValue("L",(unsigned long long)oclass->size));
    PyDict_SetItemString(data,"class.trl",Py_BuildValue("L",(unsigned long long)oclass->trl));
    PyDict_SetItemString(data,"profiler.numobjs",Py_BuildValue("L",(unsigned long long)oclass->profiler.numobjs));
    PyDict_SetItemString(data,"profiler.clocks",Py_BuildValue("L",(unsigned long long)oclass->profiler.clocks));
    PyDict_SetItemString(data,"profiler.count",Py_BuildValue("L",(unsigned long long)oclass->profiler.count));
    if ( oclass->module != NULL )
        PyDict_SetItemString(data,"class.module",Py_BuildValue("s",oclass->module->name));
    if ( oclass->parent != NULL )
        PyDict_SetItemString(data,"class.parent",Py_BuildValue("s",oclass->parent->name));
    PROPERTY *prop;
    for ( prop = oclass->pmap ; prop != NULL && prop->oclass==oclass ; prop = prop->next )
    {
        PyObject *property = PyDict_New();
        PROPERTYSPEC *spec = property_getspec(prop->ptype);
        if ( spec->size > 0 && spec->size < 1024 )
        {
            PyDict_SetItemString(property,"type",Py_BuildValue("s",spec->name));
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
            PyDict_SetItemString(property,"access",Py_BuildValue("s",access));
            if ( prop->keywords != NULL )
            {
                PyObject *keywords = PyDict_New();
                KEYWORD *key;
                for ( key = prop->keywords ; key != NULL ; key = key->next )
                {
                    char buffer[1024];
                    snprintf(buffer,sizeof(buffer),"%p",(void*)(key->value));
                    PyDict_SetItemString(keywords,key->name,Py_BuildValue("s",buffer));
                }
                PyDict_SetItemString(property,"keywords",keywords);
            }
            if ( prop->unit != NULL )
            {
                PyDict_SetItemString(property,"unit",Py_BuildValue("s",prop->unit->name));
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

//
// >>> gridlabd.get_object(name)
//
// Returns: (dict) object data
//
static PyObject *gridlabd_get_object(PyObject *self, PyObject *args)
{
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return NULL;
    OBJECT *obj = object_find_name(name);
    if ( obj == NULL )
    {
        return gridlabd_exception("object '%s' not found", name);
    }

    PyObject *data = PyDict_New();
    if ( obj->oclass->name != NULL )
        PyDict_SetItemString(data,"class",Py_BuildValue("s",obj->oclass->name));
    if ( obj->parent != NULL )
    {
        if ( obj->parent->name == NULL )
        {
            char buffer[1024];
            snprintf(buffer,sizeof(buffer),"%s:%d",obj->parent->oclass->name,obj->parent->id);
            PyDict_SetItemString(data,"parent",Py_BuildValue("s",buffer));
        }
        else
            PyDict_SetItemString(data,"parent",Py_BuildValue("s",obj->parent->name));
    }
    if ( ! isnan(obj->latitude) ) PyDict_SetItemString(data,"latitude",Py_BuildValue("d",obj->latitude));
    if ( ! isnan(obj->longitude) ) PyDict_SetItemString(data,"longitude",Py_BuildValue("d",obj->longitude));
    if ( obj->groupid[0] != '\0' ) PyDict_SetItemString(data,"groupid",Py_BuildValue("s",(const char*)obj->groupid));
    PyDict_SetItemString(data,"rank",Py_BuildValue("L",(unsigned long long)obj->rank));
    char buffer[1024];
    if ( convert_from_timestamp(obj->clock,buffer,sizeof(buffer)) )
        PyDict_SetItemString(data,"clock",Py_BuildValue("s",buffer));
    if ( obj->valid_to > TS_ZERO && obj->valid_to < TS_NEVER ) PyDict_SetItemString(data,"valid_to",Py_BuildValue("L",(unsigned long long)(obj->valid_to)));
    PyDict_SetItemString(data,"schedule_skew",Py_BuildValue("L",(unsigned long long)obj->schedule_skew));
    if ( obj->in_svc > TS_ZERO && obj->in_svc < TS_NEVER ) PyDict_SetItemString(data,"in",Py_BuildValue("L",(unsigned long long)(obj->in_svc)));
    if ( obj->out_svc > TS_ZERO && obj->out_svc < TS_NEVER ) PyDict_SetItemString(data,"out",Py_BuildValue("L",(unsigned long long)(obj->out_svc)));
    PyDict_SetItemString(data,"rng_state",Py_BuildValue("L",(unsigned long long)(obj->rng_state)));
    PyDict_SetItemString(data,"heartbeat",Py_BuildValue("L",(unsigned long long)(obj->heartbeat)));
    snprintf(buffer,sizeof(buffer),"%llx",(unsigned long long)obj->guid[0]);
    PyDict_SetItemString(data,"guid",Py_BuildValue("s",buffer));
    snprintf(buffer,sizeof(buffer),"%llx",(unsigned long long)obj->flags);
    PyDict_SetItemString(data,"guid",Py_BuildValue("s",buffer));

    exec_rlock_sync();
    PROPERTY *prop;
    for ( prop = get_first_property(obj) ; prop != NULL ; prop = get_next_property(prop) )
    {
        PROPERTYSPEC *spec = property_getspec(prop->ptype);
        if ( spec->size > 0 && spec->size < 1024 )
        {
            char value[1024] = "";
            if ( get_property_value(obj,prop,value,sizeof(value)) > 0 )
                PyDict_SetItemString(data,prop->name,Py_BuildValue("s",value));
        }
    }
    exec_runlock_sync();
    return data;
}

//
// >>> gridlabd.get_transform(name)
//
// Returns: (dict) tranform data
//
static PyObject *gridlabd_get_transform(PyObject *self, PyObject *args)
{
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return NULL;
    return gridlabd_exception("not implemented yet");
}

//
// >>> gridlabd.get_schedule(name)
//
// Returns: (dict) schedule data
//
static PyObject *gridlabd_get_schedule(PyObject *self, PyObject *args)
{
    char *name;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return NULL;
    SCHEDULE *sch = schedule_find_byname(name);
    if ( sch == NULL )
    {
        return gridlabd_exception("schedule '%s' not found",name);
    }
    PyObject *data = PyDict_New();
    PyDict_SetItemString(data,"definition",Py_BuildValue("s",sch->definition));
    PyObject *calendars = PyList_New(NULL);
    size_t calendar;
    for ( calendar = 0 ; calendar < 14 ; calendar++ )
    {
        PyObject *values = PyDict_New();
        size_t minute;
        double last = NaN;
        for ( minute = 0 ; minute < sizeof(sch->index[calendar])/sizeof(sch->index[calendar][0]) ; minute++ )
        {
            double value = sch->data[sch->index[calendar][minute]];
            if ( last != value )
            {
                PyDict_SetItem(values,PyLong_FromLong(minute),PyFloat_FromDouble(value));
                last = value;
            }
        }
        PyList_Append(calendars,values);
    }
    PyDict_SetItemString(data,"calendars",calendars);
    return data;
}

static PyObject *gridlabd_load(PyObject *self, PyObject *args)
{
    char *file;
    if ( ! PyArg_ParseTuple(args,"s", &file) )
        return NULL;
    size_t before = object_get_count();
    if ( loadall(file) != SUCCESS )
    {
        return gridlabd_exception("load failed");
    }
    size_t after = object_get_count();
    return PyLong_FromLong(after-before);
}

//
// >>> gridlabd.convert_unit(float,from,to)
// >>> gridlabd.convert_unit(complex,from,to)
// >>> gridlabd.convert_unit(string,to)
//
// Returns: float or complex
//
static PyObject *gridlabd_convert_unit(PyObject *self, PyObject *args)
{
    char *value, *from, *to;
    double real;
    complex cmplx;
    restore_environ();
    if ( PyArg_ParseTuple(args,"dss", &real, &from, &to) )
    {
        UNIT *pFrom = unit_find(from);
        if ( pFrom == NULL )
        {
            return gridlabd_exception("unit '%s' not found", from);
        }
        UNIT *pTo = unit_find(to);
        if ( pTo == NULL )
        {
            return gridlabd_exception("unit '%s' not found", to);
        }
        if ( ! unit_convert_ex(pFrom,pTo,&real) )
        {
            return gridlabd_exception("unable to convert '%s' from '%s' to '%s'", value, from, to);
        }
        return Py_BuildValue("d",real);
    }
    else if ( PyArg_ParseTuple(args,"Dss", &cmplx, &from, &to) )
    {
        return gridlabd_exception("complex unit conversion not implemented");       
    }
    else if ( PyArg_ParseTuple(args,"ss",&value,&to) )
    {
        PyErr_Clear();
        char unit[1024]="";
        if ( sscanf(value,"%lf %1023s",&real,&unit) < 2 )
        {
            return gridlabd_exception("unable to parse value and unit of '%s'", value);
        }
        UNIT *pFrom = unit_find(unit);
        if ( pFrom == NULL )
        {
            return gridlabd_exception("unit '%s' not found", from);
        }
        UNIT *pTo = unit_find(to);
        if ( pTo == NULL )
        {
            return gridlabd_exception("unit '%s' not found", to);
        }
        if ( ! unit_convert_ex(pFrom,pTo,&real) )
        {
            return gridlabd_exception("unable to convert '%s' from '%s' to '%s'", value, from, to);
        }
        return Py_BuildValue("d",real);
    }
}

/////////////////////
// module interface
////////////////////
static PyObject *modlist = NULL;
static MODULE python_module;
static PyObject *python_init = NULL;
static PyObject *python_precommit = NULL;
static PyObject *python_presync = NULL;
static PyObject *python_sync = NULL;
static PyObject *python_postsync = NULL;
static PyObject *python_commit = NULL;
static PyObject *python_term = NULL;
extern "C" bool on_init(void)
{
    size_t n;
    for ( n = 0 ; n < PyList_Size(python_init) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_init,n);
        PyObject *arg = Py_BuildValue("(i)",global_clock);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        bool retval = false; 
        if ( result ) 
        {
            retval = PyObject_IsTrue(result);
            Py_DECREF(result);
        }
        if ( ! retval )
        {
            output_error("python on_init() failed");
            return false;
        }
    }
    return true;
}
extern "C" TIMESTAMP on_precommit(TIMESTAMP t0)
{
    Callback("on_precommit");

    size_t n;
    TIMESTAMP t1 = TS_NEVER;
    for ( n = 0 ; n < PyList_Size(python_precommit) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_precommit,n);
        PyObject *arg = Py_BuildValue("(i)",t0);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        TIMESTAMP t2 = TS_INVALID; 
        if ( result ) 
        {
            if ( PyLong_Check(result) )
                t2 = PyLong_AsLong(result);
            else
                output_error("python on_precommit(%d) returned an invalid type (expected long)",t0);
            Py_DECREF(result);
        }
        else
            output_error("python on_precommit(%d) returned nothing (expected long)",t0);
        if ( t2 == TS_INVALID )
            return t2;
        else if ( absolute_timestamp(t2) < absolute_timestamp(t1) )
            t1 = t2;
    }
    return t1;
}
extern "C" TIMESTAMP on_presync(TIMESTAMP t0)
{
    Callback("on_presync");
    size_t n;
    TIMESTAMP t1 = TS_NEVER;
    for ( n = 0 ; n < PyList_Size(python_presync) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_presync,n);
        PyObject *arg = Py_BuildValue("(i)",t0);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        TIMESTAMP t2 = TS_INVALID; 
        if ( result ) 
        {
            if ( PyLong_Check(result) )
                t2 = PyLong_AsLong(result);
            else
                output_error("python on_presync(%d) returned an invalid type (expected long)",t0);
            Py_DECREF(result);
        }
        else
            output_error("python on_presync(%d) returned nothing (expected long)",t0);
        if ( t2 == TS_INVALID )
            return t2;
        else if ( absolute_timestamp(t2) < absolute_timestamp(t1) )
            t1 = t2;
    }
    return t1;
}
extern "C" TIMESTAMP on_sync(TIMESTAMP t0)
{
    Callback("on_sync");
    size_t n;
    TIMESTAMP t1 = TS_NEVER;
    for ( n = 0 ; n < PyList_Size(python_sync) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_sync,n);
        PyObject *arg = Py_BuildValue("(i)",t0);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        TIMESTAMP t2 = TS_INVALID; 
        if ( result ) 
        {
            if ( PyLong_Check(result) )
                t2 = PyLong_AsLong(result);
            else
                output_error("python on_sync(%d) returned an invalid type (expected long)",t0);
            Py_DECREF(result);
        }
        else
            output_error("python on_sync(%d) returned nothing (expected long)",t0);
        if ( t2 == TS_INVALID )
            return t2;
        else if ( absolute_timestamp(t2) < absolute_timestamp(t1) )
            t1 = t2;
    }
    return t1;
}
extern "C" TIMESTAMP on_postsync(TIMESTAMP t0)
{
    Callback("on_postsync");
    size_t n;
    TIMESTAMP t1 = TS_NEVER;
    for ( n = 0 ; n < PyList_Size(python_postsync) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_postsync,n);
        PyObject *arg = Py_BuildValue("(i)",t0);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        TIMESTAMP t2 = TS_INVALID; 
        if ( result ) 
        {
            if ( PyLong_Check(result) )
                t2 = PyLong_AsLong(result);
            else
                output_error("python on_postsync(%d) returned an invalid type (expected long)",t0);
            Py_DECREF(result);
        }
        else
            output_error("python on_postsync(%d) returned nothing (expected long)",t0);
        if ( t2 == TS_INVALID )
            return t2;
        else if ( absolute_timestamp(t2) < absolute_timestamp(t1) )
            t1 = t2;
    }
    return t1;
}
extern "C" bool on_commit(TIMESTAMP t)
{
    Callback("on_commit");

    size_t n;
    for ( n = 0 ; n < PyList_Size(python_commit) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_commit,n);
        PyObject *arg = Py_BuildValue("(i)",t);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        bool retval = false; 
        if ( result ) 
        {
            retval = PyObject_IsTrue(result);
            Py_DECREF(result);
        }
        if ( ! retval )
        {
            output_error("python on_commit(%d) failed",t);
            return false;
        }
    }
    return true;
}
extern "C" void on_term(void)
{
    Callback("on_term");
    size_t n;
    for ( n = 0 ; n < PyList_Size(python_term) ; n++ )
    {
        PyObject *call = PyList_GetItem(python_term,n);
        PyObject *arg = Py_BuildValue("(i)",global_clock);
        PyObject *result = PyEval_CallObject(call,arg);
        Py_DECREF(arg);
        if ( result && result != Py_None ) 
        {
            Py_DECREF(result);
            output_warning("python on_term() return an unexpected type (expected None)");
        }    
    }
    return;

}
static int python_import_file(const char *file)
{
    // TODO
    return false;
}
static bool get_callback(
    PyObject *mod,
    const char *file,
    const char *name, 
    const char *def, 
    PyObject **list)
{
    PyObject *dict = PyModule_GetDict(mod);
    if ( dict == NULL || ! PyDict_Check(dict) )
    {
        output_error("module does not have a namespace dict");       
        return false;
    }

    if ( *list == NULL )
        *list = PyList_New(0);
    PyObject *call = PyDict_GetItemString(dict,def);
    if ( call )
    {
        if ( PyCallable_Check(call) )
            PyList_Append(*list,call);
        else 
        {
            output_error("%s.%s is not callable",file,def);
            return false;
        }
    }
    return true;    
}
extern "C" MODULE *python_module_load(const char *file, int argc, char *argv[])
{
    PyObject *mod = PyImport_ImportModule(file);
    if ( mod == NULL)
        return NULL;

    if ( ! PyModule_Check(mod) )
    {
        gridlabd_exception("object is not a python module");
        return NULL;
    }

    if ( modlist == NULL )
        modlist = PyList_New(0);
    int n;
    for ( n = 0 ; n < PyList_Size(modlist) ; n++ )
    {
        if ( PyList_GetItem(modlist,n) == mod )
            return &python_module;
    }

    // TODO: link module to core
    strcpy(python_module.name,file);
    python_module.oclass = NULL;
    python_module.major = global_version_major;
    python_module.minor = global_version_minor;
    python_module.getvar = NULL;
    python_module.setvar = NULL;
    python_module.import_file = python_import_file;
    python_module.export_file = NULL;
    python_module.check = NULL;
    /* deltamode */
    python_module.deltadesired = NULL;
    python_module.preupdate = NULL;
    python_module.interupdate = NULL;
    python_module.deltaClockUpdate = NULL;
    python_module.postupdate = NULL;
    /* clock hook*/
    python_module.clockupdate = NULL;
    python_module.cmdargs = NULL;
    python_module.kmldump = NULL;
    python_module.test = NULL;
    python_module.subload = NULL;
    python_module.globals = NULL;
    python_module.term = NULL;
    python_module.stream = NULL;
    python_module.next = NULL;
#define GET_CALLBACK(X) (get_callback(mod,file,#X,"on_"#X,&python_##X) ? on_##X : NULL)
    python_module.on_init = GET_CALLBACK(init);
    python_module.on_precommit = GET_CALLBACK(precommit);
    python_module.on_presync = GET_CALLBACK(presync);
    python_module.on_sync = GET_CALLBACK(sync);
    python_module.on_postsync = GET_CALLBACK(postsync);
    python_module.on_commit = GET_CALLBACK(commit);
    python_module.on_term = GET_CALLBACK(term);

    PyList_Append(modlist,mod);
    PyModule_AddObject(mod,"gridlabd",this_module);

    return &python_module;
}

//
// >>> gridlabd.module(module)
// 
// Links the specified module to the gridlabd core
//
static PyObject *gridlabd_module(PyObject *self, PyObject *args)
{
    char *name = NULL;
    restore_environ();
    if ( ! PyArg_ParseTuple(args,"s", &name) )
        return gridlabd_exception("unable to import python module (name not given)");
    MODULE *mod = python_module_load(name,0,NULL);
    if ( ! module_find(name) )
    {
        output_message("python module '%s' loaded ok", name);
        module_add(mod);
    }
    else
        output_message("python module '%s' already loaded", name);
    return PyLong_FromLong(PyList_Size(modlist)-1);
}
