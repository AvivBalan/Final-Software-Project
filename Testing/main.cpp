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
		spLoggerPrintError("Memory Allocation Failure", "main.cpp", "spMainExtractFeaturesDir", 159);
		return NULL; //ERROR
	}
	features2DArray = (SPPoint**) malloc(sizeof(SPPoint*) * numOfImages);
	if(features2DArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.cpp", "spMainExtractFeaturesDir", 171);
		free(numOfFeaturesArray);
		return NULL; //ERROR
	}
	*sumOfFeatures = 0;
	for (i = 0; i < numOfImages; ++i) {
		spConfigGetImagePath(imagePath, config, i);
		features2DArray[i] = imageProc.getImageFeatures(imagePath, i, &temp);
		numOfFeaturesArray[i] = temp;
		if(features2DArray[i] == NULL){
			free(features2DArray);
			free(numOfFeaturesArray);
			return NULL; //ERROR
		}
		*sumOfFeatures = *sumOfFeatures + numOfFeaturesArray[i];
	}
	featuresArray = (SPPoint*) malloc(sizeof(SPPoint) * *sumOfFeatures);
	if(featuresArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.cpp", "spMainExtractFeaturesDir", 192);
		free(numOfFeaturesArray);
		return NULL; //ERROR
	}
	if(sumOfFeatures == 0){
		spLoggerPrintError("No features were extracted from the images", "main.cpp", "spMainExtractFeaturesDir", 192);
		free(features2DArray);
		free(numOfFeaturesArray);
		free(featuresArray);
		return NULL; //ERROR
	}
	featureCounter = 0;
	for (i = 0; i < numOfImages; ++i) {
		for (j = 0; j < numOfFeaturesArray[i]; ++j) {
			featuresArray[featureCounter] = features2DArray[i][j];
			featureCounter++;
		}
		free(features2DArray[i]);
	}
	if(spFeatureCreateFeatureFile(featuresArray, "spFeaturesFile.feat", numOfImages, *sumOfFeatures, spConfigGetPCADim(config, msg)) == 1){
		free(numOfFeaturesArray);
		free(features2DArray);
		return featuresArray;
		return NULL; //ERROR
	}
	free(numOfFeaturesArray);
	free(features2DArray);
	return featuresArray;
}

int spMainShowResults(ImageProc imageProc, int* imgCounterArray,
		SPConfig config, SP_CONFIG_MSG* msg, char* queryImgPath){
	int i, minSimilarFeatures, resultsNum, *indexArray;
	char imagePath[1025];
	SPBPQueue bpqSimilarImages;

	resultsNum = spConfigGetNumOfSimilarImages(config, msg);

	indexArray = (int*) malloc(resultsNum * sizeof(int));
	if(indexArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.cpp", "spMainShowResults", 11);
		return 1; //Allocation error
	}
	//Creating BPQ
	bpqSimilarImages = spBPQueueCreate(resultsNum);
	if(bpqSimilarImages == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.cpp", "spMainShowResults", 80);
		return 1; //ERROR
	}

	minSimilarFeatures = -1;
	for (i = 0; i < spConfigGetNumOfImages(config, msg); ++i) {
		if(!spBPQueueIsFull(bpqSimilarImages) || imgCounterArray[i] > minSimilarFeatures){
			if(spBPQueueIsFull(bpqSimilarImages))
				spBPQueueDequeue(bpqSimilarImages);
			SPListElement listEle = spListElementCreate(i, imgCounterArray[i]);
			if(listEle == NULL){
				spLoggerPrintError("Memory Allocation Failure", "main.cpp", "spMainShowResults", 109);
				return 1; //ERROR
				}
			spBPQueueEnqueue(bpqSimilarImages,listEle);
			minSimilarFeatures = spBPQueueMinValue(bpqSimilarImages);
		}
	}
	for (i = 0; i < resultsNum; ++i) {
		indexArray[i] = spListElementGetIndex(spBPQueuePeekLast(bpqSimilarImages));
		spBPQueueMaxDequeue(bpqSimilarImages);
	}
	//Showing Results
	spLoggerPrintInfo("Showing results...");
	if(spConfigMinimalGui(config, msg)) { //Minimal GUI
		for (i = 0; i < resultsNum; ++i){
			spConfigGetImagePath(imagePath, config, indexArray[i]);
			imageProc.showImage(imagePath);
		}
	}
	else{ //NON-Minimal GUI
		printf("Best candidates for - %s - are:\n",queryImgPath);
		for (i = 0; i < resultsNum; ++i){
			spConfigGetImagePath(imagePath, config, indexArray[i]);
			printf ("%s\n",imagePath);
		}
	}
	spBPQueueDestroy(bpqSimilarImages);
	free(indexArray);
	spLoggerPrintInfo("Done showing results");
	return 0;
}

