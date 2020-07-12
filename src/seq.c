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

static int vbs_red();


static int vbs_yellow(){
	/**
	 * This function is for the following scenario:
	 * 		P-TT-2/P-TT-3 wrong trends
	 * 
	 * Args:
	 * 		None
	 * 
	 * Returns:
	 * 		Success (int): 1 for successful seq
	 * */

	// get initial time;
	time_t start_t = clock();


	// Open P_EV_04 and check to make sure it is normal
	v[P_EV_04].stat = ON;

	while ((start_t + 5.0f) >= clock())
	{
		// check if trends normalize
		if (daq_val[P_PT_02].trend >= s[P_PT_02].min_trend
			&& daq_val[P_PT_02].trend <= s[P_PT_02].max_trend
			&& daq_val[P_PT_03].trend >= s[P_PT_02].min_trend
			&& daq_val[P_PT_03].trend <= s[P_PT_03].max_trend)
		{
			// everything is alright now
			v[P_EV_04].stat = OFF;
			
			// return to main sequence
			return 0;
		}
	}

	// could not control trend, initiate scrap
	return vbs_red();
	
}

static int vbs_orange()
{
	/**
	 * This function is called in the following scenarios
	 * 
	 * P-PT-1/P-TT-1 wrong trends
	 * 
	 * Args: 
	 * 		None
	 * 
	 * Returns:
	 * 
	 * 		success (int): 1 for a successful seq
	 **/
	
	// get initial time for the VBS
	time_t start_t = clock();

	v[P_EV_01].stat = ON;

	while ((start_t + 5.0f) >= clock())
	{
		// check if trends normalize
		if (daq_val[P_PT_01].trend >= s[P_PT_01].min_trend
			&& daq_val[P_PT_01].trend <= s[P_PT_01].max_trend)
		{
			// everything is alright now
			v[P_EV_01].stat = OFF;
			
			// return to main sequence
			return 0;
		}
	}

	// could not control trend, initiate scrap
	return vbs_red();

}

static int vbs_green()
{
	/**
	 * This function is called in the following scenarios
	 * 
	 * pre-seq:
	 * O-PT-1 trend failure
	 * O-TT-1 trend failure
	 * F-PT-1/F-TT-1
	 * F-PT-2/F-TT-2
	 * seq:
	 * F-PT-* trend failure
	 * 
	 * In these events it calls vbs_red to scrap the launch.
	 * 
	 * Args:
	 * 		None
	 * 
	 * Returns:
	 * 		success (int): 1 for a successful seq
	 * 
	**/

	return vbs_red();
}

static int vbs_blue()
{
	/**
	 * This function is called if O-PT-2/3 or F-PT-2/3 show bad trend
	 * It does the following actions:
	 * scrap launch
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	success (int): 1 for a successful seq
	**/
	
	// stub backup sequence
	// will be expanded during testing
	return vbs_red();
}

static int vbs_violet()
{
	/**
	 * This function is called if the rocket has bad trajectory
	 * It does the following actions:
	 * open P-EV-1, P-EV-4, O-SV-1	
	 * close O-EV-2, F-EV-3	
 	 * open O-EV-1, F-EV-1	
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	success (int): 1 for a successful seq
	**/

	//bad trajectory can happen at any instance, check if valves are in opposite state first

	v[P_EV_01].stat = ON;
	v[P_EV_04].stat = ON;
	v[O_SV_01].stat = ON;

	v[O_EV_02].stat = OFF;
	v[F_EV_03].stat = OFF;

	v[O_EV_01].stat = ON;
	v[F_EV_01].stat = ON;

	
	
	return 1;
}



static int vbs_red()
{
	// close final valves
	v[F_EV_03].stat = OFF;
	v[O_EV_02].stat = OFF;

	// open all gas and cryo vent valves
	v[P_EV_01].stat = ON;
	v[P_EV_02].stat = ON;
	v[O_SV_01].stat = ON;
	v[F_EV_01].stat = ON;
	v[O_EV_01].stat = ON;

	// TODO: add var ambient_p, ambient_t and read from config
	while (!0); // check till ambient pressure reached 

	// open RP-1 drain valve once all pressure lost
	v[F_EV_02].stat = ON;

	return 1;
}

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
