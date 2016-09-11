#include "SPPoint.h"

/*
 * A data-structure which is used for holding the KDArray. it's fields is:
 *  SPPoint pointer named "p"
 *  Integer pointer of pointer named "matrix"
 */
typedef struct sp_kd_Array* SPKDArray;

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
 * Creates a new KDArray struct.
 * is initialized based on the variables given by  SPPoint pointer 'array' and it's size 'arraySize'.
 *
 * @param array -  pointer of SPPoint.
 * @param arraySize - the number of the elements in 'array'.
 * @return NULL in case an error occurs. Otherwise, struct which
 * 		   contains the given SPPoint pointer and matrix builds respectively.
 *
 */
SPKDArray spKDArrayCreate(SPPoint* array, int arraySize);

/*
 * Returns the pointer to two pointers of SPKDArray structure, each contains the
 * lower/higher half of the SPPoint elements, sorted by the coor's Dimension and it's proper matrix
 *  the first new SPKDArray structure will contain the lower half of SPPoint elements.
 * if the length of the SPPoint array is odd, then the first new SPKDArray structure will have one more
 * SPPoint element then the second one.
 *
 * @param kdArr - the KDArray structure
 * @param dim - the integer of the Dimension the SPPoints will be sorted by.
 * @return   pointer to two pointers of SPKDArray structure in success, NULL otherwise.
 */
SPKDArray*  spKDArraySplit( SPKDArray kdArr, int dim);

/*
 * Returns SPPoint array which contains copy of the elements in the variable 'arr'
 * in the same order.
 *
 * @param arr -  the SPPoint array
 * @param arraySize - the length of arr
 * @return SPPoint array in success, NULL otherwise.
 */
SPPoint* spKDArrayCopyPointArray ( SPPoint* arr, int arraySize);

/*
 * return the result of the comparsion of the gobalDims' Dimension
 * between index 'a' of a SPPoint and  index 'b' of a SPPoint.
 *
 * @param a - pointer to const int
 * @param a - pointer to const int
 * @param globalDim - integer of the Dimension of SPPoint to be compared
 * @param globalPointArray - array of the point which each index represent
 * @return  1 - if gobalDims' Dimension of a > gobalDims' Dimension of b
 *  -1 - if gobalDims' Dimension of a < gobalDims' Dimension of b
 *   0 - if gobalDims' Dimension of a > gobalDims' Dimension of b
 */
int spKDArrayComparePoints (const void *a, const void *b);

/*
 * Returns two Dimensional array of Integers. with the size of the Dimension of SPPoints in
 * the variable 'arr' X the variable 'size'.
 * each "line" in the 2d matrix will contain the indexes of 'arr' sorted
 * by the line's number Dimension. i.e the matrix will contain all the Dimensional sorting
 * options.
 *
 * @param arr - the SPPoints array.
 * @param arraySize - the length of the arr.
 * @return   Returns two Dimensional array of Integers in success, NULL otherwise.
 */
int** spKDArrayCreateMatrix ( SPPoint* arr, int arraySize);

/*
 * Returns two arrays of Integers. with the size of the variable 'arraySize'.
 * the first array will contain in the first half of the Integers in 'sortedPoints'
 * cells number, Integers in  rising order, from 0. and in the other cells there
 * will be -1.
 * the second array will contain the opposite from the first one.
 * if 'arraySize' is odd then the first array will have one more positive Integer
 * then the second one.
 *
 * @param sortedPointIndexes - Integer array of the indexes of the features, which is sorted by a certain dimension.
 * @param arraySize - the length of 'sortedPoints'.
 * @return   pointer to two pointers of Integers in success, NULL otherwise.
 */
int** spKDArrayBuildMaps (int* sortedPointIndexes, int arraySize);

/*
 * Returns two arrays of SPPoint structure, each contains the SPPoint elements
 * splitted by the order of the variable 'maps'. the first array will contain the
 * the elements of 'array', if in the element's index, there is non-negative integer in the
 * first array of 'maps', that element will be in the new array.
 * the second array will be made the same way regard to the second array of 'maps'.
 *
 * @param maps - two Integers arrays, it's integers go from 0 to 'arraySize', each appears once. or -1.
 * @param array - SPPoint array.
 * @param arraySize - the length of 'array'.
 * @return   two arrays of SPPoint structure in success, NULL otherwise.
 */
SPPoint** spKDArraySplitPointArray (int**maps ,SPPoint* array, int arraySize);

/*
 * Returns two arrays of two Dim arrays, each contains the elements
 * splitted by the order of the variable 'maps'. the first array will contain the
 * the elements of 'array', if in the element's index, there is non-negative integer in the
 * first array of 'maps', that element will be in the new array.
 * the second array will be made the same way regard to the second array of 'maps'.
 *
 * @param maps - two Integers arrays, it's integers go from 0 to 'numOfPoints', each appears once. or -1.
 * @param matrix - the two Dim array.
 * @param numOfPoints - the length of the second dimension of 'matrix'
 * @param numOfDimensions - the length of the first dimension of 'matrix'
 * @return   two arrays of two dimensional arrays in success, NULL otherwise.
 */
int*** spKDArraySplitMatrixes (int** maps, int** matrix, int numOfPoints, int numOfDimensions);

/*
 * Frees all memory resources associate with kdArr.
 *  @param kdArr - the KDArray structure
 * If kdArr == NULL nothig is done.
 */
void spKDArrayDestroy (SPKDArray kdArr);

double spKDArrayGetMedianValue(SPKDArray kdArr, int dim);

SPPoint spKDArrayGetPoint(SPKDArray kdArr, int index);

int spKDArrayFindMaxSpreadDim(SPKDArray kdArr);
