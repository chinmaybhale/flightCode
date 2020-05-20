// HELIOS ROCKETRY
// Code here will only run once in the start
// Think setup() from arduino

#include "headers.h"

static void read_config(char *name)
{
	// CSV FORMAT
	// sensor_name, base_val, +ve error, -ve error, pin# (if availabe, -1 otherwise)
	// or
	// valve_name, pin#
	//
	// sensor_name will decide if it is a pressure or temperature or other kind of sensor.

	return;
}

static void init_sensor(char *setup)
{
	// TODO: split the string along the comma
	// and convert the values to float
	// use stof() for this.
	
	return;
}

static void init_valve(char *setup)
{
	// TODO: convert the pin number to int
	// using stoi() and assign to valve
	// set open to 0 (off)	
	
	return;
}

static void system_check()
{
	return;
}

void init(char *name)
{
	read_config(name);
	system_check();
	if(debug)
		init_file();	
	return;
}
