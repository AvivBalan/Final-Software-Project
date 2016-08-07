/*
 * SPConfig.cpp
 *
 *  Created on: 31 áéåì 2016
 *      Author: ðâä
 */
#include "SPConfig.h"

typedef enum {RANDOM,MAX_SPREAD,INCREMENTAL} SDKTreeSpresd;
typedef struct sp_config_t* SPConfig;

typedef struct sp_config_t{
	char *spImagesDirectory = NULL;
	char *spImagesPrefix = NULL;
	char *spImagesSuffix = NULL;
	char *spPCAFilename = "pca.yml";
	bool spExtractionMode = true;
	bool spMinimalGUI = false;
	int spNumOfImages = -1;
	int spNumOfFeatures =100;
	int spPCADimension =20;
	int spNumOfSimilarImages = 1;
	SDKTreeSpresd spKDTreeSplitMethod = MAX_SPREAD;
	int spKNN = 1;
	int spLoggerLevel = 3;
	char *spLoggerFilename = "stdout";


};

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){

	char line[1024];
	char *variable;
	char *value;
	bool isComment, isValidLine, isEqualSign;
	FILE *configFile = NULL;
	int lineCounter = 0;
	int lineLength, varStart, varEnd, valueStart, valueEnd, charIndex;
	SPConfig config = NULL;

	assert(msg != NULL);

	if(filename == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	config = (SPConfig) malloc(sizeof(*SPConfig)); //allocating memory for the new configuration
	if (config == NULL) { //Allocation Fails
		msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}

	configFile = fopen("testconfig.txt","r");
	if (configFile == NULL){
		msg = SP_CONFIG_CANNOT_OPEN_FILE;
		spConfigDestroy(config);
		printf("The configuration file %s couldn’t be open\n", filename);
		reutrn NULL;
	}

	while(!(feof(configFile)))
	{
		lineCounter++;
		fgets(line, 1024, configFile);
		lineLength = strlen(line);
		varStart= -1;
		varEnd = -1;
		valueStart = -1;
		valueEnd = -1;
		isEqualSign = false;
		isComment = false;
		isValidLine = true;

		charIndex = 0;

		//Phase 1: Searching for the start of variable name
		while(varStart == -1 && charIndex < lineLength && !isComment && isValidLine)
		{

			if(line[charIndex] == '#')
			{
				isComment = true;
			}
			else if(line[charIndex] == '=')
			{
				isValidLine = false;
			}
			else if(!isspace(line[charIndex]))
			{
				varStart = charIndex;
			}

			charIndex++;
		}

		//Phase 2: Searching for the end of variable name
		while(varStart != -1 && varEnd == -1 && charIndex < lineLength && !isComment && isValidLine)
		{

			if(line[charIndex] == '=' || isspace(line[charIndex]))
			{
				varEnd = charIndex;

				if(line[charIndex] == '=')
				{
					isEqualSign = true;
				}

			}

			charIndex++;
		}

		//Phase 3: Searching for the equal sign
		while(varStart != -1 && varEnd != -1 && !isEqualSign && charIndex < lineLength && !isComment && isValidLine)
		{

			if(line[charIndex] == '=')
			{
				isEqualSign = true;
			}
			else if(!isspace(line[charIndex]))
			{
				isValidLine = false;
			}

			charIndex++;
		}

		//Phase 4: Searching for the start of the value
		while(varStart != -1 && varEnd != -1 && isEqualSign && valueStart == -1 && charIndex < lineLength && !isComment && isValidLine)
		{

			if(!isspace(line[charIndex]))
			{
				valueStart = charIndex;
			}

			charIndex++;
		}

		//Phase 5: Searching for the end of the value
		while(varStart != -1 && varEnd != -1 && isEqualSign && valueStart != -1 && valueEnd == -1 && !isComment && isValidLine && charIndex < lineLength)
		{
			if(isspace(line[charIndex]))
			{
				valueEnd = charIndex;
			}
			charIndex++;
		}

		if(valueEnd == -1 && charIndex == lineLength)
		{
			valueEnd = charIndex;
		}

		//Phase 6: Checking if there is excess data
		while(varStart != -1 && varEnd != -1 && isEqualSign && valueStart != -1 && valueEnd != -1 && !isComment && isValidLine && line[charIndex] != '\0')
		{

			if(!isspace(line[charIndex]))
			{
				isValidLine = false;
			}

			charIndex++;
		}

		if(varStart != -1 && varEnd != -1 && isEqualSign && valueStart != -1 && valueEnd != -1 && !isComment && isValidLine)
		{

			int varLen = varEnd - varStart;

			variable = (char*)malloc(sizeof(char)*(varLen + 1)); //allocating memory for the new point
			if (variable == NULL) { //Allocation Fails
				msg = SP_CONFIG_ALLOC_FAIL;
				return NULL;
			}

			memcpy(variable, &line[varStart], varLen);
			variable[varLen] = '\0';

			int valLen = valueEnd - valueStart;

			value = (char*) malloc(sizeof(char)*(valLen + 1)); //allocating memory for the new point
			if (value == NULL) { //Allocation Fails
				msg = SP_CONFIG_ALLOC_FAIL;
				return NULL;
			}

			memcpy(value, &line[valueStart], (valLen));
			value[valLen] = '\0';

			spConfigAssignValue(config, variable, value, msg, lineCounter, filename);

			if(msg == SP_CONFIG_INVALID_INTEGER || msg == SP_CONFIG_INVALID_STRING || msg == SP_CONFIG_ALLOC_FAIL){
				return NULL;
			}

			free(variable);
			free(value);
		}
		else if(!isComment){
			//ERROR - Invalid line
			msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid configuration line\n", filename, lineCounter);
			return NULL;
		}

	}

	//Checking for missing parameters

	if(config->spImagesDirectory == NULL){
		msg = SP_CONFIG_MISSING_DIR;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spImagesDirectory");
		return NULL;
	}

	if(config->spImagesPrefix == NULL){
		msg = SP_CONFIG_MISSING_PREFIX;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spImagesPrefix");
		return NULL;
	}

	if(config->spImagesSuffix == NULL){
		msg = SP_CONFIG_MISSING_SUFFIX;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spImagesSuffix");
		return NULL;
	}

	if(config->spNumOfImages == -1){
		msg = SP_CONFIG_MISSING_NUM_IMAGES;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spNumOfImages");
		return NULL;
	}

	fclose(configFile);

	msg = SP_CONFIG_SUCCESS;
	return config;
}


void spConfigAssignValue(SPConfig config, char *variable, char *value, SP_CONFIG_MSG* msg, int line, const char* filename){

	int valLen = strlen(value);

	if(variable == "spImagesDirectory"){

		config->spImagesDirectory = (char*) malloc(sizeof(char)*valLen); //allocating memory for the new point
		if (value == NULL) { //Allocation Fails
			msg = SP_CONFIG_ALLOC_FAIL;
			return;
		}

		memcpy(config->spImagesDirectory, value, valLen);
	}

	else if(variable == "spImagesPrefix"){

		config->spImagesPrefix = (char*) malloc(sizeof(char)*valLen); //allocating memory for the new point
		if (value == NULL) { //Allocation Fails
			msg = SP_CONFIG_ALLOC_FAIL;
			return;
		}

		memcpy(config->spImagesPrefix, value, valLen);
	}

	else if(variable == "spImagesSuffix"){

		if(strcmp(value,".jpg") != 0 && strcmp(value,".png") != 0 && strcmp(value,".bmp") != 0 && strcmp(value,".gif") != 0){
			msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}

		config->spImagesSuffix = (char*) malloc(sizeof(char)*valLen); //allocating memory for the new point
		if (value == NULL) { //Allocation Fails
			msg = SP_CONFIG_ALLOC_FAIL;
			return;
		}

		memcpy(config->spImagesSuffix, value, valLen);
	}

	else if(variable == "spPCAFilename"){

		config->spPCAFilename = (char*) malloc(sizeof(char)*valLen); //allocating memory for the new point
		if (value == NULL) { //Allocation Fails
			msg = SP_CONFIG_ALLOC_FAIL;
			return;
		}

		memcpy(config->spPCAFilename, value, valLen);
	}

	else if(variable == "spExtractionMode"){

		if(strcmp(value,"true") == 0){
			config->spExtractionMode = true;
		}
		else if(strcmp(value,"false") == 0){
			config->spExtractionMode = false;
		}
		else {
			msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spMinimalGUI"){

		if(strcmp(value,"true") == 0){
			config->spMinimalGUI = true;
		}
		else if(strcmp(value,"false") == 0){
			config->spMinimalGUI = false;
		}
		else {
			msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spNumOfImages"){
		config->spNumOfImages = atoi(value);

		if(config->spNumOfImages <= 0){
			msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spNumOfFeatures"){
		config->spNumOfFeatures = atoi(value);

		if(config->spNumOfFeatures <= 0){
			msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spPCADimension"){
		config->spPCADimension = atoi(value);

		if(config->spPCADimension < 10 || config->spPCADimension > 20){
			msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spNumOfSimilarImages"){
		config->spNumOfSimilarImages = atoi(value);

		if(config->spNumOfSimilarImages <= 0){
			msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spKDTreeSplitMethod"){
		if(strcmp(value,"MAX_SPREAD") == 0){
			config->spMinimalGUI = MAX_SPREAD;
		}
		else if(strcmp(value,"RANDOM") == 0){
			config->spKDTreeSplitMethod = RANDOM;
		}
		else if(strcmp(value,"INCREMENTAL") == 0){
			config->spKDTreeSplitMethod = INCREMENTAL;
		}
		else {
			msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spKNN"){
		config->spKNN = atoi(value);

		if(config->spKNN <= 0){
			msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spLoggerLevel"){
		config->spNumOfImages = atoi(value);

		if(config->spNumOfImages < 1 || config->spNumOfImages > 4){
			msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(variable == "spLoggerFilename"){
		config->spLoggerFilename = (char*) malloc(sizeof(char)*valLen); //allocating memory for the new point
		if (value == NULL) { //Allocation Fails
			msg = SP_CONFIG_ALLOC_FAIL;
			return;
		}

		memcpy(config->spLoggerFilename, value, valLen);
	}

	else{
		//ERROR - Invalid line
		msg = SP_CONFIG_INVALID_STRING;
		printf("File: %s\n Line: %d\n Message: Invalid configuration line\n", filename, line);
		return;
	}

	return;
}


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


void spConfigDestroy(SPConfig config){

	free(config->spImagesDirectory);
	free(config->spImagesPrefix);
	free(config->spImagesSuffix);
	free(config->spPCAFilename);
	free(config->spLoggerFilename);
	free(config);

}

bool isEmpty (const SPConfig config){
	if (config == NULL){
		return true;
	}
	return false;

}




