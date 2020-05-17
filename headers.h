#ifndef __HEADERS_H__
#define __HEADERS_H__
//global headers
#include <stdio.h>

/*****************************/
// local headers
#include "macros.h"
#include "init.h"
#include "seq.h"

// Global variables
// used for storing values of pressure, temp, etc
// declared here, initialized in init

// max and min values decided by percentage when initializing
struct sensors {
	float max_val;
	float min_val;
	short pin;
};

struct sensors p[8]; // for pressure values
struct sensors t[5]; // for temperature values

typedef short valves;

valves main_v[7];
valves backup_v[5];

#endif
