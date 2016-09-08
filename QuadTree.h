#ifndef QUADTREE_H
#define QUADTREE_H

#include "Definitions.h"
#include "Combination.h"
#include "Visual.h"
#include <math.h>

class Node{
public:
	bool refined_factor; //whether the node should be refined or not 0=no refinement 1=to be refined
	double total_error_in_node; //total error calculated in the subtree
	double total_error_in_node_using_predefined_function; //
	int node_level;
	string name;
	Domain domain;


	Fullgrid* beforecombi_fullgrid;
	Fullgrid* aftercombi_fullgrid;
	Fullgrid* difference_grid;

	Node* parent_node;
	Node* NW_child_node;
	Node* NE_child_node;
	Node* SE_child_node;
	Node* SW_child_node;


	//constructor
	Node(): parent_node(NULL),NW_child_node(NULL),NE_child_node(NULL),SE_child_node(NULL),SW_child_node(NULL){};

};


#endif
