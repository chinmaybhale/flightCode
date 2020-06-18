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
