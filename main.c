#include "headers.h"
#include "macros.h"

int main()
{
	read_config();
	init();
	
	while(1)
		seq();
	
	return 0;
}
