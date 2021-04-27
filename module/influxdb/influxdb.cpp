// influxdb.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#define DLMAIN

#include "influxdb.h"

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
    if (set_callback(fntable)==NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    INIT_MMF(influxdb);

    database *first = new database(module);
    new recorder(module);

    return first->oclass;
}

EXPORT void term(void)
{
    // database *db;
    // for ( db=database::get_first() ; db!=NULL ; db=db->get_next() )
    //     db->term();
    // mysql_library_end();
}

EXPORT int do_kill(void*)
{
    /* if global memory needs to be released, this is a good time to do it */
    return 0;
}

