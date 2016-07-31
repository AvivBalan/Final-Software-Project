/*
 * SPConfig.cpp
 *
 *  Created on: 31 ביול 2016
 *      Author: נגה
 */
#include "SPConfig.h"

typedef enum SDKTreeSpresd  {RANDOM,MAX_SPREAD,INCREMENTAL};
typedef struct sp_config_t* SPConfig;
typedef struct sp_config_t{
	char *spImagesDirectory;
	char *spImagesPrefix;
	char* spImagesSuffix;
	char* spPCAFilename;
	bool spExtractionMode;
	bool spMinimalGUI;
	int spNumOfImages;
	int spNumOfFeatures;
	int spPCADimension;
	int spNumOfSimilarImages;
	SDKTreeSpresd spKDTreeSplitMethod;
	int spKNN;
	int spLoggerLevel;
	char * spLoggerFilename;


};

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg);

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	if (isEmpty(config)){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return 0;
	}
	*msg = SP_CONFIG_SUCCESS;
	bool temp = config-> spExtractionMode;
	return temp;
}


bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	if (isEmpty (config)){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return 0;
	}
	*msg = SP_CONFIG_SUCCESS;
	bool temp = config-> spMinimalGUI;
	return temp;
}

/*
 * Returns the number of images set in the configuration file, i.e the value
 * of spNumOfImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
	if (isEmpty (config)){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return 0;
	}
	*msg = SP_CONFIG_SUCCESS;
	int temp = config-> spNumOfImages;
	return temp;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	if (isEmpty (config)){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return 0;
	}
	*msg = SP_CONFIG_SUCCESS;
	int temp = config-> spNumOfFeatures;
	return temp;
}
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	if (isEmpty (config)){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return 0;
	}
	*msg = SP_CONFIG_SUCCESS;
	int temp = config-> spPCADimension;
	return temp;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config, int index);


SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config);


void spConfigDestroy(SPConfig config)

bool isEmpty (const SPConfig config){
	if (config = NULL){
		return 1;
	}
	return 0;

}




