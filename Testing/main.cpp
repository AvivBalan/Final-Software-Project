#include <cstdlib> //include c library
#include <cstdio>
#include <cstring>
extern "C"{
//include your own C source files
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPKDTree.h"
#include "SPMainAuxiliaries.h"
#include "SPFeaturesFiles.h"
}
#include "SPImageProc.h"
using namespace sp;

SPPoint* spMainExtractFeaturesDir(ImageProc imageProc, SPConfig config, SP_CONFIG_MSG* msg, int* sumOfFeatures){
	int i, j, temp, featureCounter, numOfImages, *numOfFeaturesArray;
	SPPoint *featuresArray, **features2DArray;
	char imagePath[1025];

	numOfImages = spConfigGetNumOfImages(config, msg);
	numOfFeaturesArray = (int*) malloc(sizeof(int) * numOfImages);
	if(numOfFeaturesArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.c", "spMainExtractFeaturesDir", 159);
		return NULL; //ERROR
	}
	features2DArray = (SPPoint**) malloc(sizeof(SPPoint*) * numOfImages);
	if(features2DArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.c", "spMainExtractFeaturesDir", 171);
		free(numOfFeaturesArray);
		return NULL; //ERROR
	}
	for (i = 0; i < numOfImages; ++i) {
		spConfigGetImagePath(imagePath, config, i);
		features2DArray[i] = imageProc.getImageFeatures(imagePath, i, &temp);
		numOfFeaturesArray[i] = temp;
		if(features2DArray[i] == NULL){
			spLoggerPrintError("Error in sp:ImageProc::getImageFeatures", "main.c", "spMainExtractFeaturesDir", 181);
			free(features2DArray);
			free(numOfFeaturesArray);
			return NULL; //ERROR
		}
		sumOfFeatures = sumOfFeatures + numOfFeaturesArray[i];
	}
	featuresArray = (SPPoint*) malloc(sizeof(SPPoint) * *sumOfFeatures);
	if(featuresArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.c", "spMainExtractFeaturesDir", 192);
		free(numOfFeaturesArray);
		return NULL; //ERROR
	}
	featureCounter = 0;
	for (i = 0; i < numOfImages; ++i) {
		for (j = 0; j < numOfFeaturesArray[i]; ++j) {
			featuresArray[featureCounter] = features2DArray[i][j];
			featureCounter++;
		}
		SPPoint* k =  features2DArray[i];
		free(features2DArray[i]);
	}
	if(spFeatureCreateFeatureFile(featuresArray, "spFeaturesFile.feat", *sumOfFeatures, spConfigGetPCADim(config, msg)) == 1){
		free(numOfFeaturesArray);
		free(features2DArray);
		return featuresArray;
		return NULL; //ERROR
	}
	free(numOfFeaturesArray);
	free(features2DArray);
	return featuresArray;
}

