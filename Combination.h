/*
 * Combination.c
 *
 *  Created on: Jun 3, 2015
 *      Author: mahyar
 */
#ifndef COMBINATION_H
#define COMBINATION_H

#include "Fullgrid.h"
#include <stdio.h>

class Combination {
private:
	/*private member variable and function definitions. */

protected:
	/*protected member variable and function definitions. */
	int level_max[DIMENSION],level_min[DIMENSION];
	int _Npoints[DIMENSION];

public:
	/*public member variable and function definitions. */
	Fullgrid _grid_l1 , _grid_l2 , _grid_lmin;
	Fullgrid _grid_l1_interpolated , _grid_l2_interpolated , _grid_lmin_interpolated;
	Fullgrid _grid_combination;

	/** Constructor for the Combination */
	Combination(int* level_1,int* level_2,int* _level_max, int* _level_min, Domain domain)
	:_grid_l1(level_1, domain),_grid_l2(level_2, domain),_grid_lmin(_level_min, domain)
	,_grid_l1_interpolated(_level_max, domain),_grid_l2_interpolated(_level_max, domain)
	,_grid_lmin_interpolated(_level_max, domain),_grid_combination(_level_max, domain){

		for ( int i = 0; i < DIMENSION; ++i ) {
			level_max[i]=_level_max[i];
			level_min[i]=_level_min[i];
		}
		//evaluate case function for each grids needed to be combined
		_grid_l1.evaluate(domain);
		_grid_l2.evaluate(domain);
		_grid_lmin.evaluate(domain);

	}

	/** Destructor Deallocates the data
	 * TASK
	 *  */
	virtual ~Combination() {
		//delete[] ;
	}

	void GetCombination(int* level_1,int* level_2, Domain domain){
		/*apply the method iterate*/
		for ( int i = 0; i < DIMENSION; ++i ) {
			_Npoints[i]=(pow(2,level_max[i]))+1;	//2^l+1
		}
		direct_injection(level_1,_grid_l1,domain,&_grid_l1_interpolated);
		direct_injection(level_2,_grid_l2,domain,&_grid_l2_interpolated);
		direct_injection(level_min,_grid_lmin,domain,&_grid_lmin_interpolated);

		interpolation(level_1,&_grid_l1_interpolated, domain);
		interpolation(level_2,&_grid_l2_interpolated, domain);
		interpolation(level_min,&_grid_lmin_interpolated, domain);

		for ( int i = 0; i < _Npoints[0]; ++i ) {
			for ( int j = 0; j < _Npoints[1]; ++j ) {
				_grid_combination.function_values[i][j]= _grid_l1_interpolated.function_values[i][j] + _grid_l2_interpolated.function_values[i][j] - _grid_lmin_interpolated.function_values[i][j];
			}
		}

	}

	void interpolation(int* level, Fullgrid* interpolated_grid, Domain domain){

		int lower_local_index[DIMENSION], upper_local_index[DIMENSION];
		//interploation in X direction
		for ( int i = 0; i < _Npoints[0]-1; ++i ) {
			for ( int j = 0; j < _Npoints[1]; ++j ) {
				lower_local_index[0]=(int) floor( i/( pow( 2,level_max[0]-level[0] ) ) ) ;
				upper_local_index[0]=lower_local_index[0]+1;
				interpolated_grid->function_values[i][j]= ((double) ( upper_local_index[0] * ( pow( 2,level_max[0]-level[0] ) ) - i))/((double) ( pow( 2,level_max[0]-level[0] ) )) * interpolated_grid->function_values[(int) (lower_local_index[0]* pow( 2,level_max[0]-level[0] ) )][j]+ ((double) ( i - lower_local_index[0] * ( pow( 2,level_max[0]-level[0] ) ) ))/((double) ( pow( 2,level_max[0]-level[0] ) )) * interpolated_grid->function_values[(int) (upper_local_index[0]*pow( 2,level_max[0]-level[0] ))][j];
			}
		}
		//interploation in Y direction
		for ( int i = 0; i < _Npoints[0]; ++i ) {
			for ( int j = 0; j < _Npoints[1]-1; ++j ) {
				lower_local_index[1]=(int) floor( j/( pow( 2,level_max[1]-level[1] ) ) );
				upper_local_index[1]=lower_local_index[1]+1;
				interpolated_grid->function_values[i][j]= ((double) ( upper_local_index[1] * ( pow( 2,level_max[1]-level[1] ) ) - j))/((double) ( pow( 2,level_max[1]-level[1] ) )) * interpolated_grid->function_values[i][(int) (lower_local_index[1]*pow( 2,level_max[1]-level[1] ))]+ ((double) ( j - lower_local_index[1] * ( pow( 2,level_max[1]-level[1] ) ) ))/((double) ( pow( 2,level_max[1]-level[1] ) )) * interpolated_grid->function_values[i][(int) (upper_local_index[1]*pow( 2,level_max[1]-level[1] ))];
			}
		}

	}

	void direct_injection(int* level, Fullgrid _grid, Domain domain, Fullgrid* _projected_grid){

		// Initialize to zero
		for ( int i = 0; i < _Npoints[0]; ++i ) {
			for ( int j = 0; j < _Npoints[1]; ++j ) {
				_projected_grid->function_values[i][j]= 0;
			}
		}

		//for all local values we directly project to global
		int _Npoints_local[DIMENSION];
		for ( int i = 0; i < DIMENSION; ++i ) {
			_Npoints_local[i]=(pow(2,level[i]))+1;	//2^l+1
		}

		for ( int i = 0; i < _Npoints_local[0]; ++i ) {
			for ( int j = 0; j < _Npoints_local[1]; ++j ) {
				_projected_grid->function_values[ (int) (i*pow( 2,level_max[0]-level[0] ))][ (int) (j*pow( 2,level_max[1]-level[1] )) ]= _grid.function_values[i][j];
			}
		}
	}

};

#endif
