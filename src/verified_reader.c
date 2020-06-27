#include "../include/headers.h"

FILE *verified_val_file;
char *line;

static int read_verified_value()
{
	/**
	 * this function is called whenever you want to get the next line of the verified/expected
	 * values for the sensors
	 */
	char str_val[30];

	if (fgets(line, MAX_DATA_LENGTH, verified_val_file) == NULL)
	{
		printf("End of verified value file!\n");
		free(line);
		fclose(verified_val_file);
		return 0;
	}
	else
	{
		int curr, prev = 0, i = 0;
    	int count = 1;

		for (curr = 0; curr <= MAX_DATA_LENGTH; curr++)
		{
			if (line[curr] == ',' || line[curr] == '\n')
			{
				strncpy(str_val, line + prev, (curr - prev));
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
					s[i].max_val = s[i].base_val + (s[i].base_val * s[i].pos_val_err);
					count++;
					break;
				
				case 3 :
					//the final value we encounter is the neg err, then we reset count to 1
					s[i].neg_val_err = atoi(str_val);
					s[i].min_val = s[i].base_val - (s[i].base_val * s[i].neg_val_err);
					count = 1;
					i++;
					break;
				
				default :
					break;
				}
				prev = curr + 1;
			}

			if (line[curr] == '\n')
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

	line = (char *)malloc(sizeof(char) * MAX_DATA_LENGTH);

	if (fgets(line, MAX_DATA_LENGTH, verified_val_file) == NULL)
		printf("No data in verified file!\n");

	return;
}

void read_verified_data()
{
	read_verified_value();	
}
