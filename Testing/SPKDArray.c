#include "SPKDArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int globalDim;
SPPoint* globalPointArray;

 struct sp_kd_Array{
	SPPoint* pArray;
	int arraySize;
	int** matrix;
};


 SPPoint* getPoint(SPKDArray kdArr){
	return kdArr->pArray;
}

int** getMatrix (SPKDArray kdArr){
	return kdArr->matrix;
}

 SPKDArray spKDArrayCreate(SPPoint* array, int arraySize){

	SPKDArray newSPKDArray = (SPKDArray) malloc(sizeof(*newSPKDArray));
	if(newSPKDArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayCreate", 25);
		return NULL; //ERROR
	}
	newSPKDArray->pArray = spKDArrayCopyPointArray (array, arraySize);
	if(newSPKDArray->pArray == NULL){
		return NULL; //ERROR
	}
	newSPKDArray->matrix = spKDArrayCreateMatrix (array, arraySize);
	if(newSPKDArray->matrix == NULL){
		return NULL;//ERROR
	}
	newSPKDArray->arraySize = arraySize;
	return newSPKDArray;
}

 SPPoint* spKDArrayCopyPointArray(SPPoint* arr, int arraySize){
	 int i;

	SPPoint* newPointArray = (SPPoint*) malloc(sizeof(SPPoint)*arraySize);
	if(newPointArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayCopyPointArray", 45);
		return NULL; //ERROR
	}
	for (i = 0; i < arraySize; i++){
		newPointArray[i] = spPointCopy(arr[i]);
		if(newPointArray[i] == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayCopyPointArray", 51);
			return NULL; //ERROR
		}
	}
	return newPointArray;
}

int spKDArrayComparePoints(const void * a, const void * b){
	double valueA = spPointGetAxisCoor( globalPointArray[*(int*) a],  globalDim);
	double valueB = spPointGetAxisCoor(  globalPointArray[*(int*) b],  globalDim);

	if (valueA < valueB){
		return -1;
	}
	if  (valueA > valueB){
		return 1;
	}
	return 0;
}

int** spKDArrayCreateMatrix (SPPoint* arr, int arraySize){
	int **matrix, i, j, dim, *indexArray;

	dim = spPointGetDimension(arr[0]);
	globalPointArray = arr;
	indexArray = (int*) malloc(arraySize * sizeof(i));
	if(indexArray == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayCreateMatrix", 78);
		return NULL; //ERROR
	}
	for (i = 0; i < arraySize; ++i) {
		indexArray[i] = i;
	}

	matrix = (int **) malloc(dim * sizeof(int*));
	if(matrix == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayCreateMatrix", 87);
		free(indexArray);
		return NULL; //ERROR
	}

	for(i = 0; i < dim; i++){
	    matrix[i] = (int *) malloc(arraySize * sizeof(int));
		if(matrix[i] == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayCreateMatrix", 95);
			for (j = 0; j < i; ++j) {
				free(matrix[j]);
			}
			free(matrix);
			free(indexArray);
			return NULL; //ERROR
		}
	}

	for (i = 0; i < dim; i++){
		// get a array of sorted points by the first dim
		globalDim = i;
		qsort(indexArray, arraySize, sizeof(int), spKDArrayComparePoints);
		for (j = 0; j < arraySize; j++){
			matrix [i][j] = indexArray[j];
		}
	}
	free(indexArray);
	return matrix;
}

void spKDArrayFreeMemForSplit(int phase, SPKDArray kdArr,int **maps,int ***newMatrixes, SPPoint** newPointArrays, SPKDArray* newKDArrays){
	int i, j, rightNumOfPoints;

	if(phase >= 5){
		free(newKDArrays[0]);
	}
	if(phase >= 4){
		free(newKDArrays);
	}
	if(phase >= 3){
		for (i = 0; i < 2; ++i) {
			for(j = 0; j < spPointGetDimension(kdArr->pArray[0]); j++){
				free(newMatrixes[i][j]);
			}
			free(newMatrixes[i]);
		}
	}
	if(phase >= 2){
		rightNumOfPoints = floor(kdArr->arraySize / 2);
		int newNumOfPoints[] = {kdArr->arraySize - rightNumOfPoints, rightNumOfPoints};
		for (i = 0; i < 2; ++i) {
			for (j = 0; j < newNumOfPoints[i]; ++j)
				spPointDestroy(newPointArrays[i][j]);
		}
		free(newPointArrays[i]);
		free(newPointArrays);
	}
	if(phase >= 0){
		free(maps[0]);
		free(maps[1]);
		free(maps);
	}
	if(phase == 0){ //success
		free(newPointArrays);
		free(newMatrixes);
	}
}

