#include "SPPoint.h"

typedef struct sp_kd_array_t* SPKDArray;

SPKDArray Init(SPPoint* arr, int size);

SPKDArray**  Split(SPKDArray kdArr, int coor);

SPPoint* copySPPointArray (SPPoint*);

int** createMatrix (SPPoint* arr, int size);

float (*comperatorCreator(int dim))(const void *, const void*);
