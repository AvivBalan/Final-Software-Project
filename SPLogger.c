#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

//Creates a logger.
SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		printf("ERROR - Memory Allocation Failure\n");
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			printf("ERROR - Couldn't open logger file - %s\n", filename);
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

//Frees all memory allocated for the logger.
void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

//Prints error message.
SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line) {

	//In case the logger is undefined
	if(logger == NULL){ //
		return SP_LOGGER_UNDIFINED;
	}

	//In case the level of the logger is not matching
	if(logger->level != SP_LOGGER_ERROR_LEVEL && logger->level != SP_LOGGER_WARNING_ERROR_LEVEL && logger->level != SP_LOGGER_INFO_WARNING_ERROR_LEVEL && logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}

	//In case one of the arguments is invalid
	if(msg == NULL || file == NULL || function == NULL || line < 0){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if(logger->isStdOut){ //In case the output channel is stdout
		if(printf("---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",file,function,line,msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}
	else {
		if(fprintf(logger->outputChannel,"---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",file,function,line,msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}

	return SP_LOGGER_SUCCESS;
}

//Prints warning message.
SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line) {

	//In case the logger is undefined
	if(logger == NULL){ //
		return SP_LOGGER_UNDIFINED;
	}

	//In case the level of the logger is not matching
	if(logger->level != SP_LOGGER_WARNING_ERROR_LEVEL && logger->level != SP_LOGGER_INFO_WARNING_ERROR_LEVEL && logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}

	//In case one of the arguments is invalid
	if(msg == NULL || file == NULL || function == NULL || line < 0){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if(logger->isStdOut){ //In case the output channel is stdout
		if(printf("---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",file,function,line,msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}
	else {
		if(fprintf(logger->outputChannel,"---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",file,function,line,msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}

	return SP_LOGGER_SUCCESS;
}

//Prints Info message.
SP_LOGGER_MSG spLoggerPrintInfo(const char* msg) {

	//In case the logger is undefined
	if(logger == NULL){ //
		return SP_LOGGER_UNDIFINED;
	}

	//In case the level of the logger is not matching
	if(logger->level != SP_LOGGER_INFO_WARNING_ERROR_LEVEL && logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}

	//In case one of the arguments is invalid
	if(msg == NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if(logger->isStdOut){ //In case the output channel is stdout
		if(printf("---INFO---\n- message: %s\n",msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}
	else {
		if(fprintf(logger->outputChannel,"---INFO---\n- message: %s\n",msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}

	return SP_LOGGER_SUCCESS;
}

//Prints the debug message.
SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line) {

	//In case the logger is undefined
	if(logger == NULL){
		return SP_LOGGER_UNDIFINED;
	}

	//In case the level of the logger is not matching
	if(logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}

	//In case one of the arguments is invalid
	if(msg == NULL || file == NULL || function == NULL || line < 0){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if(logger->isStdOut){ //In case the output channel is stdout
		if(printf("---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",file,function,line,msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}
	else {
		if(fprintf(logger->outputChannel,"---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",file,function,line,msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}

	return SP_LOGGER_SUCCESS;
}

//The given message is printed.
SP_LOGGER_MSG spLoggerPrintMsg(const char* msg) {

	//In case the logger is undefined
	if(logger == NULL){
		return SP_LOGGER_UNDIFINED;
	}

	//In case one of the arguments is invalid
	if(msg == NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if(logger->isStdOut){ //In case the output channel is stdout
		if(printf("%s\n",msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}
	else {
		if(fprintf(logger->outputChannel,"%s\n",msg) < 0){
			return SP_LOGGER_WRITE_FAIL; //If Write failure occurred
		}
	}

	return SP_LOGGER_SUCCESS;
}
