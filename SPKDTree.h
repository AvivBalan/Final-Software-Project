#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"
#include <time.h>
#include <stdlib.h>

typedef struct sp_kd_tree_t* SPKDTree;

typedef struct sp_kd_tree_node_t* SPKDTreeNode;


SPKDTree spInitTree(SPPoint* arr, int size, SDKTreeSpresd splitMethod, int spPCADimension);

SPKDTreeNode spTreeBuilder(SPKDArray kdArr, int size, int dim, SDKTreeSpresd splitMethod, int spPCADimension);

void spKNNSearch(SPKDTree tree, SPPoint queryPoint, int spKNN, int* imgArray);

void spKNNSearchRecurr(SPKDTreeNode currentNode, SPPoint queryPoint, SPBPQueue bpq);

void spKDTreeDestroy(SPKDTree tree);

void spKDTreeDestroyNode(SPKDTreeNode node);

bool spIsLeaf(SPKDTreeNode node);
