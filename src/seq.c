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


FILE *verified_trends;
char *line;

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
	read_verified_data();

	// P_EV_02 is the valve right after the pressurant tank.
	if (daq_val[P_PT_01].trend >= s[P_PT_01].min_trend
		&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend
		&& daq_val[P_TT_01].trend >= s[P_TT_01].min_trend
		&& daq_val[P_TT_01].trend <= s[P_TT_01].max_trend)
	{
		if (v[P_EV_02].stat == OFF // only go here if the valve isn't open
			&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val
			&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val
			&& daq_val[P_TT_01].curr >= s[P_TT_01].min_val
			&& daq_val[P_TT_01].curr <= s[P_TT_01].max_val)
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
			&& daq_val[P_TT_02].trend >= s[P_TT_02].min_trend
			&& daq_val[P_TT_02].trend <= s[P_TT_02].max_trend)
		{

			if (v[P_EV_03].stat == OFF // only go here if the valve isn't open
				&& v[P_EV_02].feedback == ON // check if P_EV_02 is fully open or not
				&& daq_val[P_PT_02].curr >= s[P_PT_02].min_val 
				&& daq_val[P_PT_02].curr <= s[P_PT_02].max_val 
				&& daq_val[P_TT_02].curr >= s[P_TT_02].min_val 
				&& daq_val[P_TT_02].curr <= s[P_TT_02].max_val 
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val) 
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
				&& v[P_EV_02].feedback == ON // check if P_EV_02 is fully open
				&& daq_val[P_PT_03].curr >= s[P_PT_03].min_val 
				&& daq_val[P_PT_03].curr <= s[P_PT_03].max_val 
				&& daq_val[P_TT_03].curr >= s[P_TT_03].min_val 
				&& daq_val[P_TT_03].curr <= s[P_TT_03].max_val 
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val) 
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
			&& daq_val[O_PT_03].trend <= s[O_PT_03].max_trend
			&& daq_val[O_TT_01].trend >= s[O_TT_01].min_trend
			&& daq_val[O_TT_01].trend <= s[O_TT_01].max_trend)
		{

			if (v[O_EV_02].stat == OFF // only go here if valve not open
				&& v[P_EV_02].feedback == ON // check if LOX pressurant valve is completely open
				&& v[P_EV_03].feedback == ON // check if main pressurant valve is completely open
				&& daq_val[O_PT_01].curr >= s[O_PT_01].min_val 
				&& daq_val[O_PT_01].curr <= s[O_PT_01].max_val 
				&& daq_val[O_PT_02].curr >= s[O_PT_02].min_val 
				&& daq_val[O_PT_02].curr <= s[O_PT_02].max_val 
				&& daq_val[O_PT_03].curr >= s[O_PT_03].min_val 
				&& daq_val[O_PT_03].curr <= s[O_PT_03].max_val
				&& daq_val[O_TT_01].curr >= s[O_TT_01].min_val
				&& daq_val[O_TT_01].curr <= s[O_TT_01].max_val
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val) 
			{

				// all conditions nominal, open valve
				v[O_EV_02].stat = ON;

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
			&& daq_val[F_PT_03].trend <= s[F_PT_03].max_trend
			&& daq_val[F_TT_01].trend >= s[F_TT_01].min_trend
			&& daq_val[F_TT_01].trend <= s[F_TT_01].max_trend
			&& daq_val[F_TT_02].trend >= s[F_TT_02].min_trend
			&& daq_val[F_TT_02].trend <= s[F_TT_02].max_trend)
		{

			if (v[F_EV_03].stat == OFF // only go here if valve not open
				&& v[P_EV_02].feedback == ON // check if main pressurant valve is completely open
				&& v[P_EV_05].feedback == ON // check if RP-1 pressurant valve is completely open
				&& daq_val[F_PT_01].curr >= s[F_PT_01].min_val 
				&& daq_val[F_PT_01].curr <= s[F_PT_01].max_val 
				&& daq_val[F_PT_02].curr >= s[F_PT_02].min_val 
				&& daq_val[F_PT_02].curr <= s[F_PT_02].max_val 
				&& daq_val[F_PT_03].curr >= s[F_PT_03].min_val 
				&& daq_val[F_PT_03].curr <= s[F_PT_03].max_val
				&& daq_val[F_TT_01].curr >= s[F_TT_01].min_val
				&& daq_val[F_TT_01].curr <= s[F_TT_01].max_val
				&& daq_val[F_TT_02].curr >= s[F_TT_01].min_val
				&& daq_val[F_TT_02].curr <= s[F_TT_02].max_val
				&& daq_val[P_PT_01].curr >= s[P_PT_01].min_val 
				&& daq_val[P_PT_01].curr <= s[P_PT_01].max_val) 
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

void init_verified_trends_file()
{
	/**
	 * this function is used to initiate the process of storing 
	 * the verified/expected values FOR ARRAY S[]
	 */

	verified_trends = fopen("verifiedTrends.csv", "r");

	if (!verified_trends)
		printf("unable to open verifiedTrends.csv");

	line = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);

	if (fgets(line, MAX_DATA_LENGTH, verified_trends) == NULL)
		printf("no data in verified trends file");

	return;
}

static int read_verified_trends()
{
	/**
	 * this function is called whenever you want to get the next line of the verified trends file
	 */
	char str_val[30];

	if (fgets(line, MAX_DATA_LENGTH, verified_trends) == NULL)
	{
		printf("end of verified file");
		free(line);
		fclose(verified_trends);
		return 0;
	}
	else
	{

		int curr, prev = 0, i = 0, s_position = 0;

		for (curr = 0; curr <= MAX_DATA_LENGTH; curr++)
		{
			if (line[curr] == ',' || line[curr] == '\n')
			{
				strncpy(str_val, line + prev, (curr - prev));
				str_val[curr - prev] = '\0';

				switch (i)
				{
				case 1:
					s[s_position].base_trend = atof(str_val);
					i++;
					break;
				case 2:
					s[s_position].pos_trend_err = atof(str_val);
					s[s_position].max_trend = s[s_position].base_trend + 
						(s[s_position].base_trend * s[s_position].pos_trend_err);
					i++;
					break;
				case 3:
					s[s_position].neg_trend_err = atof(str_val);
					s[s_position].min_trend = s[s_position].base_trend - 
						(s[s_position].base_trend * s[s_position].neg_trend_err);
					i = 1;
					break;
				default:
					break;
				}
			
				prev = curr + 1;
				s_position++;
			
			}
			if (line[curr] == '\n')
				break;
		}
	}

	return 1;
}
