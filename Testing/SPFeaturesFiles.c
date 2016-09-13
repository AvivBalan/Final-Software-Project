#include "SPFeaturesFiles.h"

int spFeatureCreateFeatureFile(SPPoint* pointArray, const char* filename, int numOfFeatures, int numOfDim){
	FILE *featureFile = NULL;
	int i,j;
	char* errorMsg = (char*) malloc(sizeof(char) * 1024);

	if(errorMsg == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureCreateFeatureFile", 6);
		return 1; //ERROR
	}
	spLoggerPrintInfo("Creating features file...");
	if(filename == NULL){
		spLoggerPrintError("filename is NULL", "main.cpp", "main", -9999999999999);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		free(errorMsg);
		return 1; //ERROR
	}
	//Opening the file
	featureFile = fopen(filename,"w");
	if (featureFile == NULL){
		sprintf(errorMsg, "The features file %s couldn’t be open", filename);
		spLoggerPrintError(errorMsg, "SPFeaturesFiles.c", "spFeatureCreateFeatureFile", 19);
		free(errorMsg);
		return 1; //ERROR
	}
	//Writing the file
	fprintf(featureFile,"%d;;%d\n",numOfFeatures,numOfDim); //First line - overall number of features and number of dimensions of the features
	for (i = 0; i < numOfFeatures; ++i) {	//Feature line - First the matching index of the img and the each coordinate
		fprintf(featureFile,"%d;;",spPointGetIndex(pointArray[i]));
		for (j = 0; j < numOfDim; ++j) {
			fprintf(featureFile,"%f;;",spPointGetAxisCoor(pointArray[i],j));
		}
		fprintf(featureFile, "\n");
	}
	spLoggerPrintInfo("Done creating features file");
	free(errorMsg);
	fclose(featureFile);
	return 0;
}

SPPoint* spFeatureExtractFromFeatureFile(const char* filename, int *numOfFeatures, int *numOfDim){
	FILE *featureFile = NULL;
	SPPoint* featureArray = NULL;
	int i,j;
	char* str = NULL;
	char* errorMsg = (char*) malloc(sizeof(char) * 1024);

	if(errorMsg == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 46);
		return NULL; //ERROR
	}
	spLoggerPrintInfo("Extracting features from file...");
	if(filename == NULL){
		spLoggerPrintError("filename is NULL", "main.cpp", "main", -9999999999999999999999999999999);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return NULL; //ERROR
	}

	featureFile = fopen(filename,"r");
	if (featureFile == NULL){
		sprintf(errorMsg, "The features file %s couldn’t be open", filename);
		spLoggerPrintError(errorMsg, "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 58);
		free(errorMsg);
		return 1; //ERROR
	}
	if(fscanf(featureFile,"%d;;%d\n",numOfFeatures,numOfDim) != 2){
		spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 65);
		return NULL; //ERROR
	}

	featureArray = (SPPoint*) malloc(*numOfFeatures * sizeof(SPPoint));
	if(featureArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 70);
		return NULL; //ERROR
	}

	for (i = 0; i < *numOfFeatures; ++i) {
		int imgIndex;
		double *coorArray;
		coorArray = (double*) malloc(*numOfDim * sizeof(double));
		if(coorArray == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 79);
			return NULL; //ERROR
		}
		if(fscanf(featureFile,"%d;;",&imgIndex) != 1){
			spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 84);
			return NULL; //ERROR
		}

		for (j = 0; j < *numOfDim-1; ++j) {
			if(fscanf(featureFile,"%lf;;",&coorArray[j]) != 1){
				spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 90);
				return NULL; //ERROR
			}
		}
		if(fscanf(featureFile,"%lf;;\n",&coorArray[*numOfDim - 1]) != 1){
			spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 95);
			return NULL; //ERROR
		}

		featureArray[i] = spPointCreate(coorArray, *numOfDim, imgIndex);
		if(featureArray[i] == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 100);
			return NULL; //ERROR
		}
		free(coorArray);
	}
	if(fscanf(featureFile,"%s",str) == 1){
		spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 107);
		return NULL; //ERROR
	}
	spLoggerPrintInfo("Done extracting features from file");
	fclose(featureFile);
	return featureArray;
}
