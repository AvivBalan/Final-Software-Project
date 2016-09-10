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



int** buildTwoMaps (int* sortedPoints, int len){
	int **maps;
	int i=0;

	maps=(int **) malloc(2*sizeof(int *));
	for(i=0;i<2;i++){
		maps[i]=(int *) malloc(len*sizeof(int));
	}

	i =0;
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
		if (i<(half)){
			//printf ("%d",half);
			maps[0][index] = cnt1;
			maps[1][index] = -1;
			cnt1++;
		}
		else{
			//printf ("g");
			maps[0][index] = -1;
			maps[1][index] = cnt2;
			cnt2++;
		}
	}

	return maps;
}

 SPPoint** splitPointArray (int**maps ,  SPPoint* array, int len){
	SPPoint** pointArrays;
	int i=0;
	pointArrays=(SPPoint**) malloc(2*sizeof(SPPoint *));
	for(i=0;i<2;i++){
		pointArrays[i]=(SPPoint *) malloc(len*sizeof(SPPoint));
	}
	i =0;
	//int* map1 = maps [0];
	//printf ("%d",map1[0]);
	//int ** maps2 = maps;

	for (i =0; i<len;i++){

		if (maps[0][i]==-1){


			int j = maps[1][i];
			pointArrays[1][j] = array[i];
		}
		else{

			//maps[0][i];
			int j = maps[0][i];
			pointArrays[0][j] = array[i];
		}
	}
	return pointArrays;

}

int*** splitMatrixes (int**maps, int**matrix, int len, int dim){
	int*** matrixArrays;
	int i=0;
	int j=0;
	matrixArrays=(int***) malloc(2*sizeof(int **));
	for(i=0;i<2;i++){
		matrixArrays[i]=(int **) malloc(dim*sizeof(int*));
		for (j=0;j<dim;j++){
			matrixArrays[i][j]=(int*) malloc(len*sizeof(int));
		}
	}
	i =0;
	j=0;
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

void SPKDArrayDestroy (SPKDArray kdArr, int len){

	if(kdArr != NULL) {
		int i=0;
		for(i=0;i< spPointGetDimension(kdArr->p[0]) ;i++){
			free (kdArr->matrix[i]);
		}
		free (kdArr->matrix);

		i=0;
		for (i=0; i<len;i++){
			spPointDestroy (kdArr->p[i]);
		}
		free (kdArr->p);
		free(kdArr);
	}


}


SPKDArray**  Split( SPKDArray kdArr, int coor, int len){
	int** matrix = (kdArr->matrix);
	int * matcoor = matrix [coor];
	int ** maps =  buildTwoMaps(matcoor,len);


	SPPoint** pointArrays = ( SPPoint**) malloc (sizeof (kdArr->p));
	pointArrays = splitPointArray (maps, kdArr->p, len);
	int*** matrixes = (int***) malloc (sizeof (kdArr-> matrix));
	matrixes = splitMatrixes (maps, kdArr-> matrix,len,  spPointGetDimension((kdArr->p)[0]));
	 SPKDArray* newSPKDArray = ( SPKDArray*) malloc (2*sizeof ( SPKDArray));
	 SPKDArray left = newSPKDArray[0];
	 SPKDArray right = newSPKDArray[1];
	left -> p =pointArrays[0];
	right->p = pointArrays[1];
	left->matrix =matrixes[0];
	right->matrix=matrixes[1];

	free(maps);
	SPKDArrayDestroy (kdArr, len);
	return &newSPKDArray;

}
