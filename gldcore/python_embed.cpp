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

std::string python_eval(const char *command)
{
    output_debug("python_eval(const char *command='%s')...",command);
    PyObject *result = PyRun_String(command,Py_eval_input,Py_None,Py_None);
    if ( result == NULL )
    {
        output_error("python_eval(command='%s'): python exception caught",command);
        PyObject *err = PyErr_Occurred();
        if ( err != NULL ) 
        {
            PyObject *ptype, *pvalue, *ptraceback;
            PyErr_Fetch(&ptype, &pvalue, &ptraceback);
            PyObject *pystr = PyUnicode_AsEncodedString(pvalue,"utf-8","~E~");
            char *str = pystr ? PyBytes_AsString(pystr) : NULL;
            if ( str )
            {
                output_error("python_eval(command='%s'): %s",command, str);
            }

            /* See if we can get a full traceback */
            PyObject *pyth_module = PyImport_ImportModule("traceback");
            if ( pyth_module == NULL ) 
            {
                output_error("python_eval(command='%s'): no traceback module available",command);
            }
            else
            {
                PyObject *pyth_val, *pyth_func = PyObject_GetAttrString(pyth_module, "format_exception");
                if ( pyth_func && PyCallable_Check(pyth_func) && (pyth_val = PyObject_CallFunctionObjArgs(pyth_func, ptype, pvalue, ptraceback, NULL)) != NULL ) 
                {
                    pystr = PyUnicode_AsEncodedString(pyth_val,"utf-8","~E~");
                    str = pystr ? PyBytes_AsString(pystr) : NULL;
                    if ( str )
                    {
                        output_error("python_eval(command='%s'): traceback follows",command);
                        output_error("  %s",command,str);
                    }
                    else
                    {
                        output_error("python_eval(command='%s'): traceback not available",command);
                    }
                    Py_DECREF(pyth_val);
                }
                else
                {
                    output_error("python_eval(command='%s'): unable to read traceback",command);
                }
            }
            Py_DECREF(err);
        }
        return std::string("");
    }
    else
    {
        PyObject *repr = PyObject_Repr(result);
        PyObject *str = PyUnicode_AsEncodedString(repr,"utf-8","~E~");
        std::string bytes(PyBytes_AS_STRING(str));
        Py_XDECREF(repr);
        Py_XDECREF(str);
        Py_XDECREF(result);
        return bytes;
    }
}