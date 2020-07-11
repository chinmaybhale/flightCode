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
#include <time.h>

// Local headers
#include "macros.h"
#include "init.h"
#include "seq.h"
#include "daq_interface.h"
#include "uldaq.h"
#include "daq_utility.h"
#include "verified_reader.h"
//---------------------------------------------------------------------------------------------------------------------------

// Struct declarations for global variables of the program
// BE ADVISED: max and min values decided by percentage when initializing
struct sensor
{				// TRANSDUCERS
	float base_val;	 	// Estimated value
	int pos_val_err;	// Positive error margins
	int neg_val_err; 	// Negative error margin

	float max_val; 		// Maximum estimated value				
	float min_val; 		// Minimum estimated value

	float base_trend; 	// Estimated trend
	int pos_trend_err; 	// Positive error margin
	int neg_trend_err; 	// Negative error margin

	float max_trend; 	// Maximum estimated trend
	float min_trend; 	// Minimum estimated trend

	short pin;              // not all sensors will have this, set to -1
};

struct valve 
{                       	// VALVES (Electric & Solenoid)
	short pin;            	// Connection GPIO pin on the Main computer
	short feedback; 	// Feedback from microcontroller to Main computer
	short fb_pin; 		// Pin for reading feedback
	short stat;           	// 0 = off, 1 = on
};

struct daq_value
{
	float curr; 		// Current sensor value
	float prev; 		// Previous sensor value
	float trend; 		// Sensor value trend
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
