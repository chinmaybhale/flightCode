// HELIOS ROCKETRY
// This file contains the code for interfacing with the daq
#include "../include/headers.h"

FILE *test_data;

static void convert()
{
	// TODO: convert values from 
	// Volts -> PSI/C/other format
	/**
	 * This functions converts volts to PSI/C/Other format
	 * 
	 * Args:
	 * 	None -> should take a float?
	 * 
	 * Returns:
	 * 	None -> should return a float?
	 *
	**/
	
	return;
}

static void read_data()
{
	// TODO: read code from DAQ here
	/**
	 * This function reads data from the daq
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	None
	 * 
	**/

	return;
}

static void read_line(char *line, int len)
{
	/**
	 * this code splits the strings into individual values
	 * and the converts them to float
	 * 
	 * Args:
	 * 	line(char *): The line you want to read
	 * 	len (int): the length of the line you are reading
	 * 
	 * Returns:
	 * 	None
	 * 
	**/

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
	/**
	 * This function reads a file in CSV format
	 * ex: sensor1,sensor2,....,sensorN
	 * 
	 * Args:
	 *  None
	 * 
	 * Returns:
	 * 	None
	 * 
	**/
	
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
	/**
	 * Opens the file for reading 
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	None
	 * 
	**/
	test_data = fopen("test_data.csv", "r");

	if(!test_data) {
		printf("not able to open file\n");
		return;
	}

	return;
}

void get_data()
{
	/**
	 * If in debug mode, this function reads the test data file. Otherwise, it calls the read data function
	 * and converts the data
	 * 
	 * Args:
	 *  None
	 * 
	 * Returns:
	 * 	None
	 * 
	**/
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
