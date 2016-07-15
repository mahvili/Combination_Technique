#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <math.h>
#include <float.h>


/* defined type of data stored in the structures*/
#ifdef USE_SINGLE_PRECISION
typedef float FLOAT;
#else
typedef double FLOAT;
#endif

#define DIMENSION 2


class Domain{
public:
	/*public member variables */
	FLOAT start_point[DIMENSION], end_point[DIMENSION];
};

#endif
