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

static void read_line(char *line, size_t len)
{
	char *str_val = (char *)malloc(sizeof(char) * 10);
	int curr, prev = 0, i = 0;

	for(curr = 0; curr <= len; curr++) {
		if(line[curr] == ',') {
			strncpy(str_val, line + prev , (curr - prev));
			str_val[curr - prev] = '\0';
			values[i++] = atof(str_val);			
			prev = curr + 1;	
		}
	}

	free(str_val);
	return;
}

static void read_file()
{
	// TODO: read one line, fill the 
	// data structure
	// CSV format
	// sensor1,sensor2,....,sensorN
	// using POSIX specific function getline(3)
	// read manpages on what this function is
	// THIS CODE WILL ONLY RUN ON *NIX SYSTEMS
	
	char *line = NULL;
	size_t len = 0;
	
	if(getline(&line, &len, test_data) == 1) {
		printf("End of File!\n");
		fclose(test_data);
		free(line);
		return;
	}

	read_line(line, len);
	
	free(line);
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
