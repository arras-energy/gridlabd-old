// commercial/parking.h

#include "gridlabd.h"

class parking : public gld_object 
{

public:
    static double default_nightlight_threshold;

public:

    GL_ATOMIC(object,weather);
    GL_ATOMIC(bool,lighting_nightonly);
    GL_ATOMIC(int16,charger_installed);
    GL_ATOMIC(int16,charger_active);
    GL_ATOMIC(complex,charger_unit_power);
    GL_ATOMIC(complex,lighting_power);
    GL_ATOMIC(complex,ventilation_power);
    GL_ATOMIC(complex,charger_power);
    GL_ATOMIC(complex,total_power);

public:

    /* required implementations */
    parking(MODULE *module);
    ~parking();

    int create(void);
    int init(OBJECT *parent);
    TIMESTAMP precommit(TIMESTAMP t1);
    TIMESTAMP presync(TIMESTAMP t1);
    TIMESTAMP sync(TIMESTAMP t1);
    TIMESTAMP postsync(TIMESTAMP t1);
    TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

private:

    /* pointers to load variables */
    double *p_power_A;
    double *p_power_B;
    double *p_power_C;
public:
    
    static CLASS *oclass;
    static parking *defaults;
};
