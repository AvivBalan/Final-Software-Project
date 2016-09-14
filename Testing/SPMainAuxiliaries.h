#include "SPLogger.h"
#include "SPConfig.h"
#include "SPKDTree.h"

int spMainAuxInitVariables(SPConfig config, SP_CONFIG_MSG *configMsg, int *numOfSimilarImages, int** similarImageIndices,
		int *numOfImages, int** imgCounterArray);

void spMainAuxFreeMem(int phase, SPConfig config, SP_CONFIG_MSG *configMsg, SPPoint* imagesFeaturesArray, int numOfFeaturesDir, SPKDTree kdTree, int* similarImageIndices, int* imgCounterArray);
