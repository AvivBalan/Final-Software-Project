/*
 * KDArray.h
 *
 *  Created on: 18 баев 2016
 *      Author: рвд
 */
#include "SPPoint.h"


/**
 * A data-structure which is used for holding the KDArray. it's fields is:
 *  SPPoint pointer named "p"
 *  Integer pointer of pointer named "matrix"
 */

typedef struct sp_kd_Array* SPKDArray;

/**
 * Creates a new KDArray struct.
 * is initialized based on the variables given by  SPPoint pointer 'arr' and it's size 'size'.
 *
 * @param arr -  pointer of SPPoint.
 * @param size - the number of the elements in 'arr'.
 * @return NULL in case an error occurs. Otherwise, struct which
 * 		   contains the given SPPoint pointer and matrix builds respectively.
 *
 */

SPKDArray Init( SPPoint* arr, int size);
/*
 * Returns the pointer of SPPoint of the given KDArray i.e the value
 * of 'p'.
 *
 * @param arr - the KDArray structure
 * @return pointer of SPPoint in success, NULL otherwise.
 *
 */
SPPoint* getPoint ( SPKDArray arr);


/*
 * Returns the pointer of pointer of Integer of the given KDArray i.e the value
 * of 'matrix'.
 *
 * @param arr - the KDArray structure
 * @return pointer of pointer of Integer in success, NULL otherwise.
 */
int** getMatrix ( SPKDArray arr);


/*
 * Returns the pointer to two pointers of SPKDArray structure, each contains the
 * lower/higher half of the SPPoint elements, sorted by the coor's Dimension and it's proper matrix
 *  the first new SPKDArray structure will contain the lower half of SPPoint elements.
 * if the length of the SPPoint array is odd, then the first new SPKDArray structure will have one more
 * SPPoint element then the second one.
 *
 * @param kdArr - the KDArray structure
 * @param coor - the integer of the Dimension the SPPoints will be sorted by.
 * @param len - the length of the SPPoint array of kdArr
 * @return   pointer to two pointers of SPKDArray structure in success, NULL otherwise.
 */
SPKDArray**  Split( SPKDArray kdArr, int coor, int len);


/*
 * Returns SPPoint array which contains copy of the elements in the variable 'arr'
 * in the same order.
 *
 * @param arr -  the SPPoint array
 * @param size - the length of arr
 * @return SPPoint array in success, NULL otherwise.
 */

SPPoint* copySPPointArray ( SPPoint* arr, int size);

/*
 * return the result of the comparsion of the gobalDims' Dimension
 * between SPPoint a and  SPPoint b.
 *
 * @param a - pointer to const SPPoint
 * @param a - pointer to const SPPoint
 * @param gobalDims - integer of the Dimension of SPPoint to be compared
 * @return  1 - if gobalDims' Dimension of a > gobalDims' Dimension of b
 *  -1 - if gobalDims' Dimension of a < gobalDims' Dimension of b
 *   0 - if gobalDims' Dimension of a > gobalDims' Dimension of b
 */

int compare (const SPPoint *a, const SPPoint *b);

/*
 * Returns two Dimensional array of Integers. with the size of the Dimension of SPPoints in
 * the variable 'arr' X the variable 'size'.
 * each "line" in the 2d matrix will contain the indexes of 'arr' sorted
 * by the line's number Dimension. i.e the matrix will contain all the Dimensional sorting
 * options.
 *
 * @param arr - the SPPoints array.
 * @param size - the length of the arr.
 * @param compare - comparsion function between two SPPoint pointers
 * @return   Returns two Dimensional array of Integers in success, NULL otherwise.
 */


int** createMatrix ( SPPoint* arr, int size);

/*
 * Returns two arrays of Integers. with the size of the variable 'len'.
 * the first array will contain in the first half of the Integers in 'sortedPoints'
 * cells number, Integers in  rising order, from 0. and in the other cells there
 * will be -1.
 * the second array will contain the opposite from the first one.
 * if 'len' is odd then the first array will have one more positive Integer
 * then the second one.
 *
 * @param sortedPoints - Integer array, it's integers go from 0 to 'len', each appears once.
 * @param len - the length of 'sortedPoints'.
 * @return   pointer to two pointers of Integers in success, NULL otherwise.
 */

int** buildTwoMaps (int* sortedPoints, int len);


/*
 * Returns two arrays of SPPoint structure, each contains the SPPoint elements
 * splitted by the order of the variable 'maps'. the first array will contain the
 * the elements of 'array', if in the element's index, there is non-negative integer in the
 * first array of 'maps', that element will be in the new array.
 * the second array will be made the same way regard to the second array of 'maps'.
 *
 * @param maps - two Integers arrays, it's integers go from 0 to 'len', each appears once. or -1.
 * @param array - SPPoint array.
 * @param len - the length of 'array'.
 * @return   two arrays of SPPoint structure in success, NULL otherwise.
 */
SPPoint** splitPointArray (int**maps ,  SPPoint* array, int len);


/*
 * Returns two arrays of two Dim arrays, each contains the elements
 * splitted by the order of the variable 'maps'. the first array will contain the
 * the elements of 'array', if in the element's index, there is non-negative integer in the
 * first array of 'maps', that element will be in the new array.
 * the second array will be made the same way regard to the second array of 'maps'.
 *
 * @param maps - two Integers arrays, it's integers go from 0 to 'len', each appears once. or -1.
 * @param matrix - the two Dim array.
 * @param len - the length of the second Dim of 'matrix'
 * @param dim - the length of the first Dim of 'matrix'
 * @return   two arrays of two Dim arrays in success, NULL otherwise.
 */
int*** splitMatrixes (int**maps, int**matrix, int len, int dim);

/**
 * Frees all memory resources associate with kdArr.
 *  @param kdArr - the KDArray structure
 *  @param len - the length of the SPPoint array of kdArr
 * If kdArr == NULL nothig is done.
 */

void SPKDArrayDestroy (SPKDArray kdArr,int len);


