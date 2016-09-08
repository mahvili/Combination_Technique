//============================================================================
// Name        : CombinationTechnique Project
// Author      : MahyarValizadeh
// Version     :
// Copyright   : Your copyright notice
// Description : Adaptive Combination Technique for interpolation problem
//============================================================================

#include "Solution.h"

int main( int argc, char *argv[] ) {

	initialize_scheme();

	Node* root = new Node();

	WRITE_VTK=1;
	MAX_ADAPTATION_LEVEL=3;
	if (root->parent_node==NULL){
		root->domain = Original_Domain; //check it is actually root
		root->node_level=0;
		root->name="root";
	}
	AdaptiveRefinement(root);

	WRITE_VTK=0;
	for (MAX_ADAPTATION_LEVEL = 0; MAX_ADAPTATION_LEVEL< 5;MAX_ADAPTATION_LEVEL++)
	{
		AdaptiveRefinement(root);
		printf("error with highest level %i=%f\n",MAX_ADAPTATION_LEVEL,compute_error(root));
	}
	printf ("%s \n", "everything is done!");

	return 0;
}
