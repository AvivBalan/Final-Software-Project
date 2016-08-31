#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPConfig.h"
#include <stdlib.h>

typedef struct sp_kd_tree_t* SPKDTree;

typedef struct sp_kd_tree_node_t* SPKDTreeNode;


SPKDTree InitTree(SPPoint* arr, int size, int splitMethod);

SPKDTreeNode TreeBuilder(SPKDArray kdArr, int size, int dim, int splitMethod);

void spKDTreeDestroy(SPKDTreeNode node);

void spKDTreeDestroyNode(SPKDTreeNode node);
