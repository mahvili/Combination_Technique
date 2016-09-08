#ifndef FULLGRID_H
#define FULLGRID_H

#include "Definitions.h"
/*here Full grid structure will be build*/

class Fullgrid {
public:
	/*public member variable and function definitions. */
	int _Npoints[DIMENSION];
	double _mesh_size[DIMENSION];
	double** function_values;
	double *position_d1, *position_d2;

	/** Default constructor for the fullgrid
	 */
	Fullgrid(int* level, Domain _domain) {
		for ( int i = 0; i < DIMENSION; ++i ) {
			_Npoints[i]=(pow(2,level[i]))+1;	//2^l+1
			_mesh_size[i]= (_domain.end_point[i] - _domain.start_point[i]) / (pow(2,level[i]) );	//2^-l
		}
		position_d1 = new double[ _Npoints[0] ];
		position_d2 = new double[ _Npoints[1] ];
		//Assign first dimension
		function_values = new double*[_Npoints[0]];
		//Assign second dimension
		for(int i = 0; i <_Npoints[0] ; i++){
			function_values[i] = new double[_Npoints[1]];
		}
	}

	//copy constructor
	Fullgrid(const Fullgrid& _copyfromfullgrid) {
		for ( int i = 0; i < DIMENSION; ++i ) {
			_Npoints[i]=_copyfromfullgrid._Npoints[i];	//2^l+1
			_mesh_size[i]= _copyfromfullgrid._mesh_size[i];
		}
		position_d1 = new double[ _Npoints[0] ];
		position_d2 = new double[ _Npoints[1] ];
		function_values = new double*[_Npoints[0]];
		for(int i = 0; i <_Npoints[0] ; i++){
			function_values[i] = new double[_Npoints[1]];
		}
		for ( int j_1 = 0; j_1 < _Npoints[0]; ++j_1 ) {
			position_d1[j_1]=_copyfromfullgrid.position_d1[j_1];
		}
		for ( int j_2 = 0; j_2 < _Npoints[1]; ++j_2 ) {
			position_d2[j_2]=_copyfromfullgrid.position_d2[j_2];
		}
		//		**(_array) = new double[index_size_global]();  ///Zero-initialize (somehow doesn't work here)
		for ( int i = 0; i < _Npoints[0]; ++i ) {
			for ( int j = 0; j < _Npoints[1]; ++j ) {
				function_values[i][j]= _copyfromfullgrid.function_values[i][j];  //X^2*Y^2 - TASK (implement new function to do here)
			}
		}

	}

	//Assignment operator
	Fullgrid& operator=(const Fullgrid& _equalfullgrid){
		if (this!=&_equalfullgrid){
			for ( int i = 0; i < DIMENSION; ++i ) {
				_Npoints[i]=_equalfullgrid._Npoints[i];	//2^l+1
				_mesh_size[i]= _equalfullgrid._mesh_size[i];
			}
			position_d1 = new double[ _Npoints[0] ];
			position_d2 = new double[ _Npoints[1] ];
			function_values = new double*[_Npoints[0]];
			//Assign second dimension
			for(int i = 0; i <_Npoints[0] ; i++){
				function_values[i] = new double[_Npoints[1]];
			}

			for ( int j_1 = 0; j_1 < _Npoints[0]; ++j_1 ) {
				position_d1[j_1]=_equalfullgrid.position_d1[j_1];
			}
			for ( int j_2 = 0; j_2 < _Npoints[1]; ++j_2 ) {
				position_d2[j_2]=_equalfullgrid.position_d2[j_2];
			}
			//		**(_array) = new double[index_size_global]();  ///Zero-initialize (somehow doesn't work here)
			for ( int i = 0; i < _Npoints[0]; ++i ) {
				for ( int j = 0; j < _Npoints[1]; ++j ) {
					function_values[i][j]= _equalfullgrid.function_values[i][j];  //X^2*Y^2 - TASK (implement new function to do here)
				}
			}
		}
		return *this;
	}

	/** Destructor Deallocates the data
	 * TASK
	 */
	virtual ~Fullgrid() {
		delete[] function_values;
		delete[] position_d1;
		delete[] position_d2;
	}

	/*evaluates function value for TestFunction defined in definitions*/
	void evaluate(Domain _domain){

		for ( int j_1 = 0; j_1 < _Npoints[0]; ++j_1 ) {
			position_d1[j_1]=_domain.start_point[0]+j_1*_mesh_size[0];
		}
		for ( int j_2 = 0; j_2 < _Npoints[1]; ++j_2 ) {
			position_d2[j_2]=_domain.start_point[1]+j_2*_mesh_size[1];
		}
		//		**(_array) = new double[index_size_global]();  ///Zero-initialize (somehow doesn't work here)
		for ( int i = 0; i < _Npoints[0]; ++i ) {
			for ( int j = 0; j < _Npoints[1]; ++j ) {
				function_values[i][j]= TestFunction(position_d1[i],position_d2[j]);  //X^2*Y^2 - TASK (implement new function to do here)
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
private:

};

#endif
