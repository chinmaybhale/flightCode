# flightCode

main.c:
	the main entry point into the code, should not contain any actual code, other than
	function calls and the definition for reading the config

init.c:
	contains the code for initializing and systems check before the main sequence is run

seq.c:
	contains the code for the main sequence. Loops indefinitely right now, can be changed later

init.h, seq.h, headers.h:
	header files for linking all the files together. 

macros.h:
	contains macros needed for conversion of values and actuation of valves.

Assign yourself a task in the issues tab. Branch the code to work on it, and open a pull request when you 
are done coding.

Please comment your code!!!!

To compile code, type "make" in the terminal.

To compile code for debugging, type "make debug" in the terminal.

To create a fresh solution in case of a bad compile, or to recompile, type "make clean" and then type one
of the above in the terminal.
