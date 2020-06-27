/* 
 * Helios Rocketry AFCP/src/
 * seq.c- Program containing the control algorithm of the rocket's flight. The purpose is to make valve actuation decisions
 * based on sensor inputs and predifined logic. 
 * 
 * There're muliple control protocols being programmed simultaneously- BE ADVISED! The general idea is, sensors values and trends are compared
 * and a static high is given to the valve microcontrollers to actuate any specific valve. This is done in order to avoid any delays/lags in the 
 * main program. At the end of each iteration new data is fetched from the DAQ.
 * 
 * NOTE: Any changes to the Propulsion P&ID need to be reflected here.
 * NOTE: A low-pass filter might be added later on.
*/

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
#include "../include/headers.h"
//---------------------------------------------------------------------------------------------------------------------------

FILE *verified_file;
FILE *verified_val_file;
char *verified_val_line;
char *line;


static int read_verified();

int seq()
{
	/**
	 * This function carried out the main control logic of the AFCP.
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	int: 0 
	 * 
	**/

	get_data();

	// P_EV_02 is the valve right after the pressurant tank.
	if (daq_val[P_PT_01].trend >= s[P_PT_01].min_trend
		&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)
	{
		if (v[P_EV_02].stat == OFF // only go here if the valve isn't open
			&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val
			&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val)
		{
			// all pressures nominal
			// open valve
			v[P_EV_02].stat = ON;

			// get new data
			get_data();
		}
		// otherwise all trends nominal, continue running
	}
	else
	{
		// abnormal trends, initiate backup
	}

	if (v[P_EV_02].stat == ON)
	{
		// main pressurant valve is on
		// check trend for increase of pressure

		// open P_EV_03 when absolute pressure is correct and P_EV_02 is completely open

		if (daq_val[P_PT_02].trend >= s[P_PT_02].min_trend
			&& daq_val[P_PT_02].trend <= s[P_PT_02].max_trend
			&& daq_val[O_TT_01].trend >= s[O_TT_01].min_trend
			&& daq_val[O_TT_01].trend <= s[O_TT_01].max_trend)
		{

			if (v[P_EV_03].stat == OFF // only go here if the valve isn't open
				&& daq_val[P_PT_02].curr >= s[P_PT_02].min_val 
				&& daq_val[P_PT_02].curr <= s[P_PT_02].max_val 
				&& daq_val[O_TT_01].curr >= s[O_TT_01].min_val 
				&& daq_val[O_TT_01].curr <= s[O_TT_01].max_val 
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val 
				&& daq_val[P_PT_01].trend >= s[P_PT_01].min_trend 
				&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)
			{

				// all pressures are right, trends are right
				// open the valve
				v[P_EV_03].stat = ON;

				// get new data
				get_data();
			}
			// trends are right, continue
			// valve open or not open does not matter
		}
		else
		{
			// trends are not right, initiate backup
		}

		// open P_EV_05 when absolute pressure is correct and P_EV_02 is completely open

		if (daq_val[P_PT_03].trend >= s[P_PT_03].min_trend 
			&& daq_val[P_PT_03].trend <= s[P_PT_03].max_trend 
			&& daq_val[F_TT_01].trend >= s[F_TT_01].min_trend 
			&& daq_val[F_TT_01].trend <= s[F_TT_01].max_trend)
		{

			if (v[P_EV_05].stat == OFF // only go here if the valve isn't open
				&& daq_val[P_PT_03].curr >= s[P_PT_03].min_val 
				&& daq_val[P_PT_03].curr <= s[P_PT_03].max_val 
				&& daq_val[F_TT_01].curr >= s[F_TT_01].min_val 
				&& daq_val[F_TT_01].curr <= s[F_TT_01].max_val 
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val 
				&& daq_val[P_PT_01].trend >= s[P_PT_01].min_trend 
				&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)
			{
				// all pressures are right, trends are right
				// open the valve
				v[P_EV_05].stat = ON;

				// get new data
				get_data();
			}
			// trends are right, continue
			// valve open or not open does not matter
		}
		else
		{
			// trends are not right, initiate backup
		}
	}
	if (v[P_EV_03].stat == ON && v[P_EV_05].stat == ON)
	{
		// only start checking values if LOX and RP-1 pressurant lines are open

		// start checking trends of LOX line
		if (daq_val[O_PT_01].trend >= s[O_PT_01].min_trend 
		&& daq_val[O_PT_01].trend <= s[O_PT_01].max_trend 
		&& daq_val[O_PT_02].trend >= s[O_PT_02].min_trend 
		&& daq_val[O_PT_02].trend <= s[O_PT_02].max_trend 
		&& daq_val[O_PT_03].trend >= s[O_PT_03].min_trend 
		&& daq_val[O_PT_03].trend <= s[O_PT_03].max_trend)
		{

			if (v[O_SV_03].stat == OFF // only go here if valve not open
				&& daq_val[O_PT_01].curr >= s[O_PT_01].min_val 
				&& daq_val[O_PT_01].curr <= s[O_PT_01].max_val 
				&& daq_val[O_PT_02].curr >= s[O_PT_02].min_val 
				&& daq_val[O_PT_02].curr <= s[O_PT_02].max_val 
				&& daq_val[O_PT_03].curr >= s[O_PT_03].min_val 
				&& daq_val[O_PT_03].curr <= s[O_PT_03].max_val 
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val 
				&& daq_val[P_PT_01].trend >= s[P_PT_01].min_trend 
				&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)
			{

				// all conditions nominal, open valve
				v[O_SV_03].stat = ON;

				// get new sensor data
				get_data();
			}
			// all trends nominal, continue operations
		}
		else
		{
			// abnormal trends, initiate backup
		}

		// start checking trends of RP-1 line
		if (daq_val[F_PT_01].trend >= s[F_PT_01].min_trend 
			&& daq_val[F_PT_01].trend <= s[F_PT_01].max_trend 
			&& daq_val[F_PT_02].trend >= s[F_PT_02].min_trend 
			&& daq_val[F_PT_02].trend <= s[F_PT_02].max_trend 
			&& daq_val[F_PT_03].trend >= s[F_PT_03].min_trend 
			&& daq_val[F_PT_03].trend <= s[F_PT_03].max_trend)
		{

			if (v[F_EV_03].stat == OFF // only go here if valve not open
				&& daq_val[F_PT_01].curr >= s[F_PT_01].min_val 
				&& daq_val[F_PT_01].curr <= s[F_PT_01].max_val 
				&& daq_val[F_PT_02].curr >= s[F_PT_02].min_val 
				&& daq_val[F_PT_02].curr <= s[F_PT_02].max_val 
				&& daq_val[F_PT_03].curr >= s[F_PT_03].min_val 
				&& daq_val[F_PT_03].curr <= s[F_PT_03].max_val 
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val 
				&& daq_val[P_PT_01].trend >= s[P_PT_01].min_trend 
				&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)
			{

				// all conditions nominal, open valve
				v[F_EV_03].stat = ON;

				// get new sensor data
				get_data();
			}
			// all trends nominal, continue operations
		}
		else
		{
			// abnormal trends, initiate backup
		}
	}
	// else = do nothing, valve is not open, no need to check

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

static int read_verified()
{
	/**
	 * this function is called whenever you want to get the next line of the verified/expected
	 * values for the sensors
	 */
	char str_val[30];
	int len = 256;

	if (fgets(line, len, verified_file) == NULL)
	{
		printf("end of verified file");
		free(line);
		fclose(verified_file);
		return 0;
	}
	else
	{

		int curr, prev = 0, i = 0;

		for (curr = 0; curr <= len; curr++)
		{
			if (line[curr] == ',' || line[curr] == '\n')
			{
				strncpy(str_val, line + prev, (curr - prev));
				str_val[curr - prev] = '\0';
				verified_data[i++].base_val = atof(str_val);
				prev = curr + 1;
			}
			if (line[curr] == '\n')
				break;
		}
	}

	return 1;
}
//---------------------------------------------------------------------------------------------------------------------------

void init_verified_file()
{
	/**
	 * this function is used to initiate the process of storing 
	 * the verified/expected values for our sensors.
	 */

	verified_file = fopen("verifiedFile.csv", "r");

	if (!verified_file)
		printf("unable to open verifiedFile.csv");

	line = (char *)malloc(sizeof(char) * 256);

	if (fgets(line, 256, verified_file) == NULL)
		printf("no data in verified file");

	return;
}
//---------------------------------------------------------------------------------------------------------------------------

static int read_verified_value()
{
	/**
	 * this function is called whenever you want to get the next line of the verified/expected
	 * values for the sensors
	 */
	char str_val[30];

	if (fgets(verified_val_line, MAX_DATA_LENGTH, verified_val_file) == NULL)
	{
		printf("End of verified value file!\n");
		free(verified_val_line);
		fclose(verified_val_file);
		return 0;
	}
	else
	{
		int curr, prev = 0, i = 0;
    	int count = 1;

		for (curr = 0; curr <= MAX_DATA_LENGTH; curr++)
		{
			if (verified_val_line[curr] == ',' || verified_val_line[curr] == '\n')
			{
				strncpy(str_val, verified_val_line + prev, (curr - prev));
				str_val[curr - prev] = '\0';
				switch(count)
				{
				case 1 :
					//the first value we encounter is the base value
					s[i].base_val = atof(str_val);
					count++;
					break;
				
				case 2 :
					//the next value we encounter is the pos error
					s[i].pos_val_err = atoi(str_val);
					s[i].max_val = s[i].base_val + s[i].pos_val_err;
					count++;
					break;
				
				case 3 :
					//the final value we encounter is the neg err, then we reset count to 1
					s[i].neg_val_err = atoi(str_val);
					s[i++].min_val = s[i].base_val - s[i].neg_val_err;
					count = 1;
					break;
				
				default :
					break;
				}
				prev = curr + 1;
			}

			if (verified_val_line[curr] == '\n')
				break;
		}
	}

	return 1;
}
//---------------------------------------------------------------------------------------------------------------------------

void init_verified_value_file()
{
	/**
	 * this function is used to initiate the process of storing 
	 * the verified/expected values for our sensors.
	 * 
	 * File must have a string header as the first line and in the following format:
	 * 
	 * <base_val_sensor_1>,<pos_val_err>,<neg_val_err>,...
	 */

	verified_val_file = fopen("verifiedValueFile.csv", "r");

	if (!verified_val_file)
		printf("Unable to open verifiedValueFile.csv!");

	verified_val_line = (char *)malloc(sizeof(char) * 256);

	if (fgets(verified_val_line, 256, verified_val_file) == NULL)
		printf("No data in verified file!\n");

	return;
}
