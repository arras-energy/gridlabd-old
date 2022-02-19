// module/resilience/resilience.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _GRIDLABD_MODULE_RESILIENCE_H
#define _GRIDLABD_MODULE_RESILIENCE_H

#include "gridlabd.h"

typedef struct s_asset
{
    OBJECT *obj;
    TIMESTAMP start;
    set *status;
    struct s_asset *next;
} ASSET;

#include "metrics.h"
#include "outage.h"
#include "cost.h"
#include "impact.h"

#endif
