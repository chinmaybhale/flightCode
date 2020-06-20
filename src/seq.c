#include "../include/headers.h"


FILE *verified_file;
char *line;

static int read_verified();

int seq()
{
	/**
	 * this function ...
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	int: 0 
	 * 
	**/
	
	get_data();

	if (v[P_EV_02].stat == OFF) {
		// Valve is off, check absolute pressure values
		if (daq_val[P_PT_01].curr >= s[P_PT_01].max_val
				&& daq_val[P_PT_01].curr <= s[P_PT_01].min_val) {
			// open valve;
			v[P_EV_02].stat = ON;

			// get new data before proceeding
			get_data();
		}
	}
	else {
		// valve is on, check trends
		if (!(daq_val[P_PT_01].trend >= s[P_PT_01].max_trend)
				&& !(daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)) {
			// trends are not nominal
			// initiate backup?
		}
	}

	if (v[P_EV_02].stat == ON) {
		// main pressurant valve is on
		// check trend for increase of pressure
		
		
		// open P_EV_03 when absolute pressure is correct and P_EV_02 is completely open

		if (daq_val[P_PT_02].trend >= s[P_PT_02].min_trend
				&& daq_val[P_PT_02].trend <= s[P_PT_02].max_trend) {
			if (v[P_EV_03].stat == OFF // only go here if the valve isn't open
					&& daq_val[P_PT_02].curr >= s[P_PT_02].min_val
					&& daq_val[P_PT_02].curr <= s[P_PT_02].max_val
					&& daq_val[P_PT_01].trend >= s[P_PT_01].min_trend
					&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend) {
				// all pressures are right, trends are right
				// open the valve
				v[P_EV_03].stat = ON;

				// get new data
				get_data();
				
			}
			// trends are right, continue
			// valve open or not open does not matter
		}
		else {
			// trends are not right, initiate backup
		}
		
		
		// open P_EV_05 when absolute pressure is correct and P_EV_02 is completely open

		if (daq_val[P_PT_03].trend >= s[P_PT_03].min_trend
				&& daq_val[P_PT_03].trend <= s[P_PT_03].max_trend) {
			if (v[P_EV_05].stat == OFF // only go here if the valve isn't open
					&& daq_val[P_PT_03].curr >= s[P_PT_03].min_val
					&& daq_val[P_PT_03].curr <= s[P_PT_03].max_val
					&& daq_val[P_PT_01].trend >= s[P_PT_01].min_trend
					&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend) {
				// all pressures are right, trends are right
				// open the valve
				v[P_EV_05].stat = ON;

				// get new data
				get_data();
				
			}
			// trends are right, continue
			// valve open or not open does not matter
		}
		else {
			// trends are not right, initiate backup
		}
	}
	// else = do nothing, valve is not open, no need to check

	
	return 0;
}

static int read_verified()
{
	/**
	 * this function is called whenever you want to get the next line of the verified/expected
	 * values for the sensors
	 */
	char str_val[30];
	int len = 256;

	if ( fgets(line, len, verified_file) == NULL)
	{
		printf("end of verified file");
		free(line);
		fclose(verified_file);
		return 0;
	}
	else
	{
		
		int curr, prev = 0, i = 0;

		for(curr = 0; curr <= len; curr++) 
		{
			if(line[curr] == ',' || line[curr] == '\n') 
			{
				strncpy(str_val, line + prev, (curr - prev));
				str_val[curr - prev] = '\0';
				verified_data[i++].base_val = atof(str_val);
				prev = curr + 1;	
			}
			if(line[curr] == '\n')
				break;
		}
	}

	return 1;
	
}

void init_verified_file()
{
	/**
	 * this function is used to initiate the process of storing 
	 * the verified/expected values for our sensors.
	 */

	verified_file = fopen("verifiedFile.csv", "r");

	if (! verified_file )
		printf("unable to open verifiedFile.csv");

	line = (char *) malloc(sizeof(char) * 256);

	if (fgets(line, 256, verified_file) == NULL)
		printf("no data in verified file");

	return;
}
