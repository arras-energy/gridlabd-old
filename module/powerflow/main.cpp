// $Id: main.cpp 4738 2014-07-03 00:55:39Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#define DLMAIN

#include <stdlib.h>
#include "gridlabd.h"
#include "powerflow.h"

EXPORT TIMESTAMP on_precommit(TIMESTAMP t1)
{
    powerflow_object::clear_violations();
    return TS_NEVER;
}