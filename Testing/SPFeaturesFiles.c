#include "SPFeaturesFiles.h"

int spFeatureCreateFeatureFile(SPPoint* pointArray, const char* filename, int numOfImages,int numOfFeatures, int numOfDim){
	FILE *featureFile = NULL;
	int i,j;
	char errorMsg[1025];

	spLoggerPrintInfo("Creating features file...");
	if(filename == NULL){
		spLoggerPrintError("filename is NULL", "main.cpp", "main", 182);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return 1; //ERROR
	}
	//Opening the file
	featureFile = fopen(filename,"w");
	if (featureFile == NULL){
		sprintf(errorMsg, "The features file %s couldn’t be open", filename);
		spLoggerPrintError(errorMsg, "SPFeaturesFiles.c", "spFeatureCreateFeatureFile", 14);
		return 1; //ERROR
	}
	//Writing the file
	//First line - 1- number of images 2- overall number of features 3- number of dimensions of the features
	fprintf(featureFile,"%d;;%d;;%d\n",numOfImages,numOfFeatures,numOfDim);
	for (i = 0; i < numOfFeatures; ++i) {	//Feature line - First the matching index of the img and the each coordinate
		fprintf(featureFile,"%d;;",spPointGetIndex(pointArray[i]));
		for (j = 0; j < numOfDim; ++j) {
			fprintf(featureFile,"%f;;",spPointGetAxisCoor(pointArray[i],j));
		}
		fprintf(featureFile, "\n");
	}
	spLoggerPrintInfo("Done creating features file");
	fclose(featureFile);
	return 0;
}

SPPoint* spFeatureExtractFromFeatureFile(const char* filename, int numOfImages,int *numOfFeatures, int *numOfDim){
	FILE *featureFile = NULL;
	SPPoint* featureArray = NULL;
	int i,j, numOfImagesFile;
	char* str = NULL;
	char errorMsg[1025];

	spLoggerPrintInfo("Extracting features from file...");
	if(filename == NULL){
		spLoggerPrintError("filename is NULL", "main.cpp", "main", 184);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return NULL; //ERROR
	}

	featureFile = fopen(filename,"r");
	if (featureFile == NULL){
		sprintf(errorMsg, "The features file %s couldn’t be open", filename);
		spLoggerPrintError(errorMsg, "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 48);
		return NULL; //ERROR
	}
	if(fscanf(featureFile,"%d;;%d;;%d\n",&numOfImagesFile,numOfFeatures,numOfDim) != 3){
		spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 54);
		return NULL; //ERROR
	}
	if(numOfImages != numOfImagesFile){
		spLoggerPrintError("Number of images from features file isn't matching the given configuration", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 54);
		return NULL; //ERROR
	}

	featureArray = (SPPoint*) malloc(*numOfFeatures * sizeof(SPPoint));
	if(featureArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 63);
		return NULL; //ERROR
	}

	for (i = 0; i < *numOfFeatures; ++i) {
		int imgIndex;
		double *coorArray;
		coorArray = (double*) malloc(*numOfDim * sizeof(double));
		if(coorArray == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 72);
			return NULL; //ERROR
		}
		if(fscanf(featureFile,"%d;;",&imgIndex) != 1){
			spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 77);
			return NULL; //ERROR
		}

		for (j = 0; j < *numOfDim-1; ++j) {
			if(fscanf(featureFile,"%lf;;",&coorArray[j]) != 1){
				spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 83);
				return NULL; //ERROR
			}
		}
		if(fscanf(featureFile,"%lf;;\n",&coorArray[*numOfDim - 1]) != 1){
			spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 88);
			return NULL; //ERROR
		}

		featureArray[i] = spPointCreate(coorArray, *numOfDim, imgIndex);
		if(featureArray[i] == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 94);
			return NULL; //ERROR
		}
		free(coorArray);
	}
	if(fscanf(featureFile,"%s",str) == 1){
		spLoggerPrintError("Wrong feature file format - couldn't read", "SPFeaturesFiles.c", "spFeatureExtractFromFeatureFile", 100);
		return NULL; //ERROR
	}
	spLoggerPrintInfo("Done extracting features from file");
	fclose(featureFile);
	return featureArray;
}
