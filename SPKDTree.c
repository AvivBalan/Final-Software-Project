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

SPKDTree spKDTreeCreate(SPPoint* arr, int size, SDKTreeSpresd splitMethod, int spPCADimension){
	SPKDTree newTree;
	SPKDArray kdArray = NULL;
	int dim = 0;

	newTree = (SPKDTree) malloc(sizeof(*newTree)); //allocating memory for the new tree
	if(newTree == NULL) { //Allocation Fails
		return NULL;
	}

	kdArray = spKDArrayCreate(arr, size);
	if(kdArray == NULL){
		//ERROR
		free(newTree);
		return NULL;
	}

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

	newTree->head = spKDTreeBuilder(kdArray, size, dim, splitMethod, spPCADimension);
	if(newTree->head == NULL){
		//ERROR
		spKDArrayDestroy(kdArray);
		free(newTree);
		return NULL;
	}

	return newTree;
}

SPKDTreeNode spKDTreeBuilder(SPKDArray kdArr, int size, int dim, SDKTreeSpresd splitMethod, int spPCADimension){
	SPKDTreeNode newNode;
	SPKDArray* splitArrays;
	int rightSize, newDim;

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
		if(newNode->data == NULL){
			//ERROR
			spKDTreeDestroyNode(newNode);
			return NULL;
		}
		return newNode;
	}

	newNode->dim = dim;
	newNode->data = NULL;

	rightSize = floor(size/2);
	newNode->val = spKDArrayGetMedianValue(kdArr, dim);

	splitArrays = spKDArraySplit(kdArr, dim);
	if(splitArrays == NULL){
		//ERROR
		spKDTreeDestroyNode(newNode);
		return NULL;
	}

	newDim = dim;
	if(splitMethod == INCREMENTAL){
		newDim = dim + 1;
		newDim = newDim % spPCADimension;
	}

	newNode->left = spKDTreeBuilder(splitArrays[0], size - rightSize, newDim, splitMethod, spPCADimension);
	newNode->right = spKDTreeBuilder(splitArrays[1], rightSize, newDim, splitMethod, spPCADimension);

	free(splitArrays);
	spKDArrayDestroy(kdArr);

	return newNode;
}

int spKDTreeKNNSearch(SPKDTree tree, SPPoint queryPoint, int spKNN, int* imgArray){
	SPBPQueue bpq;

	bpq = spBPQueueCreate(spKNN);
	if(bpq == NULL){
		//ERROR
		return 1;
	}

	if(spKDTreeKNNSearchRecurr(tree->head, queryPoint, bpq) == 1){
		//ERROR
		return 1;
	}

	while(!spBPQueueIsEmpty(bpq)){
		SPListElement listEle = spBPQueuePeek(bpq);
		int imgIndex = spListElementGetIndex(listEle);
		spListElementDestroy(listEle);
		spBPQueueDequeue(bpq);

		imgArray[imgIndex]++;
	}

	spBPQueueDestroy(bpq);
	return 0;
}

int spKDTreeKNNSearchRecurr(SPKDTreeNode currentNode, SPPoint queryPoint, SPBPQueue bpq){
	bool leftSub = false;
	double sqrdLDist;
	SPListElement listEle;
	int maxDist;

	if(currentNode == NULL)
		return 1;

	maxDist = spListElementGetValue(spBPQueuePeekLast(bpq));

	if(spKDTreeIsLeaf(currentNode)){
		sqrdLDist = spPointL2SquaredDistance(currentNode->data, queryPoint);
		if(!spBPQueueIsFull(bpq) || maxDist > sqrdLDist){
			if(spBPQueueIsFull(bpq))
				spBPQueueMaxDequeue(bpq);
			listEle = spListElementCreate(spPointGetIndex(currentNode->data), sqrdLDist);
			spBPQueueEnqueue(bpq,listEle);
		}
		spListElementDestroy(listEle);
		return 0;
	}

	if(spPointGetAxisCoor(queryPoint, currentNode->dim) <= currentNode->val){
		if(spKDTreeKNNSearchRecurr(currentNode->left, queryPoint, bpq) == 1){
			//ERROR
			return 1;
		}
		leftSub = true;
	}
	else{
		if(spKDTreeKNNSearchRecurr(currentNode->right, queryPoint, bpq) == 1){
			//ERROR
			return 1;
		}
	}

	sqrdLDist = sqrt(currentNode->val - spPointGetAxisCoor(queryPoint, currentNode->dim));
	if(!spBPQueueIsFull(bpq) || maxDist > sqrdLDist){
		if(leftSub){
			if(spKDTreeKNNSearchRecurr(currentNode->right, queryPoint, bpq) == 1){
				//ERROR
				return 1;
			}
		}
		else{
			if(spKDTreeKNNSearchRecurr(currentNode->left, queryPoint, bpq) == 1){
				//ERROR
				return 1;
			}
		}
	}
	return 0;
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

bool spKDTreeIsLeaf(SPKDTreeNode node)
{
	if(node->left == NULL && node->right == NULL)
		return true;
	else return false;
}


