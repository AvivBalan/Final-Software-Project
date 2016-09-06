#include "SPKDTree.h"

struct sp_kd_tree_t{
	SPKDTreeNode head;
};

struct sp_kd_tree_node_t{
	int dim;
	double val;
	SPKDTreeNode left;
	SPKDTreeNode right;
	SPPoint data;
};

SPKDTree spInitTree(SPPoint* arr, int size, SDKTreeSpresd splitMethod, int spPCADimension){
	SPKDTree newTree;
	SPKDArray kdArray = NULL;
	int dim = 0;

	newTree = (SPKDTree) malloc(sizeof(*newTree)); //allocating memory for the new tree
	if(newTree == NULL) { //Allocation Fails
		return NULL;
	}

	kdArray = Init(arr, size);

	if(splitMethod == MAX_SPREAD){
		dim = spKDArrayFindMaxSpreadDim(kdArray);
	}
	else if(splitMethod == RANDOM)
	{
		srand(time(NULL));
		dim = rand() % spPCADimension;
	}
	else
	{
		//INCREMENTAL
		dim = 0;
	}

	newTree->head = spTreeBuilder(kdArray, size, dim, splitMethod, spPCADimension);

	return newTree;
}

SPKDTreeNode spTreeBuilder(SPKDArray kdArr, int size, int dim, SDKTreeSpresd splitMethod, int spPCADimension){
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
		newNode->val = -1;
		newNode->data = spKDArrayGetPoint(kdArr, 0);
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

	newNode->val = spKDArrayGetMedianValue(kdArr, dim, size);

	newDim = dim;
	if(splitMethod == INCREMENTAL){
		newDim = dim + 1;
		newDim = newDim % spPCADimension;
	}

	splitArrays = Split(kdArr, newDim);

	newNode->left = spTreeBuilder(splitArrays[0], leftSize, newDim, splitMethod, spPCADimension);
	newNode->right = spTreeBuilder(splitArrays[1], size - leftSize, newDim, splitMethod, spPCADimension);

	SPKDArrayDestroy(kdArr);

	return newNode;
}

void spKNNSearch(SPKDTree tree, SPPoint queryPoint, int spKNN, int* imgArray){
	SPBPQueue bpq;

	bpq = spBPQueueCreate(spKNN);

	spKNNSearchRecurr(tree->head, queryPoint, bpq);

	while(!spBPQueueIsEmpty(bpq)){
		SPListElement listEle = spBPQueuePeek(bpq);
		int imgIndex = spListElementGetIndex(listEle);
		spListElementDestroy(listEle);

		imgArray[imgIndex]++;
	}

	spBPQueueDestroy(bpq);

}

void spKNNSearchRecurr(SPKDTreeNode currentNode, SPPoint queryPoint, SPBPQueue bpq){
	bool leftSub = false;
	double sqrdLDist;
	SPListElement listEle;
	int maxDist;

	if(currentNode == NULL)
		return;

	listEle = spBPQueuePeekLast(bpq);
	maxDist = spListElementGetValue(listEle);
	spListElementDestroy(listEle);

	if(spIsLeaf(currentNode)){
		sqrdLDist = spPointL2SquaredDistance(currentNode->data, queryPoint);
		listEle = spListElementCreate(spPointGetIndex(currentNode->data), sqrdLDist);
		if(!spBPQueueIsFull(bpq) || maxDist > sqrdLDist){
			spBPQueueEnqueue(bpq,listEle);
		}
		else if(maxDist > sqrdLDist){
			spBPQueueEnqueue(bpq,listEle);
			spBPQueueDequeue(bpq); //??????????????????????????????????????
		}
		spListElementDestroy(listEle);
		return;
	}

	if(spPointGetAxisCoor(queryPoint, currentNode->dim) <= currentNode->val){
		spKNNSearchRecurr(currentNode->left, queryPoint, bpq);
		leftSub = true;
	}
	else{
		spKNNSearchRecurr(currentNode->right, queryPoint, bpq);
	}

	sqrdLDist = sqrt(currentNode->val - spPointGetAxisCoor(queryPoint, currentNode->dim));
	if(!spBPQueueIsFull(bpq) || maxDist > sqrdLDist){
		if(leftSub){
			spKNNSearchRecurr(currentNode->left, queryPoint, bpq);
			leftSub = true;
		}
		else{
			spKNNSearchRecurr(currentNode->right, queryPoint, bpq);
		}
	}
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
		spPointDestroy(node->data);
		free (node);
	}
}

bool spIsLeaf(SPKDTreeNode node)
{
	if(node->left == NULL && node->right == NULL)
		return true;
	else return false;
}


