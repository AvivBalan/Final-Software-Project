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
	SPConfig config = spConfigCreate("config_unit_testing_1.txt", msg);
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
	//line in the config file contains invalid integer
	filename = "file with invalid integer";
	config = spConfigCreate("config_unit_testing_2.txt", msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy(config);
	//line in the config file contains invalid string
	filename = "file with invalid string";
	config = spConfigCreate("config_unit_testing_3.txt", msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_STRING);
	spConfigDestroy(config);
	//spImagesDirectory is missing
	filename = "file with spImagesDirectory missing";
	config = spConfigCreate("config_unit_testing_4.txt", msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_DIR);
	spConfigDestroy(config);
	//spImagesPrefix is missing
	filename = "file with spImagesPrefix is missing";
	config = spConfigCreate("config_unit_testing_5.txt", msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_PREFIX);
	spConfigDestroy(config);
	//spImagesSuffix is missing
	filename = "file with spImagesSuffix is missing";
	config = spConfigCreate("config_unit_testing_6.txt", msg);
	ASSERT_TRUE(config==NULL);
	ASSERT_TRUE(msg==SP_CONFIG_MISSING_SUFFIX);
	spConfigDestroy(config);
	//spNumOfImages is missing
	filename = "file with spNumOfImages is missing";
	config = spConfigCreate("config_unit_testing_7.txt", msg);
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
	char * filename = "config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	bool ExtractionMode;
	// config = NULL
	ExtractionMode = spConfigIsExtractionMode (NULL, msg);
	ASSERT_TRUE(ExtractionMode==false);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	ExtractionMode = spConfigIsExtractionMode (config, msg);
	ASSERT_TRUE(ExtractionMode==true);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
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
	char * filename = "config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	bool MinimalGui;
	// config = NULL
	MinimalGui = spConfigMinimalGui (NULL, msg);
	ASSERT_TRUE(MinimalGui==false);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	MinimalGui = spConfigMinimalGui (config, msg);
	ASSERT_TRUE(MinimalGui==true);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
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
	char * filename = "config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	int NumOfImages;
	// config = NULL
	NumOfImages = spConfigGetNumOfImages (NULL, msg);
	ASSERT_TRUE(NumOfImages==0);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	NumOfImages = spConfigGetNumOfImages (config, msg);
	ASSERT_TRUE(NumOfImages==10);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
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
	char * filename = "config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	int NumOfFeatures;
	// config = NULL
	NumOfFeatures = spConfigGetNumOfFeatures (NULL, msg);
	ASSERT_TRUE(NumOfFeatures==0);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	NumOfFeatures = spConfigGetNumOfFeatures (config, msg);
	ASSERT_TRUE(NumOfFeatures==80);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
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
	char * filename = "config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	int PCADim;
	// config = NULL
	PCADim = spConfigGetPCADim (NULL, msg);
	ASSERT_TRUE(PCADim==0);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	PCADim = spConfigGetPCADim (config, msg);
	ASSERT_TRUE(PCADim==15);
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
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
	char * filename = "config_unit_testing_1.txt";
	char* imagePath = (char*)malloc (sizeof(char)*50);
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	// imagePath == NULL
	msg = spConfigGetImagePath(NULL, config,1);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// config  NULL
	msg = spConfigGetImagePath(imagePath, NULL,1);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	//index >= spNumOfImages
	msg = spConfigGetImagePath(imagePath, config,1000);
	ASSERT_TRUE(msg==SP_CONFIG_INDEX_OUT_OF_RANGE);
	//success
	msg = spConfigGetImagePath(imagePath, config,1);
	ASSERT_TRUE( *imagePath == "./images/img1.png");
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (imagePath);
	free (msg);
	return true;
}


// checks if spConfigGetImagePath works
/**
 * The function stores in pcaPath the full path of the pca file.
 * For example given the values of:
 *  spImagesDirectory = "./images/"
 *  spPcaFilename = "pca.yml"
 *
 * The functions stores "./images/pca.yml" to the address given by pcaPath.
 * Thus the address given by pcaPath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
static bool testspConfigGetPCAPath (){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "config_unit_testing_1.txt";
	char* pcaPath = (char*)malloc (sizeof(char)*50);
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	// pcaPath == NULL
	msg = spConfigGetPCAPath(NULL, config);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);
	// config  NULL
	msg = spConfigGetPCAPath(pcaPath, NULL);
	ASSERT_TRUE(msg==SP_CONFIG_INVALID_ARGUMENT);

	//success
	msg = spConfigGetPCAPath(pcaPath, config);
	ASSERT_TRUE( *pcaPath == "./images/pca.yml");
	ASSERT_TRUE(msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (pcaPath);
	free (msg);
	return true;
}

static bool testsspConfigDestroy (){
	spConfigDestroy(NULL);
	return true;
}

static bool testisEmpty (){
	bool empty = isEmpty(NULL);
	ASSERT_TRUE(empty == true);
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	empty = isEmpty(config);
	ASSERT_TRUE(empty == false);
	spConfigDestroy(config);
	free (msg);
	return true;

}


