// python.h

#ifndef _PYTHON_H
#define _PYTHON_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <Python.h>

void python_embed_init(int argc, const char *argv[]);
void python_embed_term();
PyObject *python_embed_import(const char *module, const char *path=NULL);
bool python_embed_call(PyObject *pModule, const char *name);

#endif