SPKDArray* spKDArraySplit(SPKDArray kdArr, int dim){
	int **maps, i;
	SPPoint** newPointArrays;
	SPKDArray* newKDArrays = NULL;
	int ***newMatrixes = NULL;
	int newArraySize[] = {kdArr->arraySize - floor(kdArr->arraySize / 2), floor(kdArr->arraySize / 2)};

	maps =  spKDArrayBuildMaps(kdArr->matrix[dim], kdArr->arraySize);
	if(maps == NULL)
		return NULL; //ERROR

	newPointArrays = spKDArraySplitPointArray(maps, kdArr->pArray, kdArr->arraySize);
	if(newPointArrays == NULL){
		spKDArrayFreeMemForSplit(1, kdArr, maps, newMatrixes, newPointArrays, newKDArrays);
		return NULL; //ERROR
	}

	newMatrixes = spKDArraySplitMatrixes (maps, kdArr->matrix, kdArr->arraySize, spPointGetDimension((kdArr->pArray)[0]));
	if(newMatrixes == NULL){
		spKDArrayFreeMemForSplit(2, kdArr, maps, newMatrixes, newPointArrays, newKDArrays);
		return NULL; //ERROR
	}

	newKDArrays = (SPKDArray*) malloc(2 * sizeof(SPKDArray));
	if(newKDArrays == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplit", 180);
		spKDArrayFreeMemForSplit(3, kdArr, maps, newMatrixes, newPointArrays, newKDArrays);
		return NULL; //ERROR
	}
	newKDArrays[0] = (SPKDArray) malloc(sizeof(*newKDArrays[0]));
	if(newKDArrays[0] == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplit", 186);
		spKDArrayFreeMemForSplit(4, kdArr, maps, newMatrixes, newPointArrays, newKDArrays);
		return NULL; //ERROR
	}
	newKDArrays[1] = (SPKDArray) malloc(sizeof(*newKDArrays[1]));
	if(newKDArrays[0] == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplit", 192);
		spKDArrayFreeMemForSplit(5, kdArr, maps, newMatrixes, newPointArrays, newKDArrays);
		return NULL; //ERROR
	}

	for (i = 0; i < 2; ++i) {
		newKDArrays[i]->pArray = newPointArrays[i];
		newKDArrays[i]->arraySize = newArraySize[i];
		newKDArrays[i]->matrix = newMatrixes[i];
	}

	spKDArrayFreeMemForSplit(0, kdArr, maps, newMatrixes, newPointArrays, newKDArrays);
	return newKDArrays;
}

int** spKDArrayBuildMaps (int* sortedPointIndexes, int arraySize){
	int **maps, i, medianIndex, leftCounter, rightCounter, *pointToMap;

	pointToMap = (int *) malloc(arraySize * sizeof(int));
	if(pointToMap == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayBuildMaps", 212);
		return NULL; //ERROR
	}

	maps = (int **) malloc(2 * sizeof(int *));
	if(maps == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayBuildMaps", 218);
		free(pointToMap);
		return NULL; //ERROR
	}
	maps[0] = (int *) malloc(arraySize * sizeof(int));
	if(maps[0] == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayBuildMaps", 224);
		free(pointToMap);
		free(maps);
		return NULL; //ERROR
	}
	maps[1] = (int *) malloc(arraySize * sizeof(int));
	if(maps[1] == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArrayBuildMaps", 231);
		free(pointToMap);
		free(maps[0]);
		free(maps);
		return NULL; //ERROR
	}

	medianIndex = arraySize - floor(arraySize/2);

	for(i = 0; i < medianIndex; ++i) {
		pointToMap[sortedPointIndexes[i]] = 0;
	}
	for(i = medianIndex; i < arraySize; ++i) {
		pointToMap[sortedPointIndexes[i]] = 1;
	}

	leftCounter = 0;
	rightCounter = 0;
	for(i = 0; i < arraySize; ++i) {
		if(pointToMap[i] == 0){
			maps[0][i] = leftCounter;
			maps[1][i] = -1;
			leftCounter++;
		}
		else{
			maps[0][i] = -1;
			maps[1][i] = rightCounter;
			rightCounter++;
		}
	}
	free(pointToMap);
	return maps;
}

 SPPoint** spKDArraySplitPointArray (int** maps, SPPoint* array, int arraySize){
	SPPoint **newPointArrays, *copiedArray;
	int i, leftArraySize;

	copiedArray = spKDArrayCopyPointArray(array, arraySize);
	if(copiedArray == NULL){
		return NULL; //ERROR
	}

	leftArraySize = arraySize - floor(arraySize/2);

	newPointArrays = (SPPoint**) malloc(2 * sizeof(SPPoint *));
	if(newPointArrays == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplitPointArray", 278);
		for(i = 0; i < arraySize; i++){
			spPointDestroy(copiedArray[i]);
		}
		free(copiedArray);
		return NULL; //ERROR
	}

	newPointArrays[0] = (SPPoint*) malloc(leftArraySize * sizeof(SPPoint));
	if(newPointArrays[0] == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplitPointArray", 288);
		for(i = 0; i < arraySize; i++){
			spPointDestroy(copiedArray[i]);
		}
		free(copiedArray);
		free(newPointArrays);
		return NULL; //ERROR
	}
	newPointArrays[1] = (SPPoint*) malloc((arraySize -leftArraySize) * sizeof(SPPoint));
	if(newPointArrays[1] == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplitPointArray", 298);
		for(i = 0; i < arraySize; i++){
			spPointDestroy(copiedArray[i]);
		}
		free(copiedArray);
		free(newPointArrays[0]);
		free(newPointArrays);
		return NULL; //ERROR
	}

	for (i = 0; i < arraySize; i++){
		int newIndex;
		if(maps[0][i] == -1){
			newIndex = maps[1][i];
			newPointArrays[1][newIndex] = copiedArray[i];
		}
		else{
			newIndex = maps[0][i];
			newPointArrays[0][newIndex] = copiedArray[i];
		}
	}
	free(copiedArray);
	return newPointArrays;
}

