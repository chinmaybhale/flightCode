// Helios Rocketry AFCP/include/
// macros.h- Header file consisting of definitions for valves, sensors and DAQ parameter.

// Any changes to the Propulsion P&ID should be reflected here.


//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
#ifndef __MACROS_H__
#define __MACROS_H__

// Definitions required by the DAQ
#define MAX_DEV_COUNT 100
#define MAX_STR_LENGTH 64

#define MAX_DATA_LENGTH 512         // Max # of characters allowed in a single line for data to be read and parsed accurately

// Total variables
#define VALVE_COUNT 11              // Total # of valves
#define SENSOR_COUNT 15             // Total # of transducers

// Valve status
#define OFF 0
#define ON 1
//---------------------------------------------------------------------------------------------------------------------------

// VALVES- All the electric and solenoid valves that need to be controlled by AFCP
#define P_EV_01 0
#define P_EV_02 1
#define P_EV_03 2
#define P_EV_04 3
#define P_EV_05 4
#define F_EV_01 5
#define F_EV_02 6
#define F_EV_03 7
#define O_EV_01 8
#define O_EV_02 9
#define O_SV_01 10

// TRANSDUCERS- Pressure, temperature & Level transducers being read by AFCP
#define P_PT_01 0
#define P_PT_02 1
#define P_PT_03 2
#define O_PT_01 3
#define O_PT_02 4
#define O_PT_03 5
#define F_PT_01 6
#define F_PT_02 7
#define F_PT_03 8
#define P_TT_01 9
#define P_TT_02 10
#define P_TT_03 11
#define O_TT_01 12
#define F_TT_01 13
#define F_TT_02 14

#endif
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
