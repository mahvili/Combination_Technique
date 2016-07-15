#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <math.h>
#include <float.h>


/* defined type of data stored in the structures*/
/*
#ifdef USE_SINGLE_PRECISION
typedef float FLOAT;
#else
typedef double FLOAT;
#endif
*/

#define DIMENSION 2

int NumberofPoints(int level){
	return (pow(2,level))+1;
}

#endif
