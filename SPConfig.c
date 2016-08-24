#include "SPConfig.h"

typedef enum {RANDOM,MAX_SPREAD,INCREMENTAL} SDKTreeSpresd;

struct sp_config_t{
	char *spImagesDirectory;
	char *spImagesPrefix;
	char *spImagesSuffix;
	char *spPCAFilename;
	bool spExtractionMode;
	bool spMinimalGUI;
	int spNumOfImages;
	int spNumOfFeatures;
	int spPCADimension;
	int spNumOfSimilarImages;
	SDKTreeSpresd spKDTreeSplitMethod;
	int spKNN;
	int spLoggerLevel;
	char *spLoggerFilename;
};

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){

	char line[1024];
	char *variable;
	char *value;
	bool isComment, isValidLine, isEqualSign;
	FILE *configFile = NULL;
	int lineCounter = 0;
	int lineLength, varStart, varEnd, valueStart, valueEnd, charIndex;
	SPConfig config;

	assert(msg != NULL);

	if(filename == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	config = (SPConfig) malloc(sizeof(*config)); //allocating memory for the new configuration
	if(config == NULL) { //Allocation Fails
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}

	config->spImagesDirectory = (char*) malloc(sizeof(char)*1025); //allocating memory for the new point
	if (config->spImagesDirectory == NULL) { //Allocation Fails
		*msg = SP_CONFIG_ALLOC_FAIL;
		free(config);
		return NULL;
	}
	*config->spImagesDirectory = '\0';
	config->spImagesPrefix = (char*) malloc(sizeof(char)*1025); //allocating memory for the new point
	if (config->spImagesPrefix == NULL) { //Allocation Fails
		*msg = SP_CONFIG_ALLOC_FAIL;
		free(config->spImagesDirectory);
		free(config);
		return NULL;
	}
	*config->spImagesPrefix = '\0';
	config->spImagesSuffix = (char*) malloc(sizeof(char)*1025); //allocating memory for the new point
	if (config->spImagesSuffix == NULL) { //Allocation Fails
		*msg = SP_CONFIG_ALLOC_FAIL;
		free(config->spImagesPrefix);
		free(config->spImagesDirectory);
		free(config);
		return NULL;
	}
	*config->spImagesSuffix = '\0';
	config->spPCAFilename = (char*) malloc(sizeof(char)*1025); //allocating memory for the new point
	if (config->spPCAFilename == NULL) { //Allocation Fails
		*msg = SP_CONFIG_ALLOC_FAIL;
		free(config->spImagesSuffix);
		free(config->spImagesPrefix);
		free(config->spImagesDirectory);
		free(config);
		return NULL;
	}
	strcpy(config->spPCAFilename, "pca.yml");
	config->spExtractionMode = true;
	config->spMinimalGUI = false;
	config->spNumOfImages = -1;
	config->spNumOfFeatures = 100;
	config->spPCADimension = 20;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spLoggerLevel = 3;
	config->spLoggerFilename = (char*) malloc(sizeof(char)*1025); //allocating memory for the new point
	if (config->spLoggerFilename == NULL) { //Allocation Fails
		*msg = SP_CONFIG_ALLOC_FAIL;
		free(config->spPCAFilename);
		free(config->spImagesSuffix);
		free(config->spImagesPrefix);
		free(config->spImagesDirectory);
		free(config);
		return NULL;
	}
	strcpy(config->spLoggerFilename, "stdout");


	configFile = fopen(filename,"r");
	if (configFile == NULL){
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		spConfigDestroy(config);
		printf("The configuration file %s couldn’t be open\n", filename);
		return NULL;
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
				*msg = SP_CONFIG_ALLOC_FAIL;
				spConfigDestroy(config);
				return NULL;
			}

			memcpy(variable, &line[varStart], varLen);
			variable[varLen] = '\0';

			int valLen = valueEnd - valueStart;

			value = (char*) malloc(sizeof(char)*(valLen + 1)); //allocating memory for the new point
			if (value == NULL) { //Allocation Fails
				*msg = SP_CONFIG_ALLOC_FAIL;
				spConfigDestroy(config);
				return NULL;
			}

			memcpy(value, &line[valueStart], (valLen));
			value[valLen] = '\0';

			*msg = SP_CONFIG_SUCCESS;
			spConfigAssignValue(config, variable, value, msg, lineCounter, filename);

			if(*msg == SP_CONFIG_INVALID_INTEGER || *msg == SP_CONFIG_INVALID_STRING || *msg == SP_CONFIG_ALLOC_FAIL){
				spConfigDestroy(config);
				return NULL;
			}

			free(variable);
			free(value);
		}
		else if(!isComment && varStart != -1){
			//ERROR - Invalid line
			*msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid configuration line\n", filename, lineCounter);
			spConfigDestroy(config);
			return NULL;
		}

	}

	//Checking for missing parameters

	if(*config->spImagesDirectory == '\0'){
		*msg = SP_CONFIG_MISSING_DIR;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spImagesDirectory");
		spConfigDestroy(config);
		return NULL;
	}

	if(*config->spImagesPrefix == '\0'){
		*msg = SP_CONFIG_MISSING_PREFIX;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spImagesPrefix");
		spConfigDestroy(config);
		return NULL;
	}

	if(*config->spImagesSuffix == '\0'){
		*msg = SP_CONFIG_MISSING_SUFFIX;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spImagesSuffix");
		spConfigDestroy(config);
		return NULL;
	}

	if(config->spNumOfImages == -1){
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		printf("File: %s\nLine: %d\nMessage: Parameter %s is not set\n", filename, lineCounter, "spNumOfImages");
		spConfigDestroy(config);
		return NULL;
	}

	fclose(configFile);

	*msg = SP_CONFIG_SUCCESS;
	return config;
}


