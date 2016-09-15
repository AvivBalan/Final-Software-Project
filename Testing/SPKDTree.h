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


/*
 * Creates a new SPKDTree struct.
 * is initialized based on the variables given by  SPPoint pointer 'array' and 'SDKTreeSpresd'.
 *
 * @param arr -  array of SPPoint. the SPPoints represent features.
 * @param size - the number of the elements in 'array'.
 * @param splitMethod - enum SDKTreeSpresd. The method of how the tree is split.
 * @param spPCADimension - integer. The number of dimensions of the features.
 * @return NULL in case an error occurs. Otherwise, struct KD Tree.
 *
 */
SPKDTree spKDTreeCreate(SPPoint* arr, int size, SDKTreeSpresd splitMethod, int spPCADimension);

/*
 * Creates a new SPKDTreeNode struct.
 * is a recursive function which build a kd tree by building a subtree each iteration.
 *
 * @param kdArr -  a SPKDArray struct. the struct contains the features and a matrix who sort them by demensions.
 * @param size - the number of the features in 'kdArr'.
 * @param dim - the current dimension
 * @param splitMethod - enum SDKTreeSpresd. The method of how the tree is split.
 * @param spPCADimension - integer. The number of dimensions of the features.
 * @return NULL in case an error occurs. Otherwise, a sub-tree of the KD Tree.
 *
 */
SPKDTreeNode spKDTreeBuilder(SPKDArray kdArr, int size, int dim, SDKTreeSpresd splitMethod, int spPCADimension);

/*
 * Searches similar features to the feature 'queryPoint'.
 *
 * @param tree -  a SPKDTree struct that contain all the features.
 * @param queryPoint - the query feature.
 * @param spKNN - the number of similar features required to found by the search.
 * @param imgArray - integer array. each cell represent an index of an image.
 *					The value in the cell represent the number of similar features from the image.
 * @return 1 in case an error occurs. Otherwise, 0 in success.
 *
 */
int spKDTreeKNNSearch(SPKDTree tree, SPPoint queryPoint, int spKNN, int* imgArray);

/*
 * Recursive search of similar features to the feature 'queryPoint'.
 *
 * @param currentNode -  a sub tree of kd tree that contain all the features.
 * @param queryPoint - the query feature.
 * @param spKNN - the number of similar features required to found by the search.
 * @param bpq - a SPBPQueue struct which contains the contender to be the similar features.
 * @return 1 in case an error occurs. Otherwise, 0 in success.
 *
 */
int spKDTreeKNNSearchRecurr(SPKDTreeNode currentNode, SPPoint queryPoint, SPBPQueue bpq);

/*
 * Frees all memory resources associate with tree.
 * @param tree - the SPKDTree structure
 * If tree == NULL nothig is done.
 */
void spKDTreeDestroy(SPKDTree tree);

/*
 * Frees all memory resources associate with node.
 * @param node - the SPKDTreeNode structure
 * If node == NULL nothig is done.
 */
void spKDTreeDestroyNode(SPKDTreeNode node);

/*
 * Checks whether node is a leaf (doesn't have sons).
 * @param node - a SPKDTreeNode structure
 * @return true if the node is a leaf, false else.
 */
bool spKDTreeIsLeaf(SPKDTreeNode node);
