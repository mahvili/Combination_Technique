#ifndef SOLUTION_H
#define SOLUTION_H

#include "QuadTree.h"
#include "Definitions.h"
#include "Combination.h"
#include "Visual.h"
#include <math.h>
#include <iostream>
using namespace std;

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool solve(Node* current_node);

void expand_Quad(Node* current_node);

double evaluate_predefinedError(Domain new_domain);

void AdaptiveRefinement(Node* current_node);

bool solve(Node* current_node){

	current_node->beforecombi_fullgrid	= new Fullgrid(level_max, current_node->domain);
	current_node->aftercombi_fullgrid	= new Fullgrid(level_max, current_node->domain);
	current_node->difference_grid		= new Fullgrid(level_max, current_node->domain);

	current_node->beforecombi_fullgrid->evaluate(current_node->domain);

	Combination* _combinationSolution= new Combination(level_1,level_2,level_max,level_min,current_node->domain);
	_combinationSolution->GetCombination(level_1,level_2, current_node->domain);
	current_node->aftercombi_fullgrid=&_combinationSolution->_grid_combination;

	//**************************Calculation of difference value between sparse grid combination and normal full grid and computing total error of the node
	current_node->total_error_in_node=0;
	for ( int i = 0; i < _Npoints[0]; ++i ) {
		for ( int j = 0; j < _Npoints[1]; ++j ) {
			current_node->difference_grid->function_values[i][j]= current_node->beforecombi_fullgrid->function_values[i][j] - current_node->aftercombi_fullgrid->function_values[i][j];
			if (current_node->difference_grid->function_values[i][j]<0){
				current_node->total_error_in_node-=current_node->difference_grid->function_values[i][j];//if it's negative then make it positive and add (sum + absolute(x)) = sum - x
			}
			else{
				current_node->total_error_in_node+=current_node->difference_grid->function_values[i][j];//if it's positive then just add (sum + absolute(x)) = sum + x
			}

		}
	}
	//**************************
	if (USE_PREDEFINED_ERROR==0){ //if we use predefined then we don't use the error calculated
		if (current_node->total_error_in_node>TRESHOLD_ERROR && current_node->node_level<MAX_ADAPTATION_LEVEL){   //if error is not satisfied or we didn't go too far then make a new refinement
			current_node->refined_factor=1;
		}
		else {
			current_node->refined_factor=0;
		}
	}
	else if (USE_PREDEFINED_ERROR==1){
		current_node->total_error_in_node_using_predefined_function = evaluate_predefinedError(current_node->domain);
		if (current_node->total_error_in_node_using_predefined_function>TRESHOLD_ERROR && current_node->node_level<MAX_ADAPTATION_LEVEL){   //if error is not satisfied or we didn't go too far then make a new refinement
			current_node->refined_factor=1;
		}
		else {
			current_node->refined_factor=0;
		}
	}


	//if only we need refining then we draw that branch


	if (current_node->refined_factor==0){
		if (WRITE_VTK==1){
			struct stat st_results = {0};
			if (stat("Results", &st_results) == -1) {
				mkdir("Results", 0700);
			}
			//vtk with subdirectory for no combination
			struct stat st_nocombi = {0};
			if (stat("Results/no_combination", &st_nocombi) == -1) {
				mkdir("Results/no_combination", 0700);
			}
			string fullgrid_name="./Results/no_combination/noCombi"  + std::to_string(current_node->node_level) +  current_node->name;
			const char *cfullgrid_name = fullgrid_name.c_str();
			write_vtkFile(cfullgrid_name,current_node->domain,current_node->beforecombi_fullgrid);
			//vtk with subdirectory for combination
			struct stat st_combi = {0};
			if (stat("Results/combination", &st_combi) == -1) {
				mkdir("Results/combination", 0700);
			}
			string combigrid_name="./Results/combination/Combi"  + std::to_string(current_node->node_level) +  current_node->name;
			const char *ccombigrid_name = combigrid_name.c_str();
			write_vtkFile(ccombigrid_name,current_node->domain,current_node->aftercombi_fullgrid);
			//vtk with subdirectory for no combination
			struct stat st_diff = {0};
			if (stat("Results/difference_grid", &st_diff) == -1) {
				mkdir("Results/difference_grid", 0700);
			}
			string diffgrid_name="./Results/difference_grid/difference" + std::to_string(current_node->node_level) + current_node->name;
			const char *cdiffgrid_name = diffgrid_name.c_str();
			write_vtkFile(cdiffgrid_name,current_node->domain,current_node->difference_grid);
		}
	}

	return current_node->refined_factor;
}

