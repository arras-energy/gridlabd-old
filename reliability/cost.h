// File: relability/cost.h
// Author: dchassin@slac.stanford.edu
// Copyright: 2020 Regents of the Leland Stanford Junior University
//
// Cost impacts are based on Mark Burlingame and Patty Walton, "NARUC AND MDPSC 
// COST-BENEFIT ANALYSIS OF VARIOUS ELECTRIC RELIABILITY IMPROVEMENT PROJECTS 
// FROM THE END USERS' PERSPECTIVE", November 15, 2013 Table 4 (page 21)

#ifndef _COST_H
#define _COST_H

#include "gridlabd.h"

#define CT_FOOD        ((set)0x0001)
#define CT_WATER       ((set)0x0002)
#define CT_SEPTIC      ((set)0x0004)
#define CT_GEN_BASIC   ((set)0x0008)
#define CT_GEN_COMFORT ((set)0x0010)
#define CT_GEN_ALL     ((set)0x0020)
#define CT_LODGING     ((set)0x0040)
#define CT_BUSINESS    ((set)0x0080)
#define CT_ELDERCARE   ((set)0x0100)
#define CT_HEATING     ((set)0x0200)
#define CT_COOKING     ((set)0x0400)
#define CT_LIGHTS      ((set)0x0800)
#define CT_WAGES       ((set)0x1000)

typedef struct s_cost COST;

struct s_cost {
    const char *name; // cost name
    set type; // cost type
    double delay; // time delay before cost is incurred
    double fixed; // fixed cost $
    double variable; // variable cost ($/h)
};

class cost : public gld_object
{

    bool *customer_interrupted;
    TIMESTAMP interrupted_at;

public:

    GL_ATOMIC(double,total);
    GL_ATOMIC(set,type);

public:

    cost(MODULE *module);
    int create(void);
    int init(OBJECT *parent);
    TIMESTAMP commit(TIMESTAMP t0, TIMESTAMP t1);

public:

    static CLASS *oclass;
    static cost *defaults;
};

#endif // _COST_H
