// HELIOS ROCKETRY
// Entry point of the flight code.
// Please try to limit definitions over here.
// Code will either be defined in init or seq.

#include "../include/headers.h"

int main(int argc, char **argv)
{
	if(argc < 2) {
		printf("Need config file\n");
		return 0;
	}

	init(argv[1]);
	
	while(1)
		seq();
	
	return 0;
}
