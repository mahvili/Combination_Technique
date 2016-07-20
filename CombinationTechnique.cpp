//============================================================================
// Name        : CombinationTechnique.cpp
// Author      : MahyarValizadeh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "Combination.h"
#include "visual.h"

int main( int argc, char *argv[] ) {

	/*error handling - TASK
     if ( argc < a ) {
        handleError( );
    }
	 */
	Domain domain;
	int level_max[DIMENSION],level_min[DIMENSION];
	int level_1[DIMENSION];
	int level_2[DIMENSION];

	//start of scheme
	domain.start_point[0]=0.0;
	domain.start_point[1]=0.0;
	domain.end_point[0]=1.0;
	domain.end_point[1]=1.0;
	//TASK:define l1 and l2
	level_1[0]=5;
	level_1[1]=2;
	level_2[0]=2;
	level_2[1]=5;
	//end of scheme

	for( int i = 0; i < DIMENSION; ++i ) {
		level_max[i]=(level_1[i]<level_2[i]) ? level_2[i] : level_1[i];
		level_min[i]=(level_1[i]<level_2[i]) ? level_1[i] : level_2[i];
	}

	//Regular Fullgrid
	Fullgrid* _fullgrid = new Fullgrid(level_max, domain);
	_fullgrid->evaluate(domain);
	//outputs before combination
	write_vtkFile("fullgrid_noCombi", domain, _fullgrid);
	printf ("%s \n", "before combination!");


	//using combination technique
	Combination* _combinationSolution= new Combination(level_1,level_2,level_max,level_min, domain);
	_combinationSolution->GetCombination(level_1,level_2, domain);
	write_vtkFile("fullgrid_Combi", domain, &_combinationSolution->_grid_combination);
	printf ("%s \n", "after combination!");


	//plot the difference
	int _Npoints[DIMENSION];
	for ( int i = 0; i < DIMENSION; ++i ) {
		_Npoints[i]=(pow(2,level_max[i]))+1;	//2^l+1
	}
	Fullgrid* _difference_grid = new Fullgrid(level_max, domain);
	for ( int i = 0; i < _Npoints[0]; ++i ) {
		for ( int j = 0; j < _Npoints[1]; ++j ) {
			_difference_grid->function_values[i][j]=_combinationSolution->_grid_combination.function_values[i][j] - _fullgrid->function_values[i][j];
		}
	}
	write_vtkFile("fullgrid_difference", domain, _difference_grid);
	printf ("%s \n", "after difference!");


	//higher resolution test
	Domain new_domain;
	new_domain.start_point[0]=0.5;
	new_domain.start_point[1]=0.5;
	new_domain.end_point[0]=1.0;
	new_domain.end_point[1]=1.0;

	Fullgrid* new_fullgrid = new Fullgrid(level_max, new_domain);
	new_fullgrid->evaluate(new_domain);
	//outputs before combination
	write_vtkFile("fullgrid_highernoCombi", new_domain, new_fullgrid);
	printf ("%s \n", "before higher resolution combination!");
	//using combination technique
	Combination* new_combinationSolution= new Combination(level_1,level_2,level_max,level_min, new_domain);
	new_combinationSolution->GetCombination(level_1,level_2, new_domain);
	write_vtkFile("fullgrid_higherCombi", new_domain, &new_combinationSolution->_grid_combination);
	printf ("%s \n", "after higher resolution combination!");



	//plot the difference
	Fullgrid* new_difference_grid = new Fullgrid(level_max, new_domain);
	for ( int i = 0; i < _Npoints[0]; ++i ) {
		for ( int j = 0; j < _Npoints[1]; ++j ) {
			new_difference_grid->function_values[i][j]=new_combinationSolution->_grid_combination.function_values[i][j] - new_fullgrid->function_values[i][j];
		}
	}
	write_vtkFile("fullgrid_higherdifference", new_domain, new_difference_grid);
	printf ("%s \n", "after higher resolution difference!");

	printf ("%s \n", "everything is done!");
	//delete _combinationSolution;
	//delete _fullgrid;
	return 0;
}