int main(int argc, char **argv){
	char *configFilename, loggerFilename[1025], queryImgPath[1025];
	SPConfig config;
	SP_CONFIG_MSG *configMsg;
	int i, j, numOfDim, numOfFeaturesDir, numOfFeaturesQuery, *imgCounterArray = NULL;
	SPPoint *imagesFeaturesArray = NULL, *queryFeatures;
	SPKDTree kdTree = NULL;

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

	//Initializing Logger
	spConfigGetLoggerFilename(loggerFilename, config);
	if(spLoggerCreate(loggerFilename, (SP_LOGGER_LEVEL) (spConfigGetLoggerLevel(config, configMsg) - 1)) != SP_LOGGER_SUCCESS){
		spMainAuxFreeMem(0,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
		return 1;
	}

	//Initializing variables
	imgCounterArray = (int*) calloc(spConfigGetNumOfImages(config, configMsg), sizeof(int));
	if(imgCounterArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "main.cpp", "main", 21);
		spMainAuxFreeMem(0,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
		return 1;
	}
	ImageProc imageProc(config);

	//Extraction
	if(spConfigIsExtractionMode(config, configMsg)) //ExtractionMode
			imagesFeaturesArray = spMainExtractFeaturesDir(imageProc, config, configMsg, &numOfFeaturesDir);
		else //nonExtractionMode
			imagesFeaturesArray = spFeatureExtractFromFeatureFile("spFeaturesFile.feat", spConfigGetNumOfImages(config, configMsg),&numOfFeaturesDir, &numOfDim);
	if(imagesFeaturesArray == NULL){
		spMainAuxFreeMem(2,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
		return 1; //ERROR
	}

	//Creating KD tree
	kdTree = spKDTreeCreate(imagesFeaturesArray, numOfFeaturesDir, spConfigGetSplitMethods(config, configMsg), spConfigGetPCADim(config, configMsg));
	if(kdTree == NULL){
		spMainAuxFreeMem(3,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
		return 1; //ERROR
	}
	for (i = 0; i < numOfFeaturesDir; ++i)
		spPointDestroy(imagesFeaturesArray[i]);
	free(imagesFeaturesArray);

	//Receiving query
	printf("Please enter an image path:\n");
	if(scanf("%s", queryImgPath) < 0){
		spLoggerPrintError("Failed read from command line", "main.cpp", "main", 80);
		spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
		return 1;
	}
	while(strcmp(queryImgPath, "<>") != 0){
		spLoggerPrintInfo("Processing query...");
		queryFeatures = imageProc.getImageFeatures(queryImgPath, 0, &numOfFeaturesQuery);
		if(queryFeatures == NULL){
			spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
			return 1; //ERROR
		}
		for (i = 0; i < numOfFeaturesQuery; ++i){
			if(spKDTreeKNNSearch(kdTree, queryFeatures[i], spConfigGetspKNN(config, configMsg), imgCounterArray) == 1){
				spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
				for (j = 0; j < numOfFeaturesQuery; ++j)
					spPointDestroy(queryFeatures[j]);
				free(queryFeatures);
				return 1; //ERROR
				}
			spPointDestroy(queryFeatures[i]);
		}
		free(queryFeatures);
		spLoggerPrintInfo("Query completed");
		//Showing Results
		if(spMainShowResults(imageProc, imgCounterArray, config, configMsg, queryImgPath) == 1){
			spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
			return 1;
		}

    	printf("Please enter an image path:\n");
        if(scanf("%s", queryImgPath) < 0){
        	spLoggerPrintError("Failed read from command line", "main.cpp", "main", 142);
        	spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
        	return 1;
        }
        for (i = 0; i < spConfigGetNumOfImages(config, configMsg); ++i)
        	imgCounterArray[i] = 0;
    }
	printf("Exiting…\n");
	spMainAuxFreeMem(4,config,configMsg,imagesFeaturesArray,numOfFeaturesDir,kdTree,imgCounterArray);
	spLoggerDestroy();
	return 0;
}
