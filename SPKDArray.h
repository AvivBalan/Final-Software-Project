#include "SPPoint.h"
#include <stdlib.h>
#include <math.h>

typedef struct sp_kd_array_t* SPKDArray;

SPKDArray Init(SPPoint* arr, int size);

SPKDArray**  Split(SPKDArray kdArr, int coor);

SPPoint* copySPPointArray (SPPoint* Array, int size);

int** createMatrix (SPPoint* arr, int size);

int (*comperatorCreator(int dim))(const void *, const void*);

int*** splitMatrixes (int **maps, int **matrix);

void SPKDArrayDestroy(SPKDArray kdArr);
