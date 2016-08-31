#include "SPKDTree.h"

struct sp_kd_tree_t{
	SPKDTreeNode head;
};

struct sp_kd_tree_node_t{
	int dim;
	double val;
	SPKDTreeNode left;
	SPKDTreeNode right;
	SPPoint* data;
};

SPKDTree InitTree(SPPoint* arr, int size, int splitMethod){
	SPKDTree newTree;
	SPKDArray kdArray;
	int dim;

	newTree = (SPKDTree) malloc(sizeof(*newTree)); //allocating memory for the new tree
	if(newTree == NULL) { //Allocation Fails
		return NULL;
	}

	kdArray = Init(arr, size);

	if(splitMethod == 0){
		//MAX_SPREAD
		//dim = FindMaxSpread(kdArray);
		dim = 0; //???????????????????????????????????????????????
	}
	else if(splitMethod == 1)
	{
		//RANDOM
		dim = 0; //??????????????????????????????????????????????????
	}
	else
	{
		//INCREMENTAL
		dim = 0;
	}

	newTree->head = TreeBuilder(kdArray, size, dim, splitMethod);

	return newTree;
}

SPKDTreeNode TreeBuilder(SPKDArray kdArr, int size, int dim, int splitMethod){
	SPKDTreeNode newNode;
	SPKDArray* splitArrays;
	int leftSize, newDim;

	newNode = (SPKDTreeNode) malloc(sizeof(*newNode)); //allocating memory for the new node
	if(newNode == NULL) { //Allocation Fails
		return NULL;
	}

	if(size == 1){
		newNode->dim = -1;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->val = -1; //invalid???????????????????????????????????????????????????????
		newNode->data = kdArr->p[0]; //??????????????????????????????????????????????????? pointer to point
		return newNode;
	}

	newNode->dim = dim;
	newNode->data = NULL;

	if (size%2 == 0){
		leftSize = size/2 - 1;
	}
	else{
		leftSize = floor(size/2);
	}

	newNode->val = kdArr->p[kdArr->matrix[dim][leftSize]];

	newDim = dim;
	if(splitMethod == 2){
		newDim = dim + 1;
		newDim = newDim % kdArr->p[0]->dim;
	}

	splitArrays = Split(kdArr, newDim);

	newNode->left = TreeBuilder(splitArrays[0], leftSize, newDim, splitMethod);
	newNode->right = TreeBuilder(splitArrays[1], size - leftSize, newDim, splitMethod);

	SPKDArrayDestroy(kdArr);

	return newNode;
}

void spKDTreeDestroy(SPKDTree tree){
	if(tree != NULL){
		spKDTreeDestroyNode(tree->head);
		free(tree);
	}
}

void spKDTreeDestroyNode(SPKDTreeNode node){
	if(node != NULL){
		spKDTreeDestroyNode(node->left);
		spKDTreeDestroyNode(node->right);
		free (node);
	}
}
