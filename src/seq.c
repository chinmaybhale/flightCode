#include "../include/headers.h"

int *verified_data;


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




void read_surya_file(){
	/**
	 * this function reads surya's file and turns the data into a 2D array 
	 * assumptions: we know how many rows of data are in surya's file
	 **/

	FILE *ff;
	char *ptr;
	int row = 4; // once we receive the file, we'll know how many rows to put but for now its 4
	int column = 13; // the amount of sensors we have 

	verified_data = (int*) malloc( row * column * sizeof(int));

	// replace this name with the real file name
	ff = fopen("suryasFile.csv", "r");

	// fill the data array
	ptr = fgetc(ff);
	char value_as_string[10];
	float value = 0.0;
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < column; j++)
		{
			while (1)
			{
				if (ptr == ',' || ptr == '\n')
				{
					value = atof(value_as_string);
					*(verified_data + i * column + j) = value;
					strcpy(value_as_string, "");
					ptr = fgetc(ff);
					break;
				}
				else
				{
					strcat(value_as_string, ptr);
					ptr = fgetc(ff);
				}
			}
		}
	}
}