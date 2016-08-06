
#include "SPConfig.h"

typedef enum SDKTreeSpresd  {RANDOM,MAX_SPREAD,INCREMENTAL};
typedef struct sp_config_t* SPConfig;

typedef struct sp_config_t{
	char *spImagesDirectory;
	char *spImagesPrefix;
	char* spImagesSuffix;
	char* spPCAFilename="pca.yml";
	bool spExtractionMode=1;
	bool spMinimalGUI=0;
	int spNumOfImages;
	int spNumOfFeatures=100;
	int spPCADimension=20;
	int spNumOfSimilarImages=1;
	SDKTreeSpresd spKDTreeSplitMethod=MAX_SPREAD;
	int spKNN=1;
	int spLoggerLevel=3;
	char * spLoggerFilename="stdout";


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



SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config, int index){
	if (index> config->spNumOfImages-1){
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
    if (isEmpty(config)){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (imagePath == NULL){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
	/**
	 * varibales
	 */

    char * src_path, ImagesDirectory,ImagesPrefix,ImagesSuffix;
    /**
     * concat ImagesDirectory + ImagesPrefix + index.str + ImagesSuffix
     */
    ImagesDirectory = config ->spImagesDirectory;
    ImagesPrefix = config ->spImagesPrefix;
    ImagesSuffix=config ->spImagesPrefix;
    char indexStr [5];
    itoa (index, indexStr,10);
    src_path = malloc(strlen(ImagesDirectory)+strlen(ImagesPrefix)+strlen(ImagesSuffix)+6);
    strcpy(src_path, ImagesDirectory);
    strcat(src_path, ImagesPrefix);
    strcat(src_path, indexStr);
    strcat(src_path, ImagesSuffix);
    /**
      * store "./images/img10.png" to the address given by imagePath
      */
    *imagePath = *src_path;
    free (src_path);
    return SP_CONFIG_SUCCESS ;
}


SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){

    if (isEmpty(config)){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (pcaPath == NULL){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
	/**
	 * varibales
	 */

    char * src_path, ImagesDirectory,PcaFilename;
    /**
     * concat ImagesDirectory + ImagesPrefix + index.str + ImagesSuffix
     */
    ImagesDirectory = config ->spImagesDirectory;
    PcaFilename = config ->spPCAFilename;

    src_path = malloc(strlen(ImagesDirectory)+strlen(PcaFilename)+1);
    strcpy(src_path, ImagesDirectory);
    strcat(src_path, PcaFilename);
    /**
      * store "./images/img10.png" to the address given by imagePath
      */
    *pcaPath = *src_path;
    free (src_path);
    return SP_CONFIG_SUCCESS ;
}


void spConfigDestroy(SPConfig config){
	free (config->spImagesDirectory);
	free (config->spImagesPrefix);
	free (config->spImagesSuffix);
	free (config->spPCAFilename);
	free (config->spLoggerFilename);
	free (config);

}

bool isEmpty (const SPConfig config){
	if (config == NULL){
		return 1;
	}
	return 0;

}




