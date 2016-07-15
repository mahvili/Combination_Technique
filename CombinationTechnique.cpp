//============================================================================
// Name        : CombinationTechnique.cpp
// Author      : MahyarValizadeh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "Combination.h"
#include "visual.c"

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

	//Regular fullgrid
	Fullgrid* _fullgrid = new Fullgrid(level_max, domain);
	_fullgrid->evaluate(domain);
	Combination* _combinationSolution= new Combination(level_1,level_2,level_max,level_min, domain);

	//outputs before combination
	write_vtkFile("fullgrid_noCombi", domain.end_point[0]-domain.start_point[0],domain.end_point[1]-domain.start_point[1],_fullgrid->_Npoints[0]-1,_fullgrid->_Npoints[1]-1,_fullgrid->_mesh_size[0],_fullgrid->_mesh_size[1], _fullgrid->function_values);
	printf ("%s \n", "before combi!");
	//using combination technique

	write_vtkFile("l1_noCombi", domain.end_point[0]-domain.start_point[0],domain.end_point[1]-domain.start_point[1],_combinationSolution->_grid_l1._Npoints[0]-1,_combinationSolution->_grid_l1._Npoints[1]-1,_combinationSolution->_grid_l1._mesh_size[0],_combinationSolution->_grid_l1._mesh_size[1], _combinationSolution->_grid_l1.function_values);
	printf ("%s \n", "vtk l1 done!");
	write_vtkFile("l2_noCombi", domain.end_point[0]-domain.start_point[0],domain.end_point[1]-domain.start_point[1],_combinationSolution->_grid_l2._Npoints[0]-1,_combinationSolution->_grid_l2._Npoints[1]-1,_combinationSolution->_grid_l2._mesh_size[0],_combinationSolution->_grid_l2._mesh_size[1], _combinationSolution->_grid_l2.function_values);
	printf ("%s \n", "vtk l2 done!");
	write_vtkFile("lmin_noCombi", domain.end_point[0]-domain.start_point[0],domain.end_point[1]-domain.start_point[1],_combinationSolution->_grid_lmin._Npoints[0]-1,_combinationSolution->_grid_lmin._Npoints[1]-1,_combinationSolution->_grid_lmin._mesh_size[0],_combinationSolution->_grid_lmin._mesh_size[1], _combinationSolution->_grid_lmin.function_values);
	printf ("%s \n", "vtk lmin done!");
	printf ("%s \n", "before getcombi!");



	_combinationSolution->GetCombination(level_1,level_2, domain);

	printf ("%s \n", "before VTK!");
	write_vtkFile("fullgrid_Combi", domain.end_point[0]-domain.start_point[0],domain.end_point[1]-domain.start_point[1],_fullgrid->_Npoints[0]-1,_fullgrid->_Npoints[1]-1,_fullgrid->_mesh_size[0],_fullgrid->_mesh_size[1], _combinationSolution->_grid_combination.function_values);
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
	write_vtkFile("fullgrid_difference", domain.end_point[0]-domain.start_point[0],domain.end_point[1]-domain.start_point[1],_fullgrid->_Npoints[0]-1,_fullgrid->_Npoints[1]-1,_fullgrid->_mesh_size[0],_fullgrid->_mesh_size[1], _difference_grid->function_values);
	printf ("%s \n", "everything is done!");
	//delete _combinationSolution;
	//delete _fullgrid;
	return 0;
}
