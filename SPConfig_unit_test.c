/*
 * SPConfig_unit_test.c
 *
 *  Created on: 7 баев 2016
 *      Author: рвд
 */

#include "unit_test_util.h"
#include "SPConfig.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>



//Checks if the function create Works

/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 *
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 *
 *
 */
static bool testspConfigCreate(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// normal inputs
	SPConfig config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config!=NULL);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	// filename = null
	config = spConfigCreate(NULL, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	spConfigDestroy(config);
	// configuration file given by filename cannot be open
	config = spConfigCreate("abc", msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_CANNOT_OPEN_FILE);
	spConfigDestroy(config);
	// allocation failure occurred
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_ALLOC_FAIL);
	spConfigDestroy(config);
	//line in the config file contains invalid integer
	filename = "file with invalid integer";
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy(config);
	//line in the config file contains invalid string
	filename = "file with invalid string";
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_STRING);
	spConfigDestroy(config);
	//spImagesDirectory is missing
	filename = "file with spImagesDirectory missing";
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_DIR);
	spConfigDestroy(config);
	//spImagesPrefix is missing
	filename = "file with spImagesPrefix is missing";
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_PREFIX);
	spConfigDestroy(config);
	//spImagesSuffix is missing
	filename = "file with spImagesSuffix is missing";
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_SUFFIX);
	spConfigDestroy(config);
	//spNumOfImages is missing
	filename = "file with spNumOfImages is missing";
	config = spConfigCreate(filename, msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_NUM_IMAGES);
	spConfigDestroy(config);
	free (msg);
	return true;
}
// checks if spConfigIsExtractionMode works
/*
 * Returns true if spExtractionMode = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
static bool testspConfigIsExtractionMode (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	spConfigDestroy(config);
	bool ExtractionMode;
	// config = NULL
	ExtractionMode = spConfigIsExtractionMode (NULL, msg);
	ASSERT_TRUE(ExtractionMode==false);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	ExtractionMode = spConfigIsExtractionMode (config, msg);
	ASSERT_TRUE(ExtractionMode==config->ExtractionMode);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	free (msg);
	return true;
}
// checks if spConfigMinimalGui works
/*
 * Returns true if spMinimalGUI = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
static bool testspConfigMinimalGui (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	spConfigDestroy(config);
	bool MinimalGui;
	// config = NULL
	MinimalGui = spConfigMinimalGui (NULL, msg);
	ASSERT_TRUE(MinimalGui==false);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	MinimalGui = spConfigMinimalGui (config, msg);
	ASSERT_TRUE(MinimalGui==config->MinimalGui);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	free (msg);
	return true;
}
// checks if spConfigGetNumOfImages works
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
static bool testspConfigGetNumOfImages (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	spConfigDestroy(config);
	int NumOfImages;
	// config = NULL
	NumOfImages = spConfigGetNumOfImages (NULL, msg);
	ASSERT_TRUE(NumOfImages==0);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	NumOfImages = spConfigGetNumOfImages (config, msg);
	ASSERT_TRUE(NumOfImages==config->NumOfImages);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	free (msg);
	return true;
}
// checks if spConfigGetNumOfFeatures works
/*
 * Returns the number of features to be extracted. i.e the value
 * of spNumOfFeatures.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */

static bool testspConfigGetNumOfFeatures (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	spConfigDestroy(config);
	int NumOfFeatures;
	// config = NULL
	NumOfFeatures = spConfigGetNumOfFeatures (NULL, msg);
	ASSERT_TRUE(NumOfFeatures==0);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	NumOfFeatures = spConfigGetNumOfFeatures (config, msg);
	ASSERT_TRUE(NumOfFeatures==config->NumOfFeatures);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	free (msg);
	return true;
}

// checks if spConfigGetPCADim works
/**
 * Returns the dimension of the PCA. i.e the value of spPCADimension.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
static bool testspConfigGetPCADim (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	spConfigDestroy(config);
	int PCADim;
	// config = NULL
	PCADim = spConfigGetPCADim (NULL, msg);
	ASSERT_TRUE(PCADim==0);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	PCADim = spConfigGetPCADim (config, msg);
	ASSERT_TRUE(PCADim==config->PCADim);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	free (msg);
	return true;
}
// checks if spConfigGetImagePath works
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
static bool testspConfigGetImagePath (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "filename";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	spConfigDestroy(config);
}

static bool testspConfigGetPCAPath (){

}

static bool testsspConfigDestroy (){

}

static bool testisEmpty (){

}


