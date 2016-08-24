/*
 * KDArray_unittest.c
 *
 *  Created on: 23 баев 2016
 *      Author: рвд
 */
#include "unit_test_util.h"
#include "KDArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static bool testInit(){
	double d  =0.0;
	struct SPPoint point = spPointCreate (&d, 0,0);
	struct SPKDArray arr = Init(&point,1);
	// normal inputs

	ASSERT_TRUE(arr!=NULL);
	ASSERT_TRUE(spPointGetDimension(getPoint(arr)) == 0);
	ASSERT_TRUE(spPointGetIndex(getPoint(arr)) == 0);
	ASSERT_TRUE(spPointGetAxisCoor(getPoint(arr),0) == 0);

	SPKDArrayDestroy(arr);

	return true;
}

static bool testSplit(){

}

static bool testCopySPPointArray(){

}

static bool testCopySPPointArray(){

}

static bool testCreateMatrix(){

}

static bool testComperatorCreator(){

}

static bool testBuildTwoMaps(){

}

static bool testSplitPointArray(){

}

static bool testSplitMatrixes(){

}

static bool testSPKDArrayDestroy(){

}