void spConfigAssignValue(SPConfig config, char *variable, char *value, SP_CONFIG_MSG* msg, int line, const char* filename){
	int valLen = strlen(value) + 1;

	if(strcmp(variable,"spImagesDirectory") == 0){
		memcpy(config->spImagesDirectory, value, valLen);
	}

	else if(strcmp(variable,"spImagesPrefix") == 0){
		memcpy(config->spImagesPrefix, value, valLen);
	}

	else if(strcmp(variable,"spImagesSuffix") == 0){

		if(strcmp(value,".jpg") != 0 && strcmp(value,".png") != 0 && strcmp(value,".bmp") != 0 && strcmp(value,".gif") != 0){
			*msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
		memcpy(config->spImagesSuffix, value, valLen);
	}

	else if(strcmp(variable,"spPCAFilename") == 0){
		memcpy(config->spPCAFilename, value, valLen);
	}

	else if(strcmp(variable,"spExtractionMode") == 0){

		if(strcmp(value,"true") == 0){
			config->spExtractionMode = true;
		}
		else if(strcmp(value,"false") == 0){
			config->spExtractionMode = false;
		}
		else {
			*msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spMinimalGUI") == 0){

		if(strcmp(value,"true") == 0){
			config->spMinimalGUI = true;
		}
		else if(strcmp(value,"false") == 0){
			config->spMinimalGUI = false;
		}
		else {
			*msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spNumOfImages") == 0){
		config->spNumOfImages = atoi(value);

		if(config->spNumOfImages <= 0){
			*msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spNumOfFeatures") == 0){
		config->spNumOfFeatures = atoi(value);

		if(config->spNumOfFeatures <= 0){
			*msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spPCADimension") == 0){
		config->spPCADimension = atoi(value);

		if(config->spPCADimension < 10 || config->spPCADimension > 20){
			*msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spNumOfSimilarImages") == 0){
		config->spNumOfSimilarImages = atoi(value);

		if(config->spNumOfSimilarImages <= 0){
			*msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spKDTreeSplitMethod") == 0){
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
			*msg = SP_CONFIG_INVALID_STRING;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spKNN") == 0){
		config->spKNN = atoi(value);

		if(config->spKNN <= 0){
			*msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spLoggerLevel") == 0){
		config->spLoggerLevel = atoi(value);

		if(config->spLoggerLevel < 1 || config->spLoggerLevel > 4){
			*msg = SP_CONFIG_INVALID_INTEGER;
			printf("File: %s\n Line: %d\n Message: Invalid value - constraint not met", filename, line);
			return;
		}
	}

	else if(strcmp(variable,"spLoggerFilename") == 0){
		memcpy(config->spLoggerFilename, value, valLen);
	}

	else{
		//ERROR - Invalid line
		*msg = SP_CONFIG_INVALID_STRING;
		printf("File: %s\n Line: %d\n Message: Invalid configuration line\n", filename, line);
		return;
	}
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



SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config, int index){
    if (isEmpty(config)){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (imagePath == NULL){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
	if (index >= config->spNumOfImages){
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
     //concat ImagesDirectory + ImagesPrefix + index.str + ImagesSuffix
    char indexStr[5];
    itoa (index, indexStr, 10);
    strcpy(imagePath, config->spImagesDirectory);
    strcat(imagePath, config->spImagesPrefix);
    strcat(imagePath, indexStr);
    strcat(imagePath, config->spImagesSuffix);
    return SP_CONFIG_SUCCESS ;
}


SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
    if (isEmpty(config)){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (pcaPath == NULL){
    	return SP_CONFIG_INVALID_ARGUMENT;
    }
    //concat ImagesDirectory + ImagesPrefix + index.str + ImagesSuffix
    strcpy(pcaPath, config->spImagesDirectory);
    strcat(pcaPath, config->spPCAFilename);
    return SP_CONFIG_SUCCESS ;
}


void spConfigDestroy(SPConfig config){
	if(config != NULL){
		free (config->spImagesDirectory);
		free (config->spImagesPrefix);
		free (config->spImagesSuffix);
		free (config->spPCAFilename);
		free (config->spLoggerFilename);
		free (config);
	}
}

bool isEmpty (const SPConfig config){
	if (config == NULL){
		return true;
	}
	return false;

}
