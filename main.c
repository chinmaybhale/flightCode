// HELIOS ROCKETRY
// Entry point of the flight code.
// Please try to limit definitions over here.
// Code will either be defined in init or seq.

#include "headers.h"

int main(int argc, char **argc)
{
	if(argc < 2) {
		printf("Need config file\n");
		return 0;
	}

	init(argc[1]);
	
	while(1)
		seq();
	
	return 0;
}
