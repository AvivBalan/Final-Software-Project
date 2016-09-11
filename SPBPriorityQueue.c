#include "SPBPriorityQueue.h"
#include "SPList.h"
#include "SPListElement.h"
#include <stdbool.h>
#include <stdlib.h>


struct sp_bp_queue_t {
	SPList list ;
	int maxSize;

};


//Allocates a new SPBPQueue.

SPBPQueue spBPQueueCreate(int maxSize){

	if (maxSize <1) {
		return NULL;
	}
	SPBPQueue newSPBPQueue = (SPBPQueue) malloc(sizeof(*newSPBPQueue));
	if (newSPBPQueue == NULL) {
		return NULL;
	}
	newSPBPQueue->maxSize = maxSize;
	SPList list = spListCreate ();
	if (list == NULL) {
		free (newSPBPQueue);
		return NULL;
	}
	newSPBPQueue->list = list;

	return newSPBPQueue;
}

//Creates a copy of target SPBPQueue.

SPBPQueue spBPQueueCopy(SPBPQueue source){

	if (source == NULL) {
		return NULL;
	}
	SPBPQueue copy = spBPQueueCreate(source->maxSize);
	if (copy == NULL){
		spBPQueueDestroy (copy);
		return NULL;
	}
	SPList copylst = spListCopy(source->list);
	spListDestroy (copy->list);
	copy -> list = copylst;
	copy->maxSize = source->maxSize;
	//free (copylst);

	return copy;
}
//Deallocates an existing SPBPQueue. Clears all its elements
void spBPQueueDestroy(SPBPQueue source){
	if (source == NULL){
		return;
	}
	spListDestroy (source->list);

	free (source);

}
//Removes all elements from target SPBPQueue.
void spBPQueueClear(SPBPQueue source){
	if (source == NULL){
		return;
	}
	spListClear (source->list);
}
//returns the number of elements in a SPBPQueue.
int spBPQueueSize(SPBPQueue source){
	if (source == NULL){
		return -1;
	}

	return spListGetSize (source->list);
}
//Returns the max size of the SPBPQueue.
int spBPQueueGetMaxSize(SPBPQueue source){
	if (source == NULL){
		return -1;
	}

	return source->maxSize;
}
//Adds a new element to the SPBPQueue, the new element will be the last element
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	if (source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	if (spBPQueueSize(source) == source->maxSize){
		return SP_BPQUEUE_FULL;
	}
	if (element == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	SPListElement copyElement = spListElementCopy(element);

	SP_LIST_MSG msg;
	msg=spListInsertLast(source->list, copyElement);

	if (msg == SP_LIST_OUT_OF_MEMORY){
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}
	spListElementDestroy (copyElement);
	return SP_BPQUEUE_SUCCESS;
}

//Removes the max value element of the SPBPQueue
SP_BPQUEUE_MSG spBPQueueMaxDequeue(SPBPQueue source){
	if (source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	int i =0;
	if (spBPQueueSize(source) == 0){
		return SP_BPQUEUE_EMPTY;
	}
	SPList sourceList = source->list;
	double max= spListElementGetValue(spListGetFirst(sourceList));

	for (i=1; i<spBPQueueSize(source);i++){
		SPListElement temp =spListGetNext(sourceList);
		if (spListElementGetValue(temp)> max){
			max = spListElementGetValue(temp);
		}
	}
	i=0;
	spListGetFirst(sourceList);
	for (i=0; i<spBPQueueSize(source); i++){
		double nn = spListElementGetValue(spListGetCurrent(sourceList));
		if (nn == max){
			spListRemoveCurrent(sourceList);
			break;
		}
		spListGetNext(sourceList);
	}
	return SP_BPQUEUE_SUCCESS;
}

//Removes the min value element of the SPBPQueue
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	if (source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	int i =0;
	if (spBPQueueSize(source) == 0){
		return SP_BPQUEUE_EMPTY;
	}
	SPList sourceList = source->list;
	double min= spListElementGetValue(spListGetFirst(sourceList));

	for (i=1; i<spBPQueueSize(source);i++){
		SPListElement temp =spListGetNext(sourceList);
		if (spListElementGetValue(temp)< min){
			min = spListElementGetValue(temp);
		}
	}
	i=0;
	spListGetFirst(sourceList);
	for (i=0; i<spBPQueueSize(source); i++){
		double nn = spListElementGetValue(spListGetCurrent(sourceList));
		if (nn == min){
			spListRemoveCurrent(sourceList);
			break;
		}
		spListGetNext(sourceList);
	}

	return SP_BPQUEUE_SUCCESS;
}
//Returns copy of the element with the min value
SPListElement spBPQueuePeek(SPBPQueue source){
	if (source == NULL){
		return NULL;
	}
	if (spBPQueueSize(source) == 0){
		return NULL;
	}
	int i =0;
	SPList sourceList = source->list;
	SPListElement minElement = spListGetFirst(sourceList);
	for (i=0;i<spBPQueueSize(source); i++){

		SPListElement tempElement = spListGetNext(sourceList);
		if (spListElementGetValue(tempElement) < spListElementGetValue(minElement)){
			spListElementSetIndex(minElement,spListElementGetIndex(tempElement));
			spListElementSetValue(minElement,spListElementGetValue(tempElement));
		}
	}
	SPListElement finalelement = spListElementCopy (minElement);
	return finalelement;
}

//Returns copy of the element with the max value
SPListElement spBPQueuePeekLast(SPBPQueue source){
	if (source == NULL){
		return NULL;
	}
	if (spBPQueueSize(source) == 0){
		return NULL;
	}
	int i =0;
	SPList sourceList = source->list;
	SPListElement maxElement = spListGetFirst(sourceList);
	for (i=0;i<spBPQueueSize(source); i++){

		SPListElement tempElement = spListGetNext(sourceList);
		if (spListElementGetValue(tempElement) > spListElementGetValue(maxElement)){
			maxElement = tempElement;
		}
	}
	SPListElement finalelement = spListElementCopy (maxElement);
	return finalelement;
}
//Returns the value of the element with the min value
double spBPQueueMinValue(SPBPQueue source){
	if (source == NULL){

		return -1.0;
	}
	if (spBPQueueSize(source) == 0){

		return -1.0;
	}
	int i =0;
	SPList sourceList = source->list;
	SPListElement minElement = spListGetFirst(sourceList);
	for (i=1;i<spBPQueueSize(source); i++){
		SPListElement tempElement = spListGetNext(sourceList);
		if (spListElementGetValue(tempElement) < spListElementGetValue(minElement)){
			minElement = tempElement;
		}
	}

	double finalValue = spListElementGetValue(minElement);
	return finalValue;
}
//Returns the value of the element with the max value
double spBPQueueMaxValue(SPBPQueue source){
	if (source == NULL){
		return -1.0;
	}
	if (spBPQueueSize(source) == 0){
		return -1.0;
	}
	int i =0;
	SPList sourceList = source->list;
	SPListElement maxElement = spListGetFirst(sourceList);
	for (i=0;i<spBPQueueSize(source); i++){
		SPListElement tempElement = spListGetNext(sourceList);
		if (spListElementGetValue(tempElement) > spListElementGetValue(maxElement)){
			maxElement = tempElement;
		}
	}
	double finalValue = spListElementGetValue(maxElement);
	return finalValue;
}
//returns true if the given SPBPQueue is empty
bool spBPQueueIsEmpty(SPBPQueue source){
	if (source == NULL){
		return true;
	}
	if (spBPQueueSize(source)==0){
		return true;
	}
	return false;
}
//returns true if the given SPBPQueue is full.
bool spBPQueueIsFull(SPBPQueue source){
	if (source == NULL){
		return true;
	}
	if (spBPQueueSize(source) == source->maxSize){
		return true;
	}
	return false;
}
