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
#include "uldaq.h"
#include "daq_utility.h"
/*****************************/

// Global variables
// used for storing values of pressure, temp, etc
// declared here, initialized in init

// max and min values decided by percentage when initializing
struct sensor {
	float base_val;
	int pos_val_err;
	int neg_val_err;

	float max_val;
	float min_val;

	float base_trend;
	int pos_trend_err;
	int neg_trend_err;

	float max_trend;
	float min_trend;
	
	short pin; // not all sensors will have this, set to -1
};

struct valve {
	short pin;
	short feedback;
	short stat; // 0 = off, 1 = on
};

struct daq_value {
	float curr;
	float prev;
	float trend;
};


struct sensor s[SENSOR_COUNT]; // for pressure values
struct valve v[VALVE_COUNT]; // valve array
struct sensor verified_data[SENSOR_COUNT]; // for verified/expected sensor data
struct daq_value daq_val[SENSOR_COUNT]; // for values received from DAQ / DAQ debug

int debug;
int verbose;

#endif
