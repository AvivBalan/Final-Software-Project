#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPPoint.h"
#include "SPLogger.h"

int spFeatureCreateFeatureFile(SPPoint* pointArray, const char* filename, int numOfFeatures, int numOfDim);

SPPoint* spFeatureExtractFromFeatureFile(const char* filename, int *numOfFeatures, int *numOfDim);
