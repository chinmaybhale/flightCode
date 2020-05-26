// HELIOS ROCKETRY
// This file contains the code for interfacing with the daq
#include "headers.h"


// variables needed to get analog data
int descriptorIndex = 0;
DaqDeviceDescriptor devDescriptors[MAX_DEV_COUNT];
DaqDeviceInterface interfaceType = ANY_IFC;
DaqDeviceHandle daqDeviceHandle = 0;
unsigned int numDevs = MAX_DEV_COUNT;

// highChan specifies the range of channels on the daq. so data will be gathered between channel 0 to 4 here
int lowChan = 0;
int highChan = 3;
int chan = 0;
AiInputMode inputMode;
Range range;
AInFlag flags = AIN_FF_DEFAULT;

int hasAI = 0;
int numberOfChannels = 0;

char inputModeStr[MAX_STR_LENGTH];
char rangeStr[MAX_STR_LENGTH];
int __attribute__((unused)) ret;
char c;
UlError err = ERR_NO_ERROR;
int i = 0;
double data = 0;

/* allocating a space in memory for all the channel data. 
 * I'll be reusing the same space for every call and this is the pointer to that space
 */
double *sensor_data;


void init_daq(){

	//reserve a space in memory for the analog daq data 
	sensor_data = malloc( highChan * sizeof(double));	


	// Get descriptors for all of the available DAQ devices
	err = ulGetDaqDeviceInventory(interfaceType, devDescriptors, &numDevs);

	if (err != ERR_NO_ERROR)
		goto end;

	// verify at least one DAQ device is detected
	if (numDevs == 0)
	{
		printf("No DAQ device is detected\n");
		goto end;
	}

	printf("Found %d DAQ device(s)\n", numDevs);
	for (i = 0; i < (int) numDevs; i++)
		printf("  [%d] %s: (%s)\n", i, devDescriptors[i].productName, devDescriptors[i].uniqueId);

	if(numDevs > 1)
		descriptorIndex = selectDAQDevice(numDevs);

	// get a handle to the DAQ device associated with the first descriptor
	daqDeviceHandle = ulCreateDaqDevice(devDescriptors[descriptorIndex]);

	if (daqDeviceHandle == 0)
	{
		printf ("\nUnable to create a handle to the specified DAQ device\n");
		goto end;
	}

	// verify the specified DAQ device supports analog input
	err = getDevInfoHasAi(daqDeviceHandle, &hasAI);
	if (!hasAI)
	{
		printf("\nThe specified DAQ device does not support analog input\n");
		goto end;
	}

	printf("\nConnecting to device %s - please wait ...\n", devDescriptors[descriptorIndex].devString);

	// establish a connection to the DAQ device
	err = ulConnectDaqDevice(daqDeviceHandle);

	if (err != ERR_NO_ERROR)
		goto end;

	// i think we have to manipulate this line
	// get the first supported analog input mode
	err = getAiInfoFirstSupportedInputMode(daqDeviceHandle, &numberOfChannels, &inputMode, inputModeStr);

	if (highChan >= numberOfChannels)
		highChan = numberOfChannels - 1;

		// get the first supported analog input range
	err = getAiInfoFirstSupportedRange(daqDeviceHandle, inputMode, &range, rangeStr);



	end:

	// release the handle to the DAQ device
	if(daqDeviceHandle)
		ulReleaseDaqDevice(daqDeviceHandle);

	if(err != ERR_NO_ERROR)
	{
		char errMsg[ERR_MSG_LEN];
		ulGetErrMsg(err, errMsg);
		printf("Error Code: %d \n", err);
		printf("Error Message: %s \n", errMsg);
	}

}

static void convert()
{
	// TODO: convert values from 
	// Volts -> PSI/C/other format
	
	return;
}

static void read_data()
{
	// TODO: read code from DAQ here

	// write data for the first 4 analog input channels
	for (chan = lowChan; chan <= highChan; chan++)
	{
		err = ulAIn(daqDeviceHandle, chan, inputMode, range, flags, &data);

		if(err == ERR_NO_ERROR)
		{
			sensor_data[chan] = data;
		}
		else
		{
			printf("error in reading channel %d", chan);
			sensor_data[chan] = 9999.99; // weird number so we know which channels are not getting data
		}
	}
	
	return;
}

void get_data()
{
	read_data();
	convert();
	return;
}
