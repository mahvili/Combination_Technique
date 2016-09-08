#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <math.h>
#include <float.h>
#include <string>
using std::string;

/* defined type of data stored in the structures*/
/*
#ifdef USE_SINGLE_PRECISION
typedef float FLOAT;
#else
typedef double FLOAT;
#endif
 */

#define DIMENSION 2
int MAX_ADAPTATION_LEVEL;
int WRITE_VTK; //true when we want to write vtks
#define TRESHOLD_ERROR 0.0000001
#define USE_PREDEFINED_ERROR 0 //it should be a bool variable and 1=using predefined error and 2=computing error with the actual values of the grid

int level_max[DIMENSION],level_min[DIMENSION];
int level_1[DIMENSION];
int level_2[DIMENSION];
int _Npoints[DIMENSION];
int refined_level = 0;
double* total_error_level = new double[MAX_ADAPTATION_LEVEL](); //total error for any level initialized with zeros


class Domain{
public:
	double start_point[DIMENSION], end_point[DIMENSION];
};

Domain Original_Domain;

//function for testing interpolation values
double CaseStudies(double Z){
	return pow( Z , 2);
}
double TestFunction(double X,double Y){
	return CaseStudies(X)*CaseStudies(Y);
	//return X*X*Y*Y;
}
double ErrorFunction(double X,double Y){
	double value;
	if (		(0.25 < X && X < 0.5)		&& ( 0.5 < Y && Y < 0.75	 )	){
		value = 1.0;
	}
	else if (	(0.625 < X && X < 0.75)	&& ( 0.25 < Y && Y < 0.375 )	){
		value = 1.0;
	}
	else{
		value=0;
	}

	return value;
}


//Scheme defined here
void initialize_scheme(){
	//start of scheme
	Original_Domain.start_point[0]=0.0;
	Original_Domain.start_point[1]=0.0;
	Original_Domain.end_point[0]=1.0;
	Original_Domain.end_point[1]=1.0;

	level_1[0]=5;
	level_1[1]=2;
	level_2[0]=2;
	level_2[1]=5;

	for( int i = 0; i < DIMENSION; ++i ) {
		level_max[i]=(level_1[i]<level_2[i]) ? level_2[i] : level_1[i];
		level_min[i]=(level_1[i]<level_2[i]) ? level_1[i] : level_2[i];
	}

	for ( int i = 0; i < DIMENSION; ++i ) {
		_Npoints[i]=(pow(2,level_max[i]))+1;	//2^l+1
	}
	//end of scheme
}



#endif
