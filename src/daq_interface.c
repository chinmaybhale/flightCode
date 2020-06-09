// HELIOS ROCKETRY
// This file contains the code for interfacing with the daq
#include "../include/headers.h"

FILE *test_data;

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


void init_daq(){

	// Get descriptors for all of the available DAQ devices
	err = ulGetDaqDeviceInventory(interfaceType, devDescriptors, &numDevs);

	if (err != ERR_NO_ERROR)
		goto();

	// verify at least one DAQ device is detected
	if (numDevs == 0)
	{
		printf("No DAQ device is detected\n");
		goto();
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
		goto();
	}

	// verify the specified DAQ device supports analog input
	err = getDevInfoHasAi(daqDeviceHandle, &hasAI);
	if (!hasAI)
	{
		printf("\nThe specified DAQ device does not support analog input\n");
		goto();
	}

	printf("\nConnecting to device %s - please wait ...\n", devDescriptors[descriptorIndex].devString);

	// establish a connection to the DAQ device
	err = ulConnectDaqDevice(daqDeviceHandle);

	if (err != ERR_NO_ERROR)
		goto();

	// i think we have to manipulate this line
	// get the first supported analog input mode
	err = getAiInfoFirstSupportedInputMode(daqDeviceHandle, &numberOfChannels, &inputMode, inputModeStr);

	if (highChan >= numberOfChannels)
		highChan = numberOfChannels - 1;

		// get the first supported analog input range
	err = getAiInfoFirstSupportedRange(daqDeviceHandle, inputMode, &range, rangeStr);


}

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
	 * 	void -> should return a float?
	 *
	**/
	
	return;
}

static void read_data()
{
	/**
	 * This function reads data from the daq
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	void
	 * 
	**/
	// TODO: read code from DAQ here
	// write data for the first 4 analog input channels
	for (chan = lowChan; chan <= highChan; chan++)
	{
		err = ulAIn(daqDeviceHandle, chan, inputMode, range, flags, &data);

		if(err == ERR_NO_ERROR)
		{
			values[chan] = data;
		}
		else
		{
			printf("error in reading channel %d", chan);
			values[chan] = -1; // weird number so we know which channels are not getting data
		}
	}
	

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
	 * 	void
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
	 * 	int: 1 if succesful, else 0
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
	 * 	void
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
	 * 	void
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

void goto()
{

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


