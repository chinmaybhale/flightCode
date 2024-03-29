#ifndef __HEADERS_H__
#define __HEADERS_H__
//global headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*****************************/
// local headers
#include "macros.h"
#include "init.h"
#include "seq.h"
#include "daq_interface.h"
/*****************************/

// Global variables
// used for storing values of pressure, temp, etc
// declared here, initialized in init

// max and min values decided by percentage when initializing
struct sensor {
	float max_val;
	float min_val;
	short pin; // not all sensors will have this, set to -1
};

struct valve {
	short pin;
	short stat; // 0 = off, 1 = on
};

//struct daq_value {
//	// TODO:
//	// we need a specific data structure to store data from DAQ
//	// let's define this once we get a better idea with what
//	// we are dealing with.
//};

typedef float daq_value; // typedef'ing for easy reference

struct sensor s[SENSOR_NUM]; // for pressure values
struct valve v[VALVE_NUM]; // valve array
daq_value values[SENSOR_NUM]; // for values received from DAQ / DAQ debug

int debug;
int verbose;

#endif
