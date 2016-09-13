#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct sp_point_t{
	double* data;
	int dim;
	int index;
};

//Create a new point.
SPPoint spPointCreate(double* data, int dim, int index){

	SPPoint newPoint = NULL; //initializing a new point

	if(data == NULL || index < 0 || dim <= 0){ //in case one of the arguments is invalid
		return NULL;
	}
	newPoint = (SPPoint) malloc(sizeof(*newPoint)); //allocating memory for the new point
	if (newPoint == NULL) { //Allocation Fails
		return NULL;
	}

	newPoint->data = (double*) malloc(sizeof(double)*dim); //allocating memory for the data of the new point
	if (newPoint->data == NULL) { //Allocation Fails
		free(newPoint);
		return NULL;
	}

   memcpy(newPoint->data, data, sizeof(double)*dim); //copy the given data to the data assigned for the new point

	newPoint->dim = dim;
	newPoint->index = index;

	return newPoint;
}

//Creates a copy of the given point.
SPPoint spPointCopy(SPPoint source){

	assert(source != NULL); //in case the given point is null
	SPPoint copy = spPointCreate(source->data, source->dim, source->index); //creates a copy the given point

	return copy;

}

//Frees all memory allocated for the point.
void spPointDestroy(SPPoint point){

	if(point != NULL) {
		free(point->data); //frees the memory allocated for the data of the point
		free(point);
	}
}

//Returns the dimension of the point.
int spPointGetDimension(SPPoint point){

	assert(point != NULL); //in case the given point is null
	return point->dim;
}

//Returns the index of the point.
int spPointGetIndex(SPPoint point){

	assert(point != NULL); //in case the given point is null
	return point->index;
}

//Return the coordinate value of the given axis.
double spPointGetAxisCoor(SPPoint point, int axis){

	assert(point!=NULL && axis < point->dim); //in case the given point is null or axis is invalid
	return point->data[axis];

}

//Calculates the L2 squared distance between two given points.
double spPointL2SquaredDistance(SPPoint p, SPPoint q){

	double l2sd = 0; // initializing the distance
	int i = 0;

	assert(p!=NULL && q!=NULL && p->dim == q->dim); // in case one of the points is null or their dimensions dont fit

	for (i = 0; i < p->dim; ++i) { // for each axis calculates the distance and adds to the sum
		double temp =  p->data[i] - q->data[i];
		l2sd += temp*temp;
	}

	return l2sd;
}