int*** spKDArraySplitMatrixes (int** maps, int** matrix, int numOfPoints, int numOfDimensions){
	int ***newMatrixes, i, j, k, newIndex, leftCounter, rightCounter, newNumOfPoints[2];

	newMatrixes = (int***) malloc(2 * sizeof(int **));
	if(newMatrixes == NULL){
		spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplitMatrixes", 328);
		return NULL; //ERROR
	}

	newNumOfPoints[0] = numOfPoints - floor(numOfPoints/2);
	newNumOfPoints[1] = floor(numOfPoints/2);

	for(i = 0; i < 2; i++){
		newMatrixes[i] = (int**) malloc(numOfDimensions * sizeof(int *));
		if(newMatrixes[i] == NULL){
			spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplitMatrixes", 338);
			if(i == 1)
			{
				for(k = 0; k < newNumOfPoints[0]; k++)
					free(newMatrixes[0][k]);
				free(newMatrixes[0]);
			}
			free(newMatrixes);
			return NULL; //ERROR
		}
			for(j = 0; j < numOfDimensions; j++){
				newMatrixes[i][j] = (int*) malloc(newNumOfPoints[i] * sizeof(int));
				if(newMatrixes[i][j] == NULL){
					spLoggerPrintError("Memory Allocation Failure", "SPDKArray.c", "spKDArraySplitMatrixes", 351);
					if(i == 1)
					{
						for(k = 0; k < numOfPoints; k++)
							free(newMatrixes[0][k]);
						free(newMatrixes[0]);
					}
					for(k = 0; k < j; k++)
						free(newMatrixes[i][k]);
					free(newMatrixes[i]);
					free(newMatrixes);
					return NULL; //ERROR
				}
		}
	}

	for (i = 0; i < numOfDimensions; i++){
		leftCounter = 0;
		rightCounter = 0;
		for (j = 0; j < numOfPoints; j++){
			if(maps[0][matrix[i][j]] == -1){
				newIndex = maps[1][matrix[i][j]];
				newMatrixes[1][i][rightCounter] = newIndex;
				rightCounter++;
			}
			else{
				newIndex = maps[0][matrix[i][j]];
				newMatrixes[0][i][leftCounter] = newIndex;
				leftCounter++;
			}
		}
	}
	return newMatrixes;
}

void spKDArrayDestroy (SPKDArray kdArr){
	if(kdArr != NULL) {
		int i=0;
		for(i=0; i < spPointGetDimension(kdArr->pArray[0]); i++){
			free (kdArr->matrix[i]);
		}
		free (kdArr->matrix);

		i=0;
		for (i = 0; i < kdArr->arraySize; i++){
			spPointDestroy (kdArr->pArray[i]);
		}
		free (kdArr->pArray);
		free(kdArr);
	}
}

double spKDArrayGetMedianValue(SPKDArray kdArr, int dim){
	int medianIndex = kdArr->arraySize - floor(kdArr->arraySize / 2) - 1;
	return spPointGetAxisCoor(kdArr->pArray[kdArr->matrix[dim][medianIndex]], dim);
}

SPPoint spKDArrayGetPoint(SPKDArray kdArr, int index){
	return spPointCopy(kdArr->pArray[index]);
}

int spKDArrayFindMaxSpreadDim(SPKDArray kdArr){
	int i, maxSpreadDim, minIndex, min, maxIndex, max;
	double maxSpreadValue = -1;

	maxSpreadDim = 0;

	for (i = 0; i < spPointGetDimension(kdArr->pArray[0]); ++i) {
		minIndex = kdArr->matrix[i][0];
		min = spPointGetAxisCoor(kdArr->pArray[minIndex],i);
		maxIndex = kdArr->matrix[i][kdArr->arraySize - 1];
		max = spPointGetAxisCoor(kdArr->pArray[maxIndex],i);
		if(max - min > maxSpreadValue){
			maxSpreadValue = max - min;
			maxSpreadDim = i;
		}
	}
	return maxSpreadDim;
}