int main(int argc, char **argv){
	char *configFilename, loggerFilename[1025], queryImgPath[1025], imagePath[1025];
	SPConfig config;
	SP_CONFIG_MSG *configMsg;
	int i, j, numOfImages, numOfDim, numOfFeaturesDir, numOfFeaturesQuery, *imgCounterArray = NULL, numOfSimilarImages, minSimilarFeatures, *similarImageIndices = NULL;
	SPPoint *imagesFeaturesArray = NULL, *queryFeatures;
	SPKDTree kdTree = NULL;
	SPBPQueue bpqSimilarImages = NULL;
	SPListElement listEle;

	//Command line argument
	configFilename = NULL;
	for (i = 1; i < argc; ++i)
		if((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-C") == 0) && i + 1 < argc)
			configFilename = argv[i+1];
	//Creating Config
	configMsg = (SP_CONFIG_MSG*) malloc (sizeof(SP_CONFIG_MSG));
	if(configMsg == NULL)
	{
		printf("Memory Allocation Failure\nFile: main.cpp\nLine: 30");
		return 1; //Allocation error
	}
	config = spConfigCreate(configFilename, configMsg);
	if(*configMsg != SP_CONFIG_SUCCESS){
		free(configMsg);
		return 1; //ERROR
	}
	//Initializng Logger
	spConfigGetLoggerFilename(loggerFilename, config);
	if(spLoggerCreate(loggerFilename, (SP_LOGGER_LEVEL) spConfigGetLoggerLevel(config, configMsg)) != SP_LOGGER_SUCCESS){
		spMainAuxFreeMem(1,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
		return 1;
	}
	//Initializing variables
	if(spMainAuxInitVariables(config, configMsg, &numOfSimilarImages, similarImageIndices, &numOfImages, imgCounterArray))
		return 1; //Allocation Error
	ImageProc imageProc(config);
	if(spConfigIsExtractionMode(config, configMsg)){ //ExtractionMode
		imagesFeaturesArray = spMainExtractFeaturesDir(imageProc, config, configMsg, &numOfFeaturesDir);
		if(imagesFeaturesArray == NULL){
			spMainAuxFreeMem(2,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
			return 1; //ERROR
		}
	}
	else{ //nonExtractionMode
		imagesFeaturesArray = spFeatureExtractFromFeatureFile("spFeaturesFile", &numOfFeaturesDir, &numOfDim);
		if(imagesFeaturesArray == NULL){
			spMainAuxFreeMem(2,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
			return 1; //ERROR
		}
	}
	//Creating KD tree
	kdTree = spKDTreeCreate(imagesFeaturesArray, numOfFeaturesDir, spConfigGetSplitMethods(config, configMsg), spConfigGetPCADim(config, configMsg));
	if(kdTree == NULL){
		spMainAuxFreeMem(3,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
		return 1; //ERROR
	}
	for (i = 0; i < numOfFeaturesDir; ++i)
		spPointDestroy(imagesFeaturesArray[i]);
	free(imagesFeaturesArray);
	//Creating BPQ
	bpqSimilarImages = spBPQueueCreate(numOfSimilarImages);
	if(bpqSimilarImages == NULL){
		spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
		return 1; //ERROR
	}
	//Receiving query
	printf("Please enter an image path:\n");
	if(scanf("%s", queryImgPath) < 0){
		spLoggerPrintError("Failed read from command line", "main.c", "main", 80);
		spMainAuxFreeMem(5,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
		return 1;
	}
	while(strcmp(queryImgPath, "<>") != 0){
		spBPQueueClear(bpqSimilarImages);
		queryFeatures = imageProc.getImageFeatures(queryImgPath, 0, &numOfFeaturesQuery);
		if(queryFeatures == NULL){
			spLoggerPrintError("Error in sp:ImageProc::getImageFeatures", "main.c", "main", 87);
			spMainAuxFreeMem(5,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
			return 1; //ERROR
		}
		for (i = 0; i < numOfFeaturesQuery; ++i){
			if(spKDTreeKNNSearch(kdTree, queryFeatures[i], spConfigGetspKNN(config, configMsg), imgCounterArray) == 1){
				spMainAuxFreeMem(5,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
				for (j = 0; j < numOfFeaturesQuery; ++j)
					spPointDestroy(queryFeatures[j]);
				free(queryFeatures);
				return 1; //ERROR
				}
			spPointDestroy(queryFeatures[i]);
		}
		free(queryFeatures);
		minSimilarFeatures = -1;
		for (i = 0; i < numOfImages; ++i) {
			if(!spBPQueueIsFull(bpqSimilarImages) || imgCounterArray[i] > minSimilarFeatures){
				if(spBPQueueIsFull(bpqSimilarImages))
					spBPQueueDequeue(bpqSimilarImages);
				listEle = spListElementCreate(i, imgCounterArray[i]);
				if(listEle == NULL){
					spLoggerPrintError("Memory Allocation Failure", "main.c", "main", 109);
					spMainAuxFreeMem(5,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
					return 1; //ERROR
					}
				spBPQueueEnqueue(bpqSimilarImages,listEle);
				minSimilarFeatures = spBPQueueMinValue(bpqSimilarImages);
			}
		}
		for (i = 0; i < numOfSimilarImages; ++i) {
			similarImageIndices[i] = spListElementGetIndex(spBPQueuePeekLast(bpqSimilarImages));
			spBPQueueMaxDequeue(bpqSimilarImages);
		}
		spBPQueueDestroy(bpqSimilarImages);
		//Show Results
		if(spConfigMinimalGui(config, configMsg)) { //Minimal GUI
			for (i = 0; i < numOfSimilarImages; ++i){
				spConfigGetImagePath(imagePath, config, similarImageIndices[i]);
				imageProc.showImage(imagePath);
			}
		}
		else{ //NON-Minimal GUI
			printf("Best candidates for - %s - are:\n",queryImgPath);
    		for (i = 0; i < numOfSimilarImages; ++i){
    			spConfigGetImagePath(imagePath, config, similarImageIndices[i]);
    			printf ("%s\n",imagePath);
    		}
    	}
    	printf("Please enter an image path:\n");
        if(scanf("%s", queryImgPath) < 0){
        	spLoggerPrintError("Failed read from command line", "main.c", "main", 142);
        	spMainAuxFreeMem(5,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
        	return 1;
        }
    }
	printf("Exiting…\n");
	spMainAuxFreeMem(5,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,bpqSimilarImages,similarImageIndices,imgCounterArray);
	return 0;
}
