/*
 * KDArray.c
 *
 *  Created on: 18 баев 2016
 *      Author: рвд
 */

# include "KDArray.h"

struct sp_kd_array_t{
	SPPoint p;
	int** matrix;

};


SPKDArray Init(SPPoint* arr, int size){
	SPKDArray newSPKDArray = (SPKDArray) malloc(sizeof(*newSPKDArray));
	newSPKDArray->p = copySPPointArray(arr, size);
	newSPKDArray->matrix = createMatrix(arr, size);
	return newSPKDArray;
}

SPKDArray**  Split(SPKDArray kdArr, int coor){
	int ** maps = (int**)malloc (length(kdArr->p)*2);
	maps = buidTwoMaps(kdArr->matrix[coor]);
	SPPoint** pointArrays = (SPPoint**) malloc (length(kdArr->p)*sizeof (SPPoint));
	pointArrays = splitPointArray (maps, kdArr->p);
	int*** matrixes = (int***) malloc (sizeof (kdArr-> matrix));
	matrixes = splitMatrixes (maps, kdArr-> matrix);
	SPKDArray left = (SPKDArray) malloc (sizeof (SPKDArray));
	SPKDArray right = (SPKDArray) malloc (sizeof (SPKDArray));
	left -> p =pointArrays[0];
	right->p = pointArrays[1];
	left->matrix =matrixes[0];
	right->matrix=matrixes[1];

	free(maps);
	SPKDArrayDestroy(kdArr);

}

struct SPPoint* copySPPointArray (struct SPPoint* arr, int size){
	struct SPPoint* newPointArray = (struct SPPoint*) malloc(sizeof(struct SPPoint)*size);
	int i =0;
	for (i=0; i < size; i++){
		newPointArray[i] = spPointCopy(arr[i]);
	}
	return newPointArray;
}

int** createMatrix (struct SPPoint* arr, int size){
	int dim = spPointGetDimension(arr);
	int** matrix = (int**) malloc (sizeof(int*)*dim*sizeof(int)*size);
	int i =0;
	int j = 0;
	struct SPPoint* sortedPointArray = copySPPointArray (arr,  size);

	for (i=0; i<dim; i++){
		// get a array of sorted points by the first dim
		int (*compar)(const void *, const void*) = comperatorCreator (i);
		qsort (sortedPointArray,size, sizeof (struct SPPoint),compar);
		for (j=0; j<size; j++){
			matrix [i][j] = spPointGetIndex(sortedPointArray[j]);

		}
		free (sortedPointArray);
	}

	return matrix;


}

int (*comperatorCreator(int dim))(const void *, const void*){
	int (*func)(const void *, const void*);
	int compare ( const void *p1 ,  const void*p2){
		if ( spPointGetAxisCoor( *p1, dim) < spPointGetAxisCoor(*p2, dim)){
			return 1;
		}
		if  ( spPointGetAxisCoor( *p1, dim) > spPointGetAxisCoor(*p2, dim)){
			return -1;
		}
		return 0;
	}
	func = &compare;
	return func;


}

int** buidTwoMaps (int* sortedPoints,struct SPPoint* arr ){
	int** maps = (int**) malloc (length (sortedPoints)*2);
	int len = length (sortedPoints);
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
		int index = spPointGetIndex(sortedPoints[i]);
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

struct SPPoint** splitPointArray (int**maps , struct SPPoint* array){
	struct SPPoint** pointArrays = (struct SPPoint**) malloc (length (array)*2);
	int len = length (array);
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
		int len = length (*maps);
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
	for (i=0; i<length(kdArr->p);i++){
		spPointDestroy (kdArr->p[i]);
	}
	free (kdArr);

	for(i=0;i<length(kdArr->matrix);i++){
		free (kdArr->matrix[i]);
	}
	free (kdArr->matrix);
}
