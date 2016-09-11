/*
 * SPBPriorityQueue.h
 *
 *  Created on: 31 במאי 2016
 *      Author: user
 */

#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 *
 * Implements a list container type.
 * The elements of the list are of type SPListElement, please refer
 * to SPListElement.h for usage.
 * The list has an internal iterator for external use. For all functions
 * where the state of the iterator after calling that function is not stated,
 * the state of the iterator is undefined. That is you cannot assume anything about it.
 *
 * The following functions are available:
 *
 *   spBPQueueCreate            - Creates a new empty SPBPQueue with a given max size
 *   spBPQueueDestroy           - Deletes an existing SPBPQueue and frees all resources
 *   spBPQueueCopy              - Copies an existing SPBPQueue
 *   spBPQueueSize              - Returns the size of a given SPBPQueue
 *   spBPQueueClear		        - Clears all the data from the SPBPQueue
 *   spBPQueueGetMaxSize        - Returns the max size of a given SPBPQueue
 *   spBPQueueEnqueue           - Inserts a copy of given element in the end of the SPBPQueue                               internal iterator
 *   spBPQueueDequeue           - Removes the min value element from given SPBPQueue
 *   spBPQueuePeek              - Return copy of the element with the min value from given SPBPQueue
 *   spBPQueuePeekLast          - Return copy of the element with the max value from given SPBPQueue
 *   spBPQueueMinValue          - Return the value of the element with the min value from given SPBPQueue
 *   spBPQueueMaxValue          - Return the value of the element with the max value from given SPBPQueue
 *   spBPQueueIsEmpty           - returns true if the given SPBPQueue is empty
 *   spBPQueueIsFull            - returns true if the given SPBPQueue is full
 *
 */

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 *
 * Allocates a new SPBPQueue.
 *
 * This function creates a new empty SPBPQueue with the given max size.
 * @return
 * 	NULL - If allocations failed or max size <1
 * 	A new SPBPQueue in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 *
 * Creates a copy of target SPBPQueue.
 *
 * The new copy will contain all the elements from the source SPBPQueue in the same
 * order and the same max size.
 * @param SPBPQueue The target SPBPQueue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A SPBPQueue containing the same elements with same order and the same max size as the source SPBPQueue otherwise.
 */

SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Deallocates an existing SPBPQueue. Clears all elements by using the
 * spListDestroy and the function free().
 *
 * @param SPBPQueue Target list to be deallocated. If SPBPQueue is NULL nothing will be
 * done
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target SPBPQueue.
 *
 * The elements are deallocated using the function spListClear
 * @param SPBPQueue Target SPBPQueue to remove all element from
 * If SPBPQueue is NULL nothing will be
 * done
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in a SPBPQueue.
 * @param SPBPQueue The target SPBPQueue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the SPBPQueue using the function spListGetSize()
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Returns the max size of the SPBPQueue.
 * @param SPBPQueue The target SPBPQueue which max size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the max size SPBPQueue
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a new element to the SPBPQueue, the new element will be the last element.
 *
 * @param SPBPQueue The SPBPQueue for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as source or element
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 * SP_BPQUEUE_FULL if the list is full
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the min value element of the SPBPQueue using the function spListRemoveCurrent.
 *
 * @param source The SPBPQueue for which the min value element will be removed
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if source is NULL
 * SP_BPQUEUE_EMPTY if the source is empty
 * SP_BPQUEUE_SUCCESS the current element was removed successfully
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Removes the max value element of the SPBPQueue using the function spListRemoveCurrent.
 *
 * @param source The SPBPQueue for which the max value element will be removed
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if source is NULL
 * SP_BPQUEUE_EMPTY if the source is empty
 * SP_BPQUEUE_SUCCESS the current element was removed successfully
 */
SP_BPQUEUE_MSG spBPQueueMaxDequeue(SPBPQueue source);

/**
 * Returns copy of the element with the min value
 *
 * @param source The SPBPQueue for which to get the min value element
 * @return
 * NULL if the source is NULL of with size of 0
 * Otherwise copy of the element with the min value
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns copy of the element with the max value
 *
 * @param source The SPBPQueue for which to get the max value element
 * @return
 * NULL if the source is NULL of with size of 0
 * Otherwise copy of the element with the max value
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the value of the element with the min value
 *
 * @param source The SPBPQueue for which to get the min value
 * @return
 * -1 if the source is NULL of with size of 0
 * Otherwise value of the element with the min value
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the value of the element with the max value
 *
 * @param source The SPBPQueue for which to get the max value
 * @return
 * -1 if the source is NULL of with size of 0
 * Otherwise value of the element with the max value
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * returns true if the given SPBPQueue is empty.
 * @param source The SPBPQueue for which to check if is empty
 * @return
 * true If source is Null or of size 0
 * Otherwise false
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * returns true if the given SPBPQueue is full.
 * @param source The SPBPQueue for which to check if is full
 * @return
 * true If source is Null or if size (source) = max size
 * Otherwise false
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif

