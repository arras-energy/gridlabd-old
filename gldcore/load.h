// File: load.h 
// Copyright (C) 2008, Battelle Memorial Institute
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#ifndef _LOAD_H
#define _LOAD_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"
#include "module.h"

// Define: UR_NONE
// Specifies no unresolved reference flags are 
#define UR_NONE  0x00     

// Define: UR_RANKS
// Specifies unresolved reference has ranking impact
#define UR_RANKS 0x01

// Define: UR_TRANSFORM
// Specifies reference is via a transform
#define UR_TRANSFORM 0x02 

// Function: loadall
// Loads the contents of a file into the current instance
// Returns: STATUS
DEPRECATED STATUS loadall(const char *filename);

// Function: load_get_current_object
// Obtains the object currently being processed by the loaded
// Returns: OBJECT*
DEPRECATED OBJECT *load_get_current_object(void);

// Function: load_get_current_module
// Obtains the module current being processed by the loaded
// Returns: MODULE*
DEPRECATED MODULE *load_get_current_module(void);

// Class: GldLoader
// Implements the GLM parser
class GldLoader
{

private:

	// Field: instance
	// Reference to the GldMain simulation instance
	GldMain &instance;

public:

	// Constructor: GldLoader
	// Constructs a loader for a simulation instance
	GldLoader(GldMain *main);

	// Destructor: ~GldLoader
	// Destroys a loader for a simulation instance
	~GldLoader(void);

public:

	// Method: loadall
	// Loads a file into the loader's instance
	bool loadall(const char *filename);

	// Method: get_current_object
	// Obtains the object currently being processed by the loader
	OBJECT *get_current_object(void);

	// Method: get_current_module
	// Obtains the module currently being processed b the loader
	MODULE *get_current_module(void);
};

#endif
