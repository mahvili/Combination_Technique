#ifndef FULLGRID_H
#define FULLGRID_H

#include "Definitions.h"
/*here Full grid structure will be build*/

class Fullgrid {
protected:
	/*protected member variable and function definitions. */



public:
	/*public member variable and function definitions. */
	int _Npoints[DIMENSION];
	FLOAT _mesh_size[DIMENSION];
	FLOAT** function_values;
	FLOAT *position_d1, *position_d2;

	/** Default constructor for the fullgrid
	 */
	Fullgrid(int* level, Domain _domain) {
		for ( int i = 0; i < DIMENSION; ++i ) {
			_Npoints[i]=(pow(2,level[i]))+1;	//2^l+1
			_mesh_size[i]= (_domain.end_point[i] - _domain.start_point[i]) / (pow(2,level[i]) );	//2^-l
		}
		position_d1 = new FLOAT[ _Npoints[0] ];
		position_d2 = new FLOAT[ _Npoints[1] ];
		//Assign first dimension
		function_values = new FLOAT*[_Npoints[0]];
		//Assign second dimension
		for(int i = 0; i <_Npoints[0] ; i++){
			function_values[i] = new FLOAT[_Npoints[1]];
		}
	}

	//Assignment operator
	Fullgrid& operator=(const Fullgrid& fullgrid);

	/** Destructor Deallocates the data
	 * TASK
	 */
	virtual ~Fullgrid() {
		delete[] function_values;
		delete[] position_d1;
		delete[] position_d2;
	}

	/*evaluates function value for Gaussian case X^2*Y^2*/
	void evaluate(Domain _domain){
		for ( int j_1 = 0; j_1 < _Npoints[0]; ++j_1 ) {
			position_d1[j_1]=_domain.start_point[0]+j_1*_mesh_size[0];
		}
		for ( int j_2 = 0; j_2 < _Npoints[1]; ++j_2 ) {
			position_d2[j_2]=_domain.start_point[1]+j_2*_mesh_size[1];
		}
		//		**(_array) = new FLOAT[index_size_global]();  ///Zero-initialize (somehow doesn't work here)
		for ( int i = 0; i < _Npoints[0]; ++i ) {
			for ( int j = 0; j < _Npoints[1]; ++j ) {
				function_values[i][j]= position_d1[i]*position_d1[i]*position_d2[j]*position_d2[j];  //X^2*Y^2
			}
		}
	}

	/** notice: by introducing domain it may need to change accordingly by adding level of domain
	by putting level_max it will give the index for full grid
	 */
	/** here a multidimensional indices which is stored in 1D array called "index" is mapped to a value to use in grid
	 *  TASK
	 */
	/**
	int Map2arrayIndex_local(int * index, int*level)  {
		int multiplier=1,mapped_index=0;
		for loop over dimensions to get to index
		for ( int i = 0; i < DIMENSION; ++i ) {
			_Npoints[i]=(pow(2,level[i]))+1;	//2^l+1
			if (i>0){
				multiplier*=_Npoints[i-1];
			}
			mapped_index+=index[i]*multiplier;
		}
		return mapped_index;
	}
	 */

};

#endif
