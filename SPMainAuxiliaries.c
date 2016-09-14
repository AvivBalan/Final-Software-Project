#include "SPMainAuxiliaries.h"
#include <stdlib.h>

void spMainAuxFreeMem(int phase, SPConfig config, SP_CONFIG_MSG *configMsg, SPPoint* imagesFeaturesArray, int numOfFeaturesDir, SPKDTree kdTree, int* imgCounterArray){
	int i;

	spLoggerPrintInfo("Exiting...");
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
		free(imgCounterArray);
	}
}
