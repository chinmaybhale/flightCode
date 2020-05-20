// HELIOS ROCKETRY
// This file contains the code for interfacing with the daq
#include "headers.h"

FILE *test_data;

static void convert()
{
	// TODO: convert values from 
	// Volts -> PSI/C/other format
	
	return;
}

static void read_data()
{
	// TODO: read code from DAQ here
	
	return;
}

static void read_file()
{
	// TODO: read one line, fill the 
	// data structure

	return;
}

void init_file()
{
	// TODO: read the data in a CSV format when
	// running in debug mode
	
	test_data = fopen("test_data.csv", "wr");

	if(!test_data) {
		printf("not able to open file\n");
		return;
	}

	return;
}

void get_data()
{
	if(debug) {
		read_file();
	}
	else {

		read_data();
		convert();
	}

	fclose(test_data);
	return;
}
