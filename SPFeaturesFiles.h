#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPPoint.h"
#include "SPLogger.h"



/*
 * Returns 1 if error occurred. 0 in success.
 * create feature file based on the variable 'pointArray'.
 * the file will be written in the file 'filename'.
 *
 * @param pointArray - pointer to SPPoint
 * @param filename -  the name of the features file
 * @param numOfImages - number of the images
 * @param numOfFeatures - number of the features
 * @param numOfDim - number of  dimensions in the SPPoint.
 * @return 1 in case of error. 0 in success
 *
 */
int spFeatureCreateFeatureFile(SPPoint* pointArray, const char* filename, int numOfImages,int numOfFeatures, int numOfDim);



/*
 * Returns SPPoint array of features based on the features file
 * in 'filename' variable.
 *
 * @param filename -  the name of the features file
 * @param numOfImages - number of the images
 * @param numOfFeatures - an integer pointer, the function assign to it the number of features from the file
 * @param numOfDim - an integer pointer, the function assign to it the number of dimensions of the features from the file
 * @return SPPoint array in success. NULL otherwise
 *
 */
SPPoint* spFeatureExtractFromFeatureFile(const char* filename, int numOfImages,int *numOfFeatures, int *numOfDim);
