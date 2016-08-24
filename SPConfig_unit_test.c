#include "unit_test_util.h"
#include "SPConfig.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>



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
int spConfig_unit_testtestspConfigCreate(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*) malloc (sizeof(SP_CONFIG_MSG));

	// normal inputs
	SPConfig config = spConfigCreate("configFiles\\config_unit_testing_1.txt", msg);
	assert(*msg==SP_CONFIG_SUCCESS);
	char* imagePath = (char*) malloc (sizeof(char)*50);
	spConfigGetImagePath(imagePath,config,1);
	assert(strcmp(imagePath,"./images/img1.png") == 0);
	spConfigGetPCAPath(imagePath, config);
	assert(strcmp(imagePath,"./images/pca.yml") == 0);
	free(imagePath);
	assert(spConfigIsExtractionMode(config,msg) == true);
	assert(spConfigMinimalGui(config,msg) == true);
	assert(spConfigGetNumOfImages(config,msg) == 10);
	assert(spConfigGetNumOfFeatures(config,msg) == 80);
	assert(spConfigGetPCADim(config,msg) == 15);
	spConfigDestroy(config);

	//NULL
	config = spConfigCreate(NULL, msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	spConfigDestroy(config);

	// configuration file given by filename cannot be open
	config = spConfigCreate("abc", msg);
	assert(config==NULL);
	assert(*msg == SP_CONFIG_CANNOT_OPEN_FILE);
	spConfigDestroy(config);

	//line in the config file contains invalid integer
	config = spConfigCreate("configFiles\\config_unit_testing_2.txt", msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_INVALID_INTEGER);
	spConfigDestroy(config);

	//line in the config file contains invalid string
	config = spConfigCreate("configFiles\\config_unit_testing_3.txt", msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_INVALID_STRING);
	spConfigDestroy(config);

	//spImagesDirectory is missing
	config = spConfigCreate("configFiles\\config_unit_testing_4.txt", msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_MISSING_DIR);
	spConfigDestroy(config);

	//spImagesPrefix is missing
	config = spConfigCreate("configFiles\\config_unit_testing_5.txt", msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_MISSING_PREFIX);
	spConfigDestroy(config);

	//spImagesSuffix is missing
	config = spConfigCreate("configFiles\\config_unit_testing_6.txt", msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_MISSING_SUFFIX);
	spConfigDestroy(config);

	//spNumOfImages is missing
	config = spConfigCreate("configFiles\\config_unit_testing_7.txt", msg);
	assert(config==NULL);
	assert(*msg==SP_CONFIG_MISSING_NUM_IMAGES);
	spConfigDestroy(config);

	free (msg);
	return 1;
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
int testspConfigIsExtractionMode(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	bool ExtractionMode;
	// config = NULL
	ExtractionMode = spConfigIsExtractionMode (NULL, msg);
	assert(ExtractionMode==false);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	ExtractionMode = spConfigIsExtractionMode (config, msg);
	assert(ExtractionMode == true);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free(msg);
	return 1;
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
int testspConfigMinimalGui(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	bool MinimalGui;
	// config = NULL
	MinimalGui = spConfigMinimalGui (NULL, msg);
	assert(MinimalGui==false);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	MinimalGui = spConfigMinimalGui (config, msg);
	assert(MinimalGui==true);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (msg);
	return 1;
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
int testspConfigGetNumOfImages(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	int NumOfImages;
	// config = NULL
	NumOfImages = spConfigGetNumOfImages (NULL, msg);
	assert(NumOfImages==0);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	NumOfImages = spConfigGetNumOfImages (config, msg);
	assert(NumOfImages==10);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (msg);
	return 1;
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

int testspConfigGetNumOfFeatures(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	int NumOfFeatures;
	// config = NULL
	NumOfFeatures = spConfigGetNumOfFeatures (NULL, msg);
	assert(NumOfFeatures==0);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	NumOfFeatures = spConfigGetNumOfFeatures (config, msg);
	assert(NumOfFeatures==80);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (msg);
	return 1;
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
int testspConfigGetPCADim(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	// create config
	SPConfig config = spConfigCreate("configFiles\\config_unit_testing_1.txt", msg);

	int PCADim;
	// config = NULL
	PCADim = spConfigGetPCADim (NULL, msg);
	assert(PCADim==0);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// normal input
	PCADim = spConfigGetPCADim (config, msg);
	assert(PCADim==15);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (msg);
	return 1;
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
int testspConfigGetImagePath(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	char* imagePath = (char*)malloc (sizeof(char)*50);
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	// imagePath == NULL
	*msg = spConfigGetImagePath(NULL, config,1);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// config  NULL
	*msg = spConfigGetImagePath(imagePath, NULL,1);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	//index >= spNumOfImages
	*msg = spConfigGetImagePath(imagePath, config,1000);
	assert(*msg==SP_CONFIG_INDEX_OUT_OF_RANGE);
	//success
	*msg = spConfigGetImagePath(imagePath,config,1);
	assert(strcmp(imagePath,"./images/img1.png") == 0);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (imagePath);
	free (msg);
	return 1;
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
int testspConfigGetPCAPath(){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	char* pcaPath = (char*)malloc (sizeof(char)*50);
	// create config
	SPConfig config = spConfigCreate(filename, msg);

	// pcaPath == NULL
	*msg = spConfigGetPCAPath(NULL, config);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);
	// config  NULL
	*msg = spConfigGetPCAPath(pcaPath, NULL);
	assert(*msg==SP_CONFIG_INVALID_ARGUMENT);

	//success
	*msg = spConfigGetPCAPath(pcaPath, config);
	assert(strcmp(pcaPath,"./images/pca.yml") == 0);
	assert(*msg==SP_CONFIG_SUCCESS);
	spConfigDestroy(config);
	free (pcaPath);
	free (msg);
	return 1;
}

int testisEmpty(){
	bool empty = isEmpty(NULL);
	assert(empty == true);
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*)malloc (sizeof(SP_CONFIG_MSG));
	char * filename = "configFiles\\config_unit_testing_1.txt";
	// create config
	SPConfig config = spConfigCreate(filename, msg);
	empty = isEmpty(config);
	assert(empty == false);
	spConfigDestroy(config);
	free (msg);
	return 1;

}

