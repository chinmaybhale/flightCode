// HELIOS ROCKETRY
// Entry point of the flight code.
// Please try to limit definitions over here.
// Code will either be defined in init or seq.

#include "headers.h"

int main()
{
	init();
	
	while(1)
		seq();
	
	return 0;
}
