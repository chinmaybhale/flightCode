/* 
 * Helios Rocketry AFCP/src/
 * daq_utility.c- Program to interface with the Data Acquisition system.
 * Contains the getdata() function responsible for supplying formatted transducer outputs to seq.c
 * 
 * NOTE: Any changes to the Propulsion P&ID need to be reflected here.
 * NOTE: A low-pass filter might be added later on.
*/

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
#include "../include/headers.h"
//---------------------------------------------------------------------------------------------------------------------------

FILE *test_data;
char *line;


// variables needed to get analog data
int descriptorIndex = 0;
DaqDeviceDescriptor devDescriptors[MAX_DEV_COUNT];
DaqDeviceInterface interfaceType = ANY_IFC;
DaqDeviceHandle daqDeviceHandle = 0;
unsigned int numDevs = MAX_DEV_COUNT;

// highChan specifies the range of channels on the daq. so data will be gathered between channel 0 to 4 here
int lowChan = 0;
int highChan = SENSOR_COUNT;
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

static void free_daq();
//---------------------------------------------------------------------------------------------------------------------------

void init_daq()
{

	// Get descriptors for all of the available DAQ devices
	err = ulGetDaqDeviceInventory(interfaceType, devDescriptors, &numDevs);

	if (err != ERR_NO_ERROR)
		free_daq();

	// verify at least one DAQ device is detected
	if (numDevs == 0)
	{
		printf("No DAQ device is detected\n");
		free_daq();
	}

	printf("Found %d DAQ device(s)\n", numDevs);
	for (i = 0; i < (int)numDevs; i++)
		printf("  [%d] %s: (%s)\n", i, devDescriptors[i].productName, devDescriptors[i].uniqueId);

	if (numDevs > 1)
		descriptorIndex = selectDAQDevice(numDevs);

	// get a handle to the DAQ device associated with the first descriptor
	daqDeviceHandle = ulCreateDaqDevice(devDescriptors[descriptorIndex]);

	if (daqDeviceHandle == 0)
	{
		printf("\nUnable to create a handle to the specified DAQ device\n");
		free_daq();
	}

	// verify the specified DAQ device supports analog input
	err = getDevInfoHasAi(daqDeviceHandle, &hasAI);
	if (!hasAI)
	{
		printf("\nThe specified DAQ device does not support analog input\n");
		free_daq();
	}

	printf("\nConnecting to device %s - please wait ...\n", devDescriptors[descriptorIndex].devString);

	// establish a connection to the DAQ device
	err = ulConnectDaqDevice(daqDeviceHandle);

	if (err != ERR_NO_ERROR)
		free_daq();

	// i think we have to manipulate this line
	// get the first supported analog input mode
	err = getAiInfoFirstSupportedInputMode(daqDeviceHandle, &numberOfChannels, &inputMode, inputModeStr);

	if (highChan >= numberOfChannels)
		highChan = numberOfChannels - 1;

	// get the first supported analog input range
	err = getAiInfoFirstSupportedRange(daqDeviceHandle, inputMode, &range, rangeStr);
}
//---------------------------------------------------------------------------------------------------------------------------

static void free_daq()
{

	// release the handle to the DAQ device
	if (daqDeviceHandle)
		ulReleaseDaqDevice(daqDeviceHandle);

	if (err != ERR_NO_ERROR)
	{
		char errMsg[ERR_MSG_LEN];
		ulGetErrMsg(err, errMsg);
		printf("Error Code: %d \n", err);
		printf("Error Message: %s \n", errMsg);
	}

	exit(EXIT_FAILURE);
}
//---------------------------------------------------------------------------------------------------------------------------

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
//---------------------------------------------------------------------------------------------------------------------------

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

		if (err == ERR_NO_ERROR)
		{
			daq_val[chan].prev = daq_val[chan].curr;
			daq_val[chan].curr = data;
			daq_val[chan].trend = daq_val[chan].curr - daq_val[chan].prev;
		}
		else
		{
			printf("error in reading channel %d", chan);
			daq_val[chan].curr = -1; // weird number so we know which channels are not getting data
		}
	}

	return;
}
//---------------------------------------------------------------------------------------------------------------------------

static int read_DAQ_file()
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

	char str_val[30];
	int curr, prev = 0, i = 0;

	if (fgets(line, MAX_DATA_LENGTH, test_data) == NULL)
	{
		printf("End of file!\n");
		free(line);
		return 1;
	}

	for (curr = 0; curr <= MAX_DATA_LENGTH; curr++)
	{
		if (line[curr] == ',' || line[curr] == '\n')
		{
			strncpy(str_val, line + prev, (curr - prev));
			str_val[curr - prev] = '\0';
			daq_val[i].prev = daq_val[i].curr;
			daq_val[i].curr = atof(str_val);
			daq_val[i].trend = daq_val[i].curr - daq_val[i].prev;
			prev = curr + 1;
			i++;
		}
		if (line[curr] == '\n')
			break;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

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
	line = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);

	if (!test_data)
	{
		printf("not able to open file\n");
		return;
	}

	return;
}
//---------------------------------------------------------------------------------------------------------------------------

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

	if (debug)
	{
		end = read_DAQ_file();
		if (end)
			fclose(test_data);
	}
	else
	{

		read_data();
		convert();
	}

	return;
}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
