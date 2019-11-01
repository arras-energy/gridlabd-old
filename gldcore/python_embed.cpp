// python.cpp

#include "gldcore.h"
#include <string.h>

const wchar_t *program = NULL;

void python_embed_init(int argc, const char *argv[])
{
    program = Py_DecodeLocale(argv[0],NULL);
    Py_SetProgramName(program);
    Py_Initialize();
    output_verbose("python initialization ok");
}

void python_embed_term()
{
    if ( Py_FinalizeEx() )
    {
        output_warning("Py_FinalizeEx() failed");
    }
    PyMem_RawFree((void*)program);
    output_verbose("python shutdown ok");
}

PyObject *python_embed_import(const char *module, const char *path)
{
    char tmp[1024] = ".";
    if ( path != NULL )
    {
        snprintf(tmp,sizeof(tmp)-1,"import sys\nsys.path.append('%s')\n",path);
    }
    if ( PyRun_SimpleString(tmp) )
    {
        PyObject *pType, *pValue, *pTraceback;
        PyErr_Fetch(&pType, &pValue, &pTraceback);
        const char *msg = pValue?PyBytes_AS_STRING(pValue):"PyRun_SimpleString failed with no information";
        output_error("python_embed_import(module='%s',path='%s'): %s; string='%s'",module,path,msg,tmp);
        return NULL;
    }

    PyObject *pModule = PyImport_ImportModule(module);
    if ( pModule == NULL )
    { 
        PyObject *pType, *pValue, *pTraceback;
        PyErr_Fetch(&pType, &pValue, &pTraceback);
        const char *msg = pValue?PyBytes_AS_STRING(pValue):"PyImport_ImportModule failed with no information";
        output_error("python_embed_import(module='%s',path='%s'): %s",module,path,msg);
        return NULL;
    }
    else
    {
        return pModule;
    }
}

bool python_embed_call(PyObject *pModule, const char *name)
{
    if ( pModule == NULL )
    {
        output_error("python_embed_call(pModule,name='%s'): no module loaded",name);
        return false;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule,name);
    if ( pFunc == NULL )
    {
        PyErr_Print();
        output_error("python_embed_call(pModule,name='%s'): function not defined",name);
        return false;
    }
    if ( ! PyCallable_Check(pFunc) )
    {
        PyErr_Print();
        output_error("python_embed_call(name='%s'): function not callable",name);
        Py_DECREF(pFunc);
        return false;
    }

    PyObject *pResult = PyObject_CallObject(pFunc,NULL);
    if ( PyErr_Occurred() )
    {
        PyErr_Print();
    }
    if ( pResult != NULL )
    {
        Py_DECREF(pResult);
    }
    Py_DECREF(pFunc);
    return true;
}