void expand_Quad(Node* current_node){

	//making this node parent of all children
	current_node->SW_child_node = new Node();
	current_node->SE_child_node = new Node();
	current_node->NW_child_node = new Node();
	current_node->NE_child_node = new Node();
	current_node->SW_child_node->parent_node=current_node;
	current_node->SE_child_node->parent_node=current_node;
	current_node->NW_child_node->parent_node=current_node;
	current_node->NE_child_node->parent_node=current_node;
	//naming scheme
	current_node->SW_child_node->name=current_node->name+"SW";
	current_node->SE_child_node->name=current_node->name+"SE";
	current_node->NW_child_node->name=current_node->name+"NW";
	current_node->NE_child_node->name=current_node->name+"NE";
	//Node level is added one time
	current_node->SW_child_node->node_level=current_node->node_level+1;
	current_node->SE_child_node->node_level=current_node->node_level+1;
	current_node->NW_child_node->node_level=current_node->node_level+1;
	current_node->NE_child_node->node_level=current_node->node_level+1;
	//Setting new domains for all four children
	//South West child
	current_node->SW_child_node->domain.start_point[0] = current_node->domain.start_point[0];
	current_node->SW_child_node->domain.start_point[1] = current_node->domain.start_point[1];
	current_node->SW_child_node->domain.end_point[0]   = (current_node->domain.start_point[0]+current_node->domain.end_point[0])/2;
	current_node->SW_child_node->domain.end_point[1]   = (current_node->domain.start_point[1]+current_node->domain.end_point[1])/2;
	//South East child
	current_node->SE_child_node->domain.start_point[0] = (current_node->domain.start_point[0]+current_node->domain.end_point[0])/2;
	current_node->SE_child_node->domain.start_point[1] = current_node->domain.start_point[1];
	current_node->SE_child_node->domain.end_point[0]   = current_node->domain.end_point[0];
	current_node->SE_child_node->domain.end_point[1]   = (current_node->domain.start_point[1]+current_node->domain.end_point[1])/2;
	//North West Child
	current_node->NW_child_node->domain.start_point[0] = current_node->domain.start_point[0];
	current_node->NW_child_node->domain.start_point[1] = (current_node->domain.start_point[1]+current_node->domain.end_point[1])/2;
	current_node->NW_child_node->domain.end_point[0]   = (current_node->domain.start_point[0]+current_node->domain.end_point[0])/2;
	current_node->NW_child_node->domain.end_point[1]   = current_node->domain.end_point[1];
	//North East Child
	current_node->NE_child_node->domain.start_point[0] = (current_node->domain.start_point[0]+current_node->domain.end_point[0])/2;
	current_node->NE_child_node->domain.start_point[1] = (current_node->domain.start_point[1]+current_node->domain.end_point[1])/2;
	current_node->NE_child_node->domain.end_point[0]   = current_node->domain.end_point[0];
	current_node->NE_child_node->domain.end_point[1]   = current_node->domain.end_point[1];
}

double evaluate_predefinedError(Domain new_domain){
	double _error=0;
	//calculation of positions might be for no reason after evaluate function has been called once
	int Ncell[DIMENSION];
	double mesh_size_domain[DIMENSION];
	double *cellposition_d1, *cellposition_d2;

	for ( int i = 0; i < DIMENSION; ++i ) {
		Ncell[i]=(pow(2,level_max[i]));	//2^l cell
		mesh_size_domain[i]= (new_domain.end_point[i] - new_domain.start_point[i]) / (pow(2,level_max[i]) );	//2^-l
	}
	cellposition_d1 = new double[ Ncell[0] ];
	cellposition_d2 = new double[ Ncell[1] ];

	for ( int j_1 = 0; j_1 < Ncell[0]; ++j_1 ) {
		cellposition_d1[j_1]=new_domain.start_point[0]+j_1*mesh_size_domain[0]+mesh_size_domain[0]/2.0;
	}
	for ( int j_2 = 0; j_2 < Ncell[1]; ++j_2 ) {
		cellposition_d2[j_2]=new_domain.start_point[1]+j_2*mesh_size_domain[1]+mesh_size_domain[1]/2.0;
	}
	for ( int i = 0; i < _Npoints[0]; ++i ) {
		for ( int j = 0; j < _Npoints[1]; ++j ) {
			_error+= ErrorFunction(cellposition_d1[i],cellposition_d2[j]);  //X^2*Y^2 - TASK (implement new function to do here)
		}
	}
	return _error;
}

void AdaptiveRefinement(Node* current_node){

	current_node->refined_factor=solve(current_node);

	if (current_node->refined_factor==1)
	{
		expand_Quad(current_node);

		AdaptiveRefinement(current_node->SW_child_node);
		AdaptiveRefinement(current_node->SE_child_node);
		AdaptiveRefinement(current_node->NW_child_node);
		AdaptiveRefinement(current_node->NE_child_node);
	}

}

double compute_error(Node* current_node){
	static int j=0;

	double current_error=0;
	if (current_node->refined_factor==0){
		current_error+=current_node->total_error_in_node;
		j++;
		//cout<<"current one we are at j= "<< j <<" with name= "<<current_node->name<<endl;
		//cout<<"total error in node was=" << current_node->total_error_in_node<<endl;
		const char *temp = current_node->name.c_str();
		if (USE_PREDEFINED_ERROR==1){printf ("total error predefined for %s =  %f\n",temp,current_node->total_error_in_node_using_predefined_function);	}

	}
	else if (current_node->refined_factor==1)
	{
		current_error+=compute_error(current_node->SW_child_node);
		current_error+=compute_error(current_node->SE_child_node);
		current_error+=compute_error(current_node->NW_child_node);
		current_error+=compute_error(current_node->NE_child_node);
	}

	return current_error;
}

#endif

