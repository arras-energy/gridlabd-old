// python.h

#ifndef _PYTHON_H
#define _PYTHON_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <Python.h>

void python_embed_init(int argc, const char *argv[]);
void *python_loader_init(int argc, const char **argv);
void python_embed_term();
PyObject *python_embed_import(const char *module, const char *path=NULL);
bool python_embed_call(PyObject *pModule, const char *name, const char *vargsfmt, va_list varargs);
std::string python_eval(const char *command);
bool python_parser(const char *line=NULL, void *context = NULL);

// forward declarations to gldcore/link/python.c
MODULE *python_module_load(const char *, int, const char *[]);

// Function: convert_from_double
DEPRECATED int convert_from_python(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_double
DEPRECATED int convert_to_python(const char *buffer, void *data, PROPERTY *prop);

DEPRECATED int initial_from_python(char *buffer, int size, void *data, PROPERTY *prop);

DEPRECATED int python_create(void *ptr);

double python_get_part(void *c, const char *name);


#endif
