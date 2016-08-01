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


/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.png" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config, int index){
	/**
	 * varibales
	 */
    int src_fd, dst_fd, n, err,NumOfImages;
    unsigned char buffer[4096];
    char * src_path, dst_path, ImagesDirectory,ImagesPrefix,ImagesSuffix;
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
      * concat ImagesDirectory + ImagesPrefix + index.str + ImagesSuffix
      */

    src_fd = open(src_path, O_RDONLY);
    dst_fd = open(dst_path, O_CREAT | O_WRONLY);

    while (1) {
        err = read(src_fd, buffer, 4096);
        if (err == -1) {
            printf("Error reading file.\n");
            exit(1);
        }
        n = err;

        if (n == 0) break;

        err = write(dst_fd, buffer, n);
        if (err == -1) {
            printf("Error writing to file.\n");
            exit(1);
        }
    }

    close(src_fd);
    close(dst_fd);
}
}


SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config);


void spConfigDestroy(SPConfig config)

bool isEmpty (const SPConfig config){
	if (config = NULL){
		return 1;
	}
	return 0;

}




