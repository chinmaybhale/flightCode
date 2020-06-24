/*
* Helios Rocketry AFCP/include
* headers.h- Header file including all the 'header' files in the program as well as class definitions for the main sequence
* and global variables for 'debug' and 'verbose'
*
* NOTE: Any updates or additions in 'AFCP/include' must be updated here accordingly
* NOTE: Any changes in the format and/or information in the config file must be reflected here as well
*/
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
#ifndef __HEADERS_H__
#define __HEADERS_H__

// Global headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Local headers
#include "macros.h"
#include "init.h"
#include "seq.h"
#include "daq_interface.h"
#include "uldaq.h"
#include "daq_utility.h"
//---------------------------------------------------------------------------------------------------------------------------

// Class declarations for global variables of the program
// BE ADVISED: max and min values decided by percentage when initializing
struct sensor
{										 // TRANSDUCERS
	float base_val;	 					 // Estimated value
	int pos_val_err;					 // Error  margins
	int neg_val_err;

	float max_val;
	float min_val;

	float base_trend;
	int pos_trend_err;
	int neg_trend_err;

	float max_trend;
	float min_trend;

	short pin;                             // not all sensors will have this, set to -1
};


struct valve 
{                       // VALVES (Electric & Solenoid)
	short pin;            // Connection GPIO pin on the Main computer
	short feedback;       // Feedback from microcontroller to Main computer
	short stat;           // 0 = off, 1 = on
};

struct daq_value
{
	float curr;
	float prev;
	float trend;
};
//---------------------------------------------------------------------------------------------------------------------------

struct sensor s[SENSOR_COUNT];			   // Pressure data
struct valve v[VALVE_COUNT];			   // Valve array
struct sensor verified_data[SENSOR_COUNT]; // For verified/expected sensor data
struct daq_value daq_val[SENSOR_COUNT];	   // For values received from DAQ / DAQ debug

int debug;
int verbose;

#endif
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
