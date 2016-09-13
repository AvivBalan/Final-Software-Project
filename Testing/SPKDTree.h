#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

typedef struct sp_kd_tree_t* SPKDTree;

typedef struct sp_kd_tree_node_t* SPKDTreeNode;


SPKDTree spKDTreeCreate(SPPoint* arr, int size, SDKTreeSpresd splitMethod, int spPCADimension);

SPKDTreeNode spKDTreeBuilder(SPKDArray kdArr, int size, int dim, SDKTreeSpresd splitMethod, int spPCADimension);

int spKDTreeKNNSearch(SPKDTree tree, SPPoint queryPoint, int spKNN, int* imgArray);

int spKDTreeKNNSearchRecurr(SPKDTreeNode currentNode, SPPoint queryPoint, SPBPQueue bpq);

void spKDTreeDestroy(SPKDTree tree);

void spKDTreeDestroyNode(SPKDTreeNode node);

bool spKDTreeIsLeaf(SPKDTreeNode node);
