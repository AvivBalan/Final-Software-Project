/*
 * KDArray.c
 *
 *  Created on: 18 баев 2016
 *      Author: рвд
 */

# include "KDArray.h"

typedef struct SPKDArray{
	struct SPPoint* p;
	int** matrix;

};


struct SPKDArray Init(struct SPPoint* arr, int size){
	struct SPKDArray newSPKDArray = (struct SPKDArray)malloc (sizeof (*newSPKDArray));
	newSPKDArray -> p = copySPPointArray (arr, size);
	newSPKDArray -> matrix = createMatrix (arr, size);
	return newSPKDArray;
}

struct SPKDArray**  Split(struct SPKDArray kdArr, int coor){

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
