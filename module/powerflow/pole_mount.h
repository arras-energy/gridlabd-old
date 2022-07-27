// module/powerflow/pole.h
// Copyright (C) 2018, Regents of the Leland Stanford Junior University

#ifndef _POLEMOUNT_H
#define _POLEMOUNT_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

class pole_mount : public gld_object
{
public:
	static CLASS *oclass;
	static CLASS *pclass;
    static pole_mount *defaults;
public:
    GL_ATOMIC(object,equipment);
    GL_ATOMIC(double,height);
    GL_ATOMIC(double,offset);
    GL_ATOMIC(double,offset_directio);
    GL_ATOMIC(double,direction);
    GL_ATOMIC(double,weight);
    GL_ATOMIC(double,area);
    GL_ATOMIC(double,pole_spacing);
    GL_ATOMIC(double, wind_speed);
    GL_ATOMIC(double, wind_direction);
private:
    double equipment_moment_weight;
    double equipment_moment_wind;
    double equipment_moment_nowind;
    double equipment_load_nowind;
    gld_property *pole_status;
    gld_property *equipment_status;
    bool equipment_is_line;
    double strength;
    double line_moment_weight;
    double line_moment_wind;
    double line_moment_nowind;
    double line_moment_tension;
    double line_load_nowind;
    // double equipment_x_tilt;
    // double equipment_y_tilt;
    // double equipment_z_tilt;
    double equipment_offset;
public:
	pole_mount(MODULE *);
	int create(void);
	int init(OBJECT *);
    TIMESTAMP precommit(TIMESTAMP);
    TIMESTAMP presync(TIMESTAMP);
    TIMESTAMP sync(TIMESTAMP);
	TIMESTAMP postsync(TIMESTAMP);
    TIMESTAMP commit(TIMESTAMP,TIMESTAMP);
};

#endif // _POLEMOUNT_H
