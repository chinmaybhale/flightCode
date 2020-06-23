// Helios Rocketry AFCP/src/
// main.c- Entry point of AFCP. Please limits definitions and programs in here; they should go in /src/init.c and /src/seq.c

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

#include "../include/headers.h"

int main(int argc, char **argv)
{
	/**
	 * Main function
	 * 
	 * Args:
	 * 	argc (int): Amount of command line arguments
	 *  argv (char **): Array of strings containing command line arguments
	 * 
	 * Returns:
	 * 	int: 0 if no errors
	 * 
	**/

	if(argc < 2) {
		printf("Need config file\n");
		return 0;
	}

	init(argv[1]);
	
	while(1)
		seq();
	
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
