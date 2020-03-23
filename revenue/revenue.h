// File: revenue.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _REVENUE_H

#define _REVENUE_H

#include "gridlabd.h"

#ifdef DLMAIN
#define INIT(X) = X
#define GLOBAL
#else
#define INIT(X)
#define GLOBAL extern
#endif

// TODO: declare module globals here
// GLOBAL {{{global1_type}}} {{{global1_name}}} INIT({{{global1_init}}});

#include "billing.h"

#endif // _REVENUE_H