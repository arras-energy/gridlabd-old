// python.cpp

#include "gldcore.h"
#include <string.h>

wchar_t *program = NULL;
PyObject *main_module = NULL;

void python_embed_init(int argc, const char *argv[])
{
    program = Py_DecodeLocale(argv[0],NULL);
    Py_SetProgramName(program);
    Py_Initialize();
    main_module = PyModule_GetDict(PyImport_AddModule("__main__"));
    if ( main_module == NULL )
    {
        output_error("python_embed_init(argc=%d,argv=[...]: unable to load module __main__ module",argc);
        throw "python exception";
    }
    python_parser("import " PACKAGE);
    python_parser();
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
    if ( path != NULL )
    {
        char tmp[1024];
        int len = snprintf(tmp,sizeof(tmp)-1,"import sys");
        if ( path != NULL )
        {
            snprintf(tmp+len,sizeof(tmp)-len-1,"\nsys.path.append('%s')\n",path);
        }
        if ( PyRun_SimpleString(tmp) )
        {
            PyObject *pType, *pValue, *pTraceback;
            PyErr_Fetch(&pType, &pValue, &pTraceback);
            const char *msg = pValue?PyBytes_AS_STRING(pValue):"PyRun_SimpleString failed with no information";
            output_error("python_embed_import(module='%s',path='%s'): %s; string='%s'",module,path,msg,tmp);
            return NULL;
        }
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

const char *truncate(const char *command)
{
    while ( isspace(*command) ) command++;
    static char buf[20];
    memset(buf,0,sizeof(buf));
    strncpy(buf,command,sizeof(buf));
    char *eol = strchr(buf,'\n');
    if ( eol ) *eol = '\0';
    if ( buf[sizeof(buf)-1] != '\0' ) 
    {
        strcpy(buf+sizeof(buf)-4,"...");
    }
    return buf;
}

bool python_embed_call(PyObject *pModule, const char *name)
{
    if ( pModule == NULL )
    {
        output_error("python_embed_call(pModule,name='%s'): no module loaded",truncate(name));
        return false;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule,name);
    if ( pFunc == NULL )
    {
        PyErr_Print();
        output_error("python_embed_call(pModule,name='%s'): function not defined",truncate(name));
        return false;
    }
    if ( ! PyCallable_Check(pFunc) )
    {
        PyErr_Print();
        output_error("python_embed_call(name='%s'): function not callable",truncate(name));
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

void traceback(const char *command)
{
    PyObject *err = PyErr_Occurred();
    if ( err == NULL )
    {
        output_error("python_eval(command='%s'): no error occurred",command);
        return;
    }
    PyObject *pyth_val, *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    Py_DECREF(err);
    if ( ptype == NULL || pvalue == NULL || ptraceback == NULL )
    {
        output_error("python_eval(command='%s'): no error to fetch",command);
        return;
    }
    PyObject *pystr = PyUnicode_AsEncodedString(pvalue,"utf-8","~E~");
    char *str = pystr ? PyBytes_AsString(pystr) : NULL;
    if ( str )
    {
        output_error("python_eval(command='%s'): python exception caught: %s",command, str);
    }
    PyObject *pyth_module = PyImport_ImportModule("traceback");
    if ( pyth_module == NULL ) 
    {
        output_error("python_eval(command='%s'): no traceback module available",command);
    }
    else
    {
        PyObject *pyth_func = PyObject_GetAttrString(pyth_module, "format_exception");
        if ( pyth_func == NULL )
        {
            output_error("python_eval(command='%s'): traceback.format_exception() not found",command);
        }
        else if ( ! PyCallable_Check(pyth_func) )
        {
            output_error("python_eval(command='%s'): traceback.format_exception() not callable",command);
        }
        else if ( (pyth_val = PyObject_CallFunctionObjArgs(pyth_func, ptype, pvalue, ptraceback, NULL)) != NULL ) 
        {
            PyObject *pystr = PyUnicode_AsEncodedString(pyth_val,"utf-8","~E~");
            const char *str = pystr ? PyBytes_AsString(pystr) : NULL;
            if ( str )
            {
                output_error("python_eval(command='%s'): traceback follows",command);
                output_error_raw("BEGIN TRACEBACK\n%s\nEND TRACEBACK",command,str);
            }
            else
            {
                output_error("python_eval(command='%s'): no traceback found",command);
            }
            Py_DECREF(pyth_val);
        }
        else
        {
            output_error("python_eval(command='%s'): unable to format traceback, arguments follow",command);
            FILE *error = output_get_stream("error");
            output_error_raw("BEGIN TRACEBACK");
            fprintf(error,"function: "); PyObject_Print(pyth_func,error,Py_PRINT_RAW); fprintf(error,"\n");
            fprintf(error,"type: "); PyObject_Print(ptype,error,Py_PRINT_RAW); fprintf(error,"\n");
            fprintf(error,"value: "); PyObject_Print(pvalue,error,Py_PRINT_RAW); fprintf(error,"\n");
            fprintf(error,"traceback: "); PyObject_Print(ptraceback,error,Py_PRINT_RAW); fprintf(error,"\n");
            output_error_raw("END TRACEBACK");
        }
    }
}

std::string python_eval(const char *command)
{
    PyObject *result = PyRun_String(command,Py_eval_input,main_module,main_module);
    if ( result == NULL )
    {
        output_error("python_eval(command='%s') failed",truncate(command));
        traceback(truncate(command));
        throw "python exception";
    }
    PyObject *repr = PyObject_Repr(result);
    PyObject *str = PyUnicode_AsEncodedString(repr,"utf-8","~E~");
    std::string bytes(PyBytes_AS_STRING(str));
    Py_XDECREF(repr);
    Py_XDECREF(str);
    Py_XDECREF(result);
    return bytes;
}

// Parser implementation
//   python_parse(<string>) to append <string> to input buffer
//   python_parser(NULL) to parse input buffer
// Returns true on success, false on failure
static std::string input_buffer("");
bool python_parser(const char *line)
{
    // end input -> run code
    if ( line != NULL )
    {
        input_buffer.append(line);
        return true;
    }

    const char *command = input_buffer.c_str();
    PyObject *result = PyRun_String(command,Py_file_input,main_module,main_module);
    if ( result == NULL )
    {
        output_error("python_embed_call(parser_module='gridlabd',command='%s') failed",truncate(command));
        traceback(truncate(command));
        input_buffer = "";
        return false;
    }
    else
    {
        Py_XDECREF(result);
        input_buffer = "";
        return true;
    }
}
