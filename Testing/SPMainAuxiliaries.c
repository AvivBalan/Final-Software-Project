#include "SPMainAuxiliaries.h"
#include <stdlib.h>



int spMainAuxInitVariables(SPConfig config, SP_CONFIG_MSG *configMsg, int *numOfSimilarImages, int** similarImageIndices,
		int *numOfImages, int** imgCounterArray){

	*numOfSimilarImages = spConfigGetNumOfSimilarImages(config, configMsg);

	*similarImageIndices = (int*) malloc(*numOfSimilarImages * sizeof(int));
	if(*similarImageIndices == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPMainAuxiliaries.c", "spMainAuxInitVariables", 11);
		free(configMsg);
		spConfigDestroy(config);
		return 1; //Allocation error
	}

	*numOfImages = spConfigGetNumOfImages(config, configMsg);

	*imgCounterArray = (int*) calloc(numOfImages, sizeof(int));
	if(imgCounterArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPMainAuxiliaries.cpp", "spMainAuxInitVariables", 21);
		return 1;
	}

	return 0;
}

void spMainAuxFreeMem(int phase, SPConfig config, SP_CONFIG_MSG *configMsg, SPPoint* imagesFeaturesArray, int numOfFeaturesDir, SPKDTree kdTree,
		SPBPQueue bpqSimilarImages, int* similarImageIndices, int* imgCounterArray){
	int i;

	if(phase >= 5)
		spBPQueueDestroy(bpqSimilarImages);
	if(phase >= 4)
		spKDTreeDestroy(kdTree);
	if(phase == 3){
		for (i = 0; i < numOfFeaturesDir; ++i)
			spPointDestroy(imagesFeaturesArray[i]);
		free(imagesFeaturesArray);
	}
	if(phase >= 2)
		spLoggerDestroy();
	if(phase >= 1){
		spConfigDestroy(config);
		free(configMsg);
		free(similarImageIndices);
		free(imgCounterArray);
		spLoggerDestroy();
	}
}
