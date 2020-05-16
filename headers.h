#ifndef __HEADERS_H__
#define __HEADERS_H__
//global headers
//#include <stdio.h>

/*****************************/
// local headers
#include "macros.h"
#include "init.h"
#include "seq.h"

// Global variables
// used for storing values of pressure, temp, etc
// declared here, initialized in init

typedef float pressure;
typedef float temperature;

extern pressure p[8];
extern temperature t[5];

#endif
