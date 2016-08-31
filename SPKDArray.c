#include "SPKDArray.h"

struct sp_kd_array_t{
	SPPoint* p;
	int** matrix;

};


SPKDArray Init(SPPoint* arr, int size){
	SPKDArray newArray = (SPKDArray) malloc(sizeof(*newArray));
	if(newArray == NULL) { //Allocation Fails
		return NULL;
	}
	newArray->p = copySPPointArray(arr, size);
	newArray->matrix = createMatrix(arr, size);
	return newArray;
}

SPKDArray*  Split(SPKDArray kdArr, int coor){
	int ** maps = (int**) malloc(length(kdArr->p)*2);
	if(maps == NULL) { //Allocation Fails
		return NULL;
	}
	maps = buidTwoMaps(kdArr->matrix[coor]);
	SPPoint** pointArrays = (SPPoint**) malloc(length(kdArr->p)*sizeof (SPPoint));
	if(pointArrays == NULL) { //Allocation Fails
		return NULL;
	}
	pointArrays = splitPointArray (maps, kdArr->p);
	int*** matrixes = (int***) malloc(sizeof (kdArr-> matrix));
	if(matrixes == NULL) { //Allocation Fails
		return NULL;
	}
	matrixes = splitMatrixes (maps, kdArr-> matrix);
	SPKDArray left = (SPKDArray) malloc(sizeof (SPKDArray));
	if(left == NULL) { //Allocation Fails
		return NULL;
	}
	SPKDArray right = (SPKDArray) malloc(sizeof (SPKDArray));
	if(right == NULL) { //Allocation Fails
		return NULL;
	}
	left -> p =pointArrays[0];
	right->p = pointArrays[1];
	left->matrix =matrixes[0];
	right->matrix=matrixes[1];

	free(maps);
	SPKDArrayDestroy(kdArr);

}

SPPoint* copySPPointArray(SPPoint* arr, int size){
	SPPoint* newArray = (SPPoint*) malloc(size*sizeof(SPPoint));
	if(newArray == NULL) { //Allocation Fails
		return NULL;
	}
	int i = 0;
	for (i = 0; i < size; i++){
		newArray[i] = spPointCopy(arr[i]);
	}
	return newArray;
}

int** createMatrix (SPPoint* arr, int size){
	int i, j, dim, **matrix;
	dim = spPointGetDimension(arr[0]);
	matrix = (int**) malloc (sizeof(int*)*dim*sizeof(int)*size);
	if(matrix == NULL) { //Allocation Fails
		return NULL;
	}
	i = 0;
	j = 0;
	SPPoint* sortedPointArray = copySPPointArray(arr,  size);

	for (i = 0; i < dim; i++){
		// get a array of sorted points by the first dim
		int (*compar)(const void *, const void*) = comperatorCreator(i);
		qsort(sortedPointArray, size, sizeof(SPPoint), compar);
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

int** buidTwoMaps (int* sortedPoints, SPPoint* arr){
	int len = length (sortedPoints);
	int i =0;
	int half;
	int cnt1=0;
	int cnt2 =0;
	int** maps = (int**) malloc (length (sortedPoints)*2);
	if(maps == NULL) { //Allocation Fail
		return NULL;
	}
	if (len%2 == 0){
		half = len/2;
	}
	else{
		half = floor(len/2)+1;
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

SPPoint** splitPointArray (int** maps, SPPoint* array){
	struct SPPoint** pointArrays = (SPPoint**) malloc (length (array)*2);
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

int*** splitMatrixes (int **maps, int **matrix){
	int*** matrixArrays = (int***) malloc(sizeof(matrix));
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

void SPKDArrayDestroy(SPKDArray kdArr){
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
