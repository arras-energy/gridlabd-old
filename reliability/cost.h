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

typedef struct s_cost COST;

struct s_cost {
    const char *name; // cost name
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
