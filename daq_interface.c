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
	// this code splits the strings into individual values
	// and the converts them to float

	char *str_val = (char *)malloc(sizeof(char) * 30);
	int curr, prev = 0, i = 0;

	for(curr = 0; curr <= len; curr++) {
		if(line[curr] == ',' || line[curr] == '\n') {
			strncpy(str_val, line + prev, (curr - prev));
			str_val[curr - prev] = '\0';
			values[i++] = atof(str_val);
			prev = curr + 1;	
		}
		if(line[curr] == '\n')
			break;
	}

	free(str_val);
	return;
}

static int read_file()
{
	// TODO: read one line, fill the 
	// data structure
	// CSV format
	// sensor1,sensor2,....,sensorN
	
	char *line = (char *)malloc(sizeof(char) * 256);

	if(fgets(line, 256, test_data) == NULL) {
		printf("End of file!\n");
		fclose(test_data);
		free(line);
		return 1;
	}


	read_line(line, 256);
	
	free(line);
	return 0;
}

void init_file()
{
	// TODO: read the data in a CSV format when
	// running in debug mode
	
	test_data = fopen("test_data.csv", "r");

	if(!test_data) {
		printf("not able to open file\n");
		return;
	}

	return;
}

void get_data()
{
	int end;

	if(debug) {
		end = read_file();
		if(end)
			fclose(test_data);
	}
	else {

		read_data();
		convert();
	}

	return;
}
