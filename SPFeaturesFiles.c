#include "SPFeaturesFiles.h"

int spFeatureCreateFeatureFile(SPPoint* pointArray, const char* filename, int numOfFeatures, int numOfDim){
	FILE *featureFile = NULL;
	int i,j;

	if(filename == NULL){
		//ERORR
		return 1;
	}

	featureFile = fopen(filename,"w");
	if (featureFile == NULL){
		//ERORR
		return 1;
	}
	fprintf(featureFile,"%d;;%d\n",numOfFeatures,numOfDim);
	for (i = 0; i < numOfFeatures; ++i) {
		fprintf(featureFile,"%d;;",spPointGetIndex(pointArray[i]));
		for (j = 0; j < numOfDim; ++j) {
			fprintf(featureFile,"%f;;",spPointGetAxisCoor(pointArray[i],j));
		}
		fprintf(featureFile, "\n");
	}

	fclose(featureFile);
	return 0;
}

SPPoint* spFeatureExtractFromFeatureFile(const char* filename, int *numOfFeatures, int *numOfDim){
	FILE *featureFile = NULL;
	SPPoint* featureArray = NULL;
	int i,j;
	char* str = NULL;
	if(filename == NULL){
		//ERORR
		return NULL;
	}

	featureFile = fopen(filename,"r");
	if (featureFile == NULL){
		//ERORR
		return NULL;
	}
	if(fscanf(featureFile,"%d;;%d\n",numOfFeatures,numOfDim) != 2){
		//ERORR
		return NULL;
	}

	featureArray = (SPPoint*) malloc(*numOfFeatures * sizeof(SPPoint));
	if(featureArray == NULL){
		//ERORR
		return NULL;
	}

	for (i = 0; i < *numOfFeatures; ++i) {
		int imgIndex;
		double *coorArray;
		coorArray = (double*) malloc(*numOfDim * sizeof(double));
		if(coorArray == NULL){
			//ERORR
			return NULL;
		}
		if(fscanf(featureFile,"%d;;",&imgIndex) != 1){
			//ERORR
			return NULL;
		}

		for (j = 0; j < *numOfDim-1; ++j) {
			if(fscanf(featureFile,"%lf;;",&coorArray[j]) != 1){
				//ERORR
				return NULL;
			}
		}
		if(fscanf(featureFile,"%lf;;\n",&coorArray[*numOfDim - 1]) != 1){
			//ERORR
			return NULL;
		}

		featureArray[i] = spPointCreate(coorArray, *numOfDim, imgIndex);
		if(featureArray[i] == NULL){
			//ERORR
			return NULL;
		}
		free(coorArray);
	}
	if(fscanf(featureFile,"%s",str) == 1){
		//ERORR
		return NULL;
	}

	fclose(featureFile);
	return featureArray;
}
