// Helios Rocketry AFCP/src/
// init.c- Initialize systems program to be run at the very beginning of main.c (Similar to 'void setup() in Arduino)
// The program consists of several static functions that are being called one by one in the main sequence. 

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
#include "../include/headers.h"
static void init_sensor(char *, struct sensor *);
static void init_valve(char *, struct valve *);

static void show_config(char *file_name)
{
	/**
	 * This function shows the config which has been set from the config file. Code will not execute further
	 *  without conformation from user that configuration is set right.
	 * 
	 * Args:
	 * 	file_name (char *): The name of the config file the program will open
	 * 
	 * Returns:
	 *	Void

	 */

	char opt;
	int i;
	FILE *file = fopen(file_name, "r");					
	char *name = (char *)malloc(sizeof(char) * 31);	

	if (!file)                                // File not accessible
	{ 
		printf("Not able to open file!");
		return;
	}

	// Displaying configuration file in an ordered manner
	printf("\n############### HELIOS ROCKETRY ##############\n");
	printf("###### Configuration of firing sequence ######\n\n");
	printf(">>> SENSOR CONFIGURATION <<<\n\n");
	printf("SENSOR NAME\t|\tMAX VALUE\t|\tMIN VALUE\t|\tMAX TREND\t|\tMIN TREND\t|\tPIN\t|\n");
	printf("---------------------------------------------------------------------------------------------------------------------------------\n");


	fgets(name, 31, file);				   	// reading line >MAIN

	for (i = 0; i < SENSOR_COUNT; i++)
	{
		fgets(name, 8, file);
		name[8] = '\0';
		printf("%s\t\t|\t%f\t|\t%f\t|\t%f\t|\t%f\t|\t%d\t|\n", name, s[i].max_val, s[i].min_val, 
				s[i].max_trend, s[i].min_trend, s[i].pin);
		fgets(name, 31, file);
	}

	printf("\n>>> VALVE CONFIGURATION <<<\n\n");
	printf("VALVE NAME\t|\tPIN\t|\tSTATUS\t|\tFEEDBACK PIN\t|\tFEEDBACK STATUS\t|\n");
	printf("-------------------------------------------------------------------------------------------------\n");

	fgets(name, 31, file);

	for (i = 0; i < VALVE_COUNT; i++)
	{
		fgets(name, 8, file);
		name[8] = '\0';
		printf("%s\t\t|\t%d\t|\t%s\t|\t    %d\t\t|\t    %s\t\t|\n", name, v[i].pin, ((v[i].stat) ? "on" : "off"),
				v[i].fb_pin, ((v[i].feedback) ? "on" : "off"));
		fgets(name, 31, file);
	}

	// User input for debug and verbose
	printf("\n>>> DEBUG CONFIGURATION <<<\n\n");
	printf("DEBUG: %s\n", ((debug) ? "YES" : "NO"));

	printf("\n>>> VERBOSE <<<\n\n");
	printf("VERBOSE: %s\n", ((verbose) ? "YES" : "NO"));

	printf("\n\nConfirm configuration (y/N): ");
	scanf("%c", &opt);

	if (opt != 'y')
	{
		printf("Exiting!\n");
		free(name);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	free(name);
	fclose(file);
	return;
}
//---------------------------------------------------------------------------------------------------------------------------

static void read_config(char *file_name)
{
	/** 
	 * This function reads the CSV config file, example format is as follows:
	 * 
	 * CSV FORMAT
	 * sensor list will start with ###SENSORS###
	 * valve list will start with ###VALVES###
	 * debug will start with ###DEBUG###
	 * verbose will start with ###VERBOSE###
	 *
	 * sensor_name(7)[0-6], base_val(4)[8-11], +ve error(2)[13, 14], -ve error(2)[16, 17],
	 * 	base_trend(4)[19-22], +ve trend error(2)[24, 25], -ve trend error(2)[27, 28], pin# (if availabe, -1 otherwise)(2)[30, 31]
	 * 
	 * or
	 * 
	 * valve_name(7)[0-6], pin#(2)[8, 9],feedback_pin#(2)[11, 12]
	 * 
	 * sensor_name will decide if it is a pressure or temperature or other kind of sensor.
	 * ------------------------------------------------------------------------------------------------------------------------------
	 * EXAMPLE CONFIG FILE
	 *
	 * ###SENSORS###
	 * P_PT_01,1000,10,5,50,2,2,-1
	 * F_PT_01,500,10,10,20,2,2-1
	 * ###VALVES###
	 * P_EV_02,00,01
	 * ###DEBUG###
	 * 1
	 * ###VERBOSE###
	 * 1
	 * -----------------------------------------------------------------------------------------------------------------------------
	 * Args:
	 *  file_name (char *): The name of the config file to opened
	 * 
	 * Returns;
	 * 	Void
	 */

	int i;
	char *setup = (char *)malloc(sizeof(char) * 34);
	FILE *file = fopen(file_name, "r");

	if (!file)
	{
		printf("Unable to open file\n");
		return;
	}

	while (fgets(setup, 33, file))
	{
		setup[32] = '\0';

		// sensors
		if (setup[0] == '#' && setup[3] == 'S')
		{
			for (i = 0; i < SENSOR_COUNT; i++)
			{
				fgets(setup, 34, file);
				setup[34] = '\0';
				init_sensor(setup, &s[i]);
			}
		}
		// valves
		else if (setup[0] == '#' && setup[4] == 'A')
		{
			for (i = 0; i < VALVE_COUNT; i++)
			{
				fgets(setup, 15, file);
				setup[15] = '\0';
				init_valve(setup, &v[i]);
			}
		}
		// debug
		else if (setup[0] == '#' && setup[3] == 'D')
		{
			fgets(setup, 2, file);
			if (setup[0] == '1')
				debug = 1;
			else
				debug = 0;
		}
		// verbose
		else if (setup[0] == '#' && setup[4] == 'E')
		{
			fgets(setup, 2, file);
			if (setup[0] == '1')
				verbose = 1;
			else
				verbose = 0;
		}
	}

	free(setup);
	fclose(file);
	return;
}
//---------------------------------------------------------------------------------------------------------------------------

static void init_sensor(char *setup, struct sensor *s)
{
	/**
	 * This function initializes a sensor given the setup string
	 * 
	 * Args:
	 * 	setup (char *): string containing the the base, +- error, and pin # of the sensor of the format: N_PT_01,1000,10,5,-1
	 * 	s (sensor *): the sensor whose base, +- error, and pin values you want to change based on the setup string
	 * 
	 * Returns:
	 * 	Void
	 * 
	**/

	// declaring generic variables
	char str_base[5], str_pos_err[3], str_neg_err[3], str_pin[3];
	float base, pos_err, neg_err;

	// reading absolute values and errors
	strncpy(str_base, setup + 8, 4);
	str_base[4] = '\0';

	strncpy(str_pos_err, setup + 13, 2);
	str_pos_err[2] = '\0';

	strncpy(str_neg_err, setup + 16, 2);
	str_neg_err[2] = '\0';
	
	base = atof(str_base);
 	pos_err = atof(str_pos_err) / 100.0f;
 	neg_err = atof(str_pos_err) / 100.0f;

	s->base_val = base;
	s->pos_val_err = pos_err;
	s->neg_val_err = neg_err;

	s->max_val = base + (base * pos_err);
	s->min_val = base - (base * neg_err);

	// reading trends and errors
	strncpy(str_base, setup + 19, 4);
	str_base[4] = '\0';

	strncpy(str_pos_err, setup + 24, 2);
	str_pos_err[2] = '\0';

	strncpy(str_neg_err, setup + 27, 2);
	str_neg_err[2] = '\0';

	base = atof(str_base);
 	pos_err = atof(str_pos_err) / 100.0f;
 	neg_err = atof(str_pos_err) / 100.0f;
	
	s->base_trend = base;
	s->pos_trend_err = pos_err;
	s->neg_trend_err = neg_err;

	s->max_trend = base + (base * pos_err);
	s->min_trend = base - (base * neg_err);
	
	// reading pin number
	strncpy(str_pin, setup + 30, 2);
	str_pin[2] = '\0';

	s->pin = atoi(str_pin);	

	return;
}
//---------------------------------------------------------------------------------------------------------------------------

static void init_valve(char *setup, struct valve *v)
{
	/**
	 * This function initializes a valve v, based on the setup string
	 * 
	 * Args:
	 * 	setup (char *): Description of the values you want to assosciate with the valve in a string format
	 *  ex: N_SV_01,0
	 * 
	 * Returns:
	 * 	void
	**/

	char pin[3];

	// reading output pin number
	strncpy(pin, setup + 8, 2);
	pin[2] = '\0';

	v->pin = atoi(pin);

	// reading feedback pin number
	strncpy(pin, setup + 11, 2);
	pin[2] = '\0';

	v->fb_pin = atoi(pin);

	v->stat = 0;
	
	return;
}
//---------------------------------------------------------------------------------------------------------------------------

static void system_check(char *file_name)
{
	/**
	 * This function checks all the valves and transducers to make sure they are within their expected values/range
	 * 
	 * Args:
	 * 	file_name (char*): Name of the file to get values from
	 *
	 * Returns:
	 * 	void
	 * 
	 **/
	get_data(); // get the most current values from the DAQ
	char name[33], opt;
	int i, no_go = 0;
	
	FILE *config_file = fopen(file_name, "r");

	if (!config_file)
	{
		printf("Unable to open file!\n");
		exit(EXIT_FAILURE);
	}

	// reading line ###SENSORS### 
	fgets(name, 33, config_file);

	printf("\n>>> SYSTEM CHECK START <<< \n\n");

	printf(">>> SENSORS:\n");
	for (i = 0; i < SENSOR_COUNT; i++)
	{
		// reading the sensor name
		fgets(name, 8, config_file);
		name[8] = '\0';

		if (s[i].min_val <= daq_val[i].curr &&
			daq_val[i].curr <= s[i].max_val)
		{
			if (verbose == 1)
				printf("%s\t->\tVal: %f,\tMin: %f,\tMax: %f,\tGood: Yes\n", name, daq_val[i].curr, s[i].min_val, s[i].max_val);
		}
		else
		{
			no_go = 1;
			printf("%s\t->\tVal: %f,\tMin: %f,\tMax: %f,\tGood: NO\n", name, daq_val[i].curr, s[i].min_val, s[i].max_val);
		}
		
		// skipping the rest of the contents of the line and moving to the next sensor name
		fgets(name, 33, config_file);
	}

	// reading line ###VALVES###
	fgets(name, 33, config_file);

	printf(">>> VALVES:\n");
	//assuming all valves should be off
	for (i = 0; i < VALVE_COUNT; i++)
	{
		// reading the valve name
		fgets(name, 8, config_file);
		name[8] = '\0';

		if (v[i].stat == 0)
		{
			if (verbose == 1)
				printf("%s\t->\tStat: %d,\tExpected: 0,\tGood: Yes\n", name, v[i].stat);
		}
		else
		{
			no_go = 1;
			printf("%s\t->\tStat: %d,\tExpected: 0,\tGood: NO\n", name, v[i].stat);
		}

		// skipping the rest of the contents of the line and moving to the next valve name
		fgets(name, 33, config_file);
	}

	// no need to read anything else from file, closing it
	fclose(config_file);

	printf("\n>>> SYSTEM CHECK COMPLETE <<<\n");

	// SYSTEMS CHECK- User input to abort!
	if (no_go)
	{
		printf("ANOMALIES DETECTED: RECOMMEND NO GO\n");
	}
	else
	{
		printf("ALL NOMINAL: SYSTEMS GO FOR MAIN SEQUENCE\n");
	}

	printf("\n\nConfirm System Check (y/N): ");
	scanf("%c", &opt);
	scanf("%c", &opt); // hacky way, ignoring newline

	if (opt != 'y')
	{
		printf("Exiting!\n");
		exit(EXIT_FAILURE);
	}

	return;
}
//---------------------------------------------------------------------------------------------------------------------------

void init(char *file_name)
{
	/**
	 * This function initializes the overall system, by making calls to the other init functions
	 * 
	 * Args:
	 * 	file_name (char *): The name of the config file
	 * 
	 * Returns:
	 * 	void
	 * 
	**/
	read_config(file_name);
	show_config(file_name);
	if (debug)
		init_file();
	else
		init_daq();
	system_check(file_name);
	init_verified_value_file();
	init_verified_trends_file();	
	return;
}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
