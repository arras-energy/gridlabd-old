// python.cpp

#include "gldcore.h"
#include <string.h>

wchar_t *program = NULL;
PyObject *main_module = NULL;
PyObject *gridlabd_module = NULL;

PyMODINIT_FUNC PyInit_gridlabd(void);

void python_embed_init(int argc, const char *argv[])
{
    program = Py_DecodeLocale(argv[0],NULL);
    Py_SetProgramName(program);
    Py_Initialize();
    main_module = PyModule_GetDict(PyImport_AddModule("__main__"));
    if ( main_module == NULL )
    {
        throw_exception("python_embed_init(argc=%d,argv=(%s,...)): unable to load module __main__ module",argc,argv?argv[0]:"NULL");
    }

    extern PyObject *this_module; // from gldcore/link/python.cpp
    if ( this_module == NULL )
    {
        PyInit_gridlabd();
    }
    gridlabd_module = this_module;
}

void *python_loader_init(int argc, const char **argv)
{
    if ( main_module == NULL )
    {
        python_embed_init(argc,argv);
    }
    return main_module;
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

void python_reset_stream(PyObject *pModule, const char *stream_name)
{
    PyObject *pio = PyImport_ImportModule("io");
    PyObject *pStringIO = pio ? PyObject_GetAttrString(pio,"StringIO") : NULL;
    PyErr_Clear();
    PyObject *pReset = pStringIO ? PyObject_CallObject(pStringIO,NULL) : NULL;
    if ( pReset )
    {
        PyObject_SetAttrString(pModule,stream_name,pReset);
        Py_DECREF(pReset);
    }
    if ( pio ) Py_DECREF(pio);
    if ( pStringIO ) Py_DECREF(pStringIO);
}

PyObject *python_embed_import(const char *module, const char *path)
{
    // fix module search path
    char tmp[1024];
    if ( path != NULL )
    {
        int len = snprintf(tmp,sizeof(tmp)-1,"import io, sys\nsys.path.extend('%s'.split(':'))\n",path);
        output_debug("python_embed_import(const char *module='%s', const char *path='%s'): running [%s]",module,path,tmp);
        if ( len > 0 && PyRun_SimpleString(tmp) )
        {
            PyObject *pType, *pValue, *pTraceback;
            PyErr_Fetch(&pType, &pValue, &pTraceback);
            PyObject *repr = pValue ? PyObject_Repr(pValue) : NULL;
            PyObject *bytes = repr ? PyUnicode_AsEncodedString(repr, "utf-8", "~E~") : NULL;
            const char *msg = bytes?PyBytes_AS_STRING(bytes):"PyRun_SimpleString failed with no information";
            output_error("python_embed_import(module='%s',path='%s'): %s; string='%s'",module,path,msg,tmp);
            if ( repr ) Py_XDECREF(repr);
            if ( bytes ) Py_XDECREF(bytes);
            return NULL;
        }
    }

    // clean up module name for use by import
    char basename[1024];
    strcpy(basename,module);
    char *ext = strrchr(basename,'.');
    if ( ext != NULL && strcmp(ext,".py") == 0 )
    {
        *ext = '\0';
    }
    char *dir = strrchr(basename,'/');

    // import module
    PyObject *pModule = PyImport_ImportModule(dir?dir+1:basename);
    if ( pModule == NULL )
    { 
        PyObject *pType, *pValue, *pTraceback;
        PyErr_Fetch(&pType, &pValue, &pTraceback);
        PyObject *repr = pValue ? PyObject_Repr(pValue) : NULL;
        PyObject *bytes = repr ? PyUnicode_AsEncodedString(repr, "utf-8", "~E~") : NULL;
        const char *msg = bytes?PyBytes_AS_STRING(bytes):"PyImport_ImportModule failed with no information";
        output_error("python_embed_import(module='%s',path='%s'): %s",module,path,msg);
        if ( repr ) Py_XDECREF(repr);
        if ( bytes ) Py_XDECREF(bytes);
        return NULL;
    }
    python_reset_stream(pModule,"error_stream");
    python_reset_stream(pModule,"output_stream");

    return pModule;
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

void traceback(const char *command)
{
    PyObject *err = PyErr_Occurred();
    if ( err == NULL )
    {
        output_error("traceback(command='%s'): no error occurred",command);
        return;
    }
    PyObject *pyth_val, *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    Py_DECREF(err);
    if ( ptype == NULL || pvalue == NULL || ptraceback == NULL )
    {
        output_error("traceback(command='%s'): no error to fetch",command);
        return;
    }
    PyObject *pystr = PyUnicode_AsEncodedString(pvalue,"utf-8","~E~");
    char *str = pystr ? PyBytes_AsString(pystr) : NULL;
    if ( str )
    {
        output_error("traceback(command='%s'): python exception caught: %s",command, str);
    }
    PyObject *pyth_module = PyImport_ImportModule("traceback");
    if ( pyth_module == NULL ) 
    {
        output_error("traceback(command='%s'): no traceback module available",command);
    }
    else
    {
        PyObject *pyth_func = PyObject_GetAttrString(pyth_module, "format_exception");
        PyErr_Clear();
        if ( pyth_func == NULL )
        {
            output_error("traceback(command='%s'): traceback.format_exception() not found",command);
        }
        else if ( ! PyCallable_Check(pyth_func) )
        {
            output_error("traceback(command='%s'): traceback.format_exception() not callable",command);
        }
        else if ( (pyth_val = PyObject_CallFunctionObjArgs(pyth_func, ptype, pvalue, ptraceback, NULL)) != NULL ) 
        {
            PyObject *pystr = PyUnicode_AsEncodedString(pyth_val,"utf-8","~E~");
            const char *str = pystr ? PyBytes_AsString(pystr) : NULL;
            if ( str )
            {
                output_error("traceback(command='%s'): traceback follows",command);
                output_error_raw("BEGIN TRACEBACK\n%s\nEND TRACEBACK",command,str);
            }
            else
            {
                output_error("traceback(command='%s'): no traceback found",command);
            }
            Py_DECREF(pyth_val);
        }
        else
        {
            output_error("traceback(command='%s'): unable to format traceback, arguments follow",command);
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

// Function: python_embed_call
// 
// If pModule is dict and name is null, the varargsfmt is used to store the
// the result of the last call, i.e., pModule[varargsfmt] = result
bool python_embed_call(
    PyObject *pModule, 
    const char *name, 
    const char *vargsfmt, 
    va_list varargs,
    void *result)
{
    PyObject **pResult = (PyObject **)result;
    if ( name == NULL ) 
    {
        output_error("python_embed_call(pModule,name): name is null");
        return false;
    }
    
    if ( pModule == NULL )
    {
        output_error("python_embed_call(pModule,name='%s'): no module loaded",truncate(name));
        return false;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule,name);
    if ( pFunc == NULL )
    {
        PyObject *pType, *pValue, *pTraceback;
        PyErr_Fetch(&pType, &pValue, &pTraceback);
        PyObject *repr = pValue ? PyObject_Repr(pValue) : NULL;
        PyObject *bytes = repr ? PyUnicode_AsEncodedString(repr, "utf-8", "~E~") : NULL;
        const char *msg = bytes?PyBytes_AS_STRING(bytes):"function not defined";
        output_error("python_embed_call(pModule,name='%s'): %s ",truncate(name), msg);
        if ( repr ) Py_XDECREF(repr);
        if ( bytes ) Py_XDECREF(bytes);
        return false;
    }
    if ( ! PyCallable_Check(pFunc) )
    {
        PyObject *pType, *pValue, *pTraceback;
        PyErr_Fetch(&pType, &pValue, &pTraceback);
        PyObject *repr = pValue ? PyObject_Repr(pValue) : NULL;
        PyObject *bytes = repr ? PyUnicode_AsEncodedString(repr, "utf-8", "~E~") : NULL;
        const char *msg = bytes?PyBytes_AS_STRING(bytes):"function not callable";
        output_error("python_embed_call(pModule,name='%s'): %s ",truncate(name), msg);
        if ( repr ) Py_XDECREF(repr);
        if ( bytes ) Py_XDECREF(bytes);
        Py_DECREF(pFunc);
        return false;
    }

    PyObject *pGridlabd = PyDict_GetItemString(pModule,"gridlabd");
    PyObject *pArgs = Py_BuildValue("(O)",pGridlabd?pGridlabd:PyInit_gridlabd());
    PyObject *pKwargs = vargsfmt ? Py_VaBuildValue(vargsfmt,varargs) : NULL;
    PyErr_Clear();
    PyObject *return_value = PyObject_Call(pFunc,pArgs,pKwargs);
    if ( pGridlabd ) Py_DECREF(pGridlabd);
    Py_DECREF(pArgs);
    if ( PyErr_Occurred() )
    {
        traceback(name);
        return false;
    }
    assert(return_value!=NULL);
    PyObject *pError = PyObject_GetAttrString(pModule,"error_stream");
    if ( pError )
    {
        PyObject *pCall = PyObject_GetAttrString(pError,"getvalue");
        PyErr_Clear();
        PyObject *pValue = pCall && PyCallable_Check(pCall) ? PyObject_CallObject(pCall,NULL) : NULL;
        PyObject *pBytes = pValue && PyUnicode_Check(pValue) ? PyUnicode_AsEncodedString(pValue, "utf-8", "~E~") : NULL;
        const char *msg = pBytes ? PyBytes_AS_STRING(pBytes): NULL;
        if ( strcmp(msg,"") != 0 )
        {
            output_error("%s: %s", name, msg ? msg : "(python error_stream not available");
        }
        if ( pCall ) Py_DECREF(pCall);
        if ( pValue ) Py_DECREF(pValue);
        if ( pBytes ) Py_DECREF(pBytes);
        Py_DECREF(pError);
        python_reset_stream(pModule,"error_stream");
    }
    PyObject *pOutput = PyObject_GetAttrString(pModule,"output_stream");
    if ( pOutput )
    {
        PyObject *pCall = PyObject_GetAttrString(pOutput,"getvalue");
        PyErr_Clear();
        PyObject *pValue = pCall && PyCallable_Check(pCall) ? PyObject_CallObject(pCall,NULL) : NULL;
        PyObject *pBytes = pValue && PyUnicode_Check(pValue) ? PyUnicode_AsEncodedString(pValue, "utf-8", "~E~") : NULL;
        const char *msg = pBytes ? PyBytes_AS_STRING(pBytes): NULL;
        if ( strcmp(msg,"") != 0 )
        {
            output_raw("%s", msg ? msg : "(python output_stream not available)");
        }
        if ( pCall ) Py_DECREF(pCall);
        if ( pValue ) Py_DECREF(pValue);
        if ( pBytes ) Py_DECREF(pBytes);
        Py_DECREF(pOutput);
        python_reset_stream(pModule,"output_stream");
    }
    Py_DECREF(pFunc);
    if ( pKwargs ) 
    {
        Py_DECREF(pKwargs);
    }
    if ( pResult != NULL )
    {
        *pResult = return_value;
    }
    return true;
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
static std::string input_buffer("from gridlabd import *\n");
bool python_parser(const char *line, void *context)
{
    // end input -> run code
    if ( line != NULL )
    {
        input_buffer.append(line);
        return true;
    }

    PyObject *module = (PyObject*)context;
    if ( module == NULL )
    {
        module = main_module;
    }

    const char *command = input_buffer.c_str();
    PyObject *result = PyRun_String(command,Py_file_input,module,module);
    if ( result == NULL )
    {
        output_error("python_parser(NULL,...): command '%s' failed",command);
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
