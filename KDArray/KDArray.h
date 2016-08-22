/*
 * KDArray.h
 *
 *  Created on: 18 ���� 2016
 *      Author: ���
 */
#include "SPPoint.h"

typedef struct SPKDArray;

struct SPKDArray Init(struct SPPoint* arr, int size);

struct SPKDArray**  Split(struct SPKDArray kdArr, int coor);

struct SPPoint* copySPPointArray (struct SPPoint*);

int** createMatrix (struct SPPoint* arr, int size);

float (*comperatorCreator(int dim))(const void *, const void*);
