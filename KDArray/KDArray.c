/*
 * KDArray.c
 *
 *  Created on: 18 баев 2016
 *      Author: рвд
 */

#include "KDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
int gobalDim;

 struct sp_kd_Array{
	 SPPoint* p;
	int** matrix;
};

 SPPoint* getPoint (SPKDArray arr){
	return arr->p;
}

int** getMatrix ( SPKDArray arr){
	return arr->matrix;
}


 SPKDArray Init( SPPoint* arr, int size){
	SPKDArray newSPKDArray = ( SPKDArray)malloc (sizeof (*newSPKDArray));
	newSPKDArray -> p = copySPPointArray (arr, size);
	newSPKDArray -> matrix = createMatrix (arr, size);
	return newSPKDArray;
}



 SPPoint* copySPPointArray ( SPPoint* arr, int size){
	 SPPoint* newPointArray = ( SPPoint*) malloc(sizeof( SPPoint)*size);
	int i =0;
	for (i=0; i < size; i++){
		newPointArray[i] = spPointCopy(arr[i]);
	}
	return newPointArray;
}

int compare (const SPPoint *a, const SPPoint *b)
{
	if ( spPointGetAxisCoor( *a,  gobalDim) < spPointGetAxisCoor(*b,  gobalDim)){
		return -1;
	}
	if  ( spPointGetAxisCoor( *a,  gobalDim) > spPointGetAxisCoor(*b,  gobalDim)){
		return 1;
	}
	return 0;
}
int** createMatrix ( SPPoint* arr, int size){
	//printf("Hello world");
	int dim = spPointGetDimension(arr[0]);
	int **matrix;
	int i=0;
	matrix=(int **) malloc(dim*sizeof(int *));
	for(i=0;i<dim;i++){
	    matrix[i]=(int *) malloc(size*sizeof(int));
	}

	i =0;
	int j = 0;
	SPPoint* sortedPointArray = copySPPointArray (arr,  size);

	for (i=0; i<dim; i++){

		// get a array of sorted points by the first dim
		gobalDim = i;
		qsort (sortedPointArray,size, sizeof ( SPPoint),compare);
		for (j=0; j<size; j++){

			matrix [i][j] = spPointGetIndex(sortedPointArray[j]);

		}

	}

	free (sortedPointArray);
	return matrix;


}




int** buildTwoMaps (int* sortedPoints){
	int** maps = (int**) malloc (sizeof (sortedPoints)*2);
	int len = sizeof (sortedPoints)/sizeof (int);
	int i =0;
	int half;
	int cnt1=0;
	int cnt2 =0;
	if (len%2 ==0){
		half = len/2;
	}
	else{
		half = floor (len/2)+1;
	}
	for (i=0;i<len;i++){
		int index = sortedPoints[i];
		if (i<half+1){
			maps[0][index] = cnt1;
			maps[1][index] = -1;
			cnt1++;
		}
		else{
			maps[0][index] = -1;
			maps[1][index] = cnt2;
			cnt2++;
		}
	}

	return maps;
}

 SPPoint** splitPointArray (int**maps ,  SPPoint* array){
	 SPPoint** pointArrays = ( SPPoint**) malloc ((sizeof (array)/sizeof (SPPoint))*2);
	int len = sizeof (array)/sizeof (SPPoint);
	int i=0;
	for (i =0; i<len;i++){
		if (maps[0][i]==-1){
			int j = maps[1][i];
			pointArrays[1][j] = array[i];
		}
		else{
			int j = maps[0][i];
			pointArrays[0][j] = array[i];
		}
	}
	return pointArrays;

}

int*** splitMatrixes (int**maps, int**matrix){
	int*** matrixArrays = (int***) malloc (sizeof(matrix));
		int len = sizeof (*maps)/sizeof(int);
		int i=0;
		for (i =0; i<len;i++){
			if (maps[0][i]==-1){
				int j = maps[1][i];
				matrixArrays [1][j] = matrix[i];
			}
			else{
				int j = maps[0][i];
				matrixArrays[0][j] = matrix[i];
			}
		}
		return matrixArrays;
}

void SPKDArrayDestroy (SPKDArray kdArr){
	int i=0;
	for (i=0; i<(sizeof (kdArr->p)/sizeof (SPPoint));i++){
		spPointDestroy (kdArr->p[i]);
	}
	free (kdArr->p);

	for(i=0;i<(sizeof (kdArr->matrix)/sizeof (int*));i++){
		free (kdArr->matrix[i]);
	}
	free (kdArr->matrix);
}


SPKDArray**  Split( SPKDArray kdArr, int coor){
	int** matrix = (kdArr->matrix);
	int * matcoor = matrix [coor];
	int ** maps =  buildTwoMaps(matcoor);


	SPPoint** pointArrays = ( SPPoint**) malloc (sizeof (kdArr->p));
	pointArrays = splitPointArray (maps, kdArr->p);
	int*** matrixes = (int***) malloc (sizeof (kdArr-> matrix));
	matrixes = splitMatrixes (maps, kdArr-> matrix);
	 SPKDArray* newSPKDArray = ( SPKDArray*) malloc (2*sizeof ( SPKDArray));
	 SPKDArray left = newSPKDArray[0];
	 SPKDArray right = newSPKDArray[1];
	left -> p =pointArrays[0];
	right->p = pointArrays[1];
	left->matrix =matrixes[0];
	right->matrix=matrixes[1];

	free(maps);
	SPKDArrayDestroy (kdArr);
	return &newSPKDArray;

}
