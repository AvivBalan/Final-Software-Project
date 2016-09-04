/*
 * KDArray.h
 *
 *  Created on: 18 баев 2016
 *      Author: рвд
 */
#include "SPPoint.h"

typedef struct sp_kd_Array* SPKDArray;


SPPoint* getPoint ( SPKDArray arr);

int** getMatrix ( SPKDArray arr);

 SPKDArray Init( SPPoint* arr, int size);

 SPKDArray**  Split( SPKDArray kdArr, int coor);

SPPoint* copySPPointArray ( SPPoint* arr, int size);

int compare (const SPPoint *a, const SPPoint *b);

int** createMatrix ( SPPoint* arr, int size);

int** buildTwoMaps (int* sortedPoints);

SPPoint** splitPointArray (int**maps ,  SPPoint* array);

int*** splitMatrixes (int**maps, int**matrix);

void SPKDArrayDestroy (SPKDArray kdArr);


