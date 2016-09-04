/*
 * KDArray_unittest.c
 *
 *  Created on: 23 баев 2016
 *      Author: рвд
 */

#include "unit_test_util.h"
#include "KDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>




static bool testInit(){
	double d [1] ={0.0};
	SPPoint* point = (SPPoint*)malloc (3*sizeof(SPPoint));
	point [0]= spPointCreate (d,1,0);
	 SPKDArray arr = Init(point,1);
	// normal inputs

	ASSERT_TRUE(arr!=NULL);
	ASSERT_TRUE(spPointGetDimension(getPoint(arr)[0]) == 1);
	ASSERT_TRUE(spPointGetIndex(getPoint(arr)[0]) == 0);
	ASSERT_TRUE(spPointGetAxisCoor((getPoint(arr)[0]),0) == 0);

	SPKDArrayDestroy(arr,1);

	return true;
}

static bool testSplit(){
	double d [2]  ={0.0, 10.0};
	SPPoint* points = (SPPoint*)malloc (3*sizeof(SPPoint));
	points [0]= spPointCreate (d,3,0);
	double d1 [2] = {1.0, 9.0};
	points [1] = spPointCreate (d1, 3, 1);
	double d2 [2] = {2.0, 8.0};
	points [2] = spPointCreate (d2, 3,2);
	 SPKDArray arr = Init(points,3);

//	 SPKDArray** splitted = Split(arr,0);
//	ASSERT_TRUE(splitted !=NULL);
//	ASSERT_TRUE(spPointGetIndex(getPoint(*splitted[0])[0]) ==0);
//	ASSERT_TRUE(spPointGetIndex(getPoint(*splitted[0])[1]) ==1);
//	ASSERT_TRUE(spPointGetIndex(getPoint(*splitted[1])[0]) ==2);
//
//	splitted = Split(arr,1);
//	ASSERT_TRUE(splitted !=NULL);
//	ASSERT_TRUE(spPointGetIndex(getPoint(*splitted[0])[0]) ==2);
//	ASSERT_TRUE(spPointGetIndex(getPoint(*splitted[0])[1]) ==1);
//	ASSERT_TRUE(spPointGetIndex(getPoint(*splitted[1])[0]) ==0);



	SPKDArrayDestroy(arr,3);
	free (points);
	return true;
}

static bool testCopySPPointArray(){
	double d [2]  ={0.0, 10.0};
	SPPoint* points = (SPPoint*)malloc (3*sizeof(SPPoint));
	points [0]= spPointCreate (d,3,0);
	double d1 [2] = {1.0, 9.0};
	points [1] = spPointCreate (d1,3,1);
	double d2 [2] = {2.0, 8.0};
	points [2] = spPointCreate (d2,3,2);

	SPPoint* copyPoints = copySPPointArray (points, 3);


	ASSERT_TRUE(spPointGetIndex(copyPoints[0]) ==0);
	ASSERT_TRUE(spPointGetIndex(copyPoints[1]) ==1);
	ASSERT_TRUE(spPointGetIndex(copyPoints[2]) ==2);
	ASSERT_TRUE(spPointGetAxisCoor(copyPoints[0],0) ==0.0);
	ASSERT_TRUE(spPointGetAxisCoor(copyPoints[0],1) ==10.0);

	int i;
	for (i=0;i<3;i++){
		spPointDestroy(copyPoints[i]);
		spPointDestroy(points[i]);
	}
	free (copyPoints);
	free (points);

	return true;
}


static bool testCreateMatrix(){
	double d [2]  ={0.0, 10.0};
	SPPoint* points = (SPPoint*)malloc (3*sizeof(SPPoint));
	points [0]= spPointCreate (d,2,0);
	double d1 [2] = {1.0, 9.0};
	points [1] = spPointCreate (d1,2,1);
	double d2 [2] = {2.0, 8.0};
	points [2] = spPointCreate (d2,2,2);
	int ** matrix = createMatrix(points, 3);


	ASSERT_TRUE (matrix [0][0]== 0);
	ASSERT_TRUE (matrix [0][1]== 1);
	ASSERT_TRUE (matrix [0][2]== 2);
	ASSERT_TRUE (matrix [1][0]== 2);
	ASSERT_TRUE (matrix [1][1]== 1);
	ASSERT_TRUE (matrix [1][2]== 0);




	int i=0;
	for (i=0;i<3;i++){

		spPointDestroy(points[i]);
	}

	free (points);


	return true;
}



static bool testBuildTwoMaps(){
	int arr [3]  ={0,2,1};
	int** twoMaps = buildTwoMaps(arr,3);
//	printf ("%d   ", twoMaps[0][0]);
	//printf ("ppp - %d", twoMaps[0][1]);
////	printf ("%d", twoMaps[0][2]);
//	printf ("%d", twoMaps[1][0]);
//	printf ("%d", twoMaps[1][1]);
//	printf ("%d", twoMaps[1][2]);

	ASSERT_TRUE (twoMaps[0][0]== 0);
	ASSERT_TRUE (twoMaps[0][1]== -1);
	ASSERT_TRUE (twoMaps[0][2]== 1);
	ASSERT_TRUE (twoMaps[1][0]==-1);
	ASSERT_TRUE (twoMaps[1][1]==0);
	ASSERT_TRUE (twoMaps[1][2]==-1);

	free (twoMaps[0]);
	free (twoMaps[1]);

	return true;
}

static bool testSplitPointArray(){
	int** twoMaps;
	int i=0;
	twoMaps = (int**)malloc(2 * sizeof(int*));
    for (i=0; i<2; i++)
    {
    	twoMaps[i] = (int*)malloc(3 * sizeof(int));
    }
    int j =0;
    i=0;
    for (i=0; i<2; i++){
    	for (j=0;j<3;j++){
    		if(i==j){
    			twoMaps[i][j]=0;
    		}
    		else{
    			if(i==0 && j==2){
    				twoMaps[i][j]=1;
    			}
    			else{
    				twoMaps[i][j]=-1;
    			}
    		}
    	}
    }

	double d [2]  ={0.0, 10.0};
	SPPoint* points = (SPPoint*)malloc (3*sizeof(SPPoint));
	points [0]= spPointCreate (d,2,0);
	double d1 [2] = {1.0, 9.0};
	points [1] = spPointCreate (d1,2,1);
	double d2 [2] = {2.0, 8.0};
	points [2] = spPointCreate (d2,2,2);

	 SPPoint** splitedPointArray = splitPointArray (twoMaps,points,3);
	 ASSERT_TRUE (splitedPointArray != NULL);
	// printf ("%d",spPointGetIndex(splitedPointArray[0][1]));
	 ASSERT_TRUE (spPointGetIndex(splitedPointArray[0][0])== 0);
	 ASSERT_TRUE (spPointGetIndex(splitedPointArray[0][1])== 2);
	 ASSERT_TRUE (spPointGetIndex(splitedPointArray[1][0])== 1);



	return true;
}

static bool testSplitMatrixes(){
	int** twoMaps;
	int i=0;
	twoMaps = (int**)malloc(2 * sizeof(int*));
    for (i=0; i<2; i++)
    {
    	twoMaps[i] = (int*)malloc(3 * sizeof(int));
    }
    int j =0;
    i=0;
    for (i=0; i<2; i++){
    	for (j=0;j<3;j++){
    		if(i==j){
    			twoMaps[i][j]=0;
    		}
    		else{
    			if(i==0 && j==2){
    				twoMaps[i][j]=1;
    			}
    			else{
    				twoMaps[i][j]=-1;
    			}
    		}
    	}
    }
	int** matrix;
	 i=0;
	 matrix = (int**)malloc(3 * sizeof(int*));
    for (i=0; i<3; i++)
    {
    	matrix[i] = (int*)malloc(sizeof(int));
    }

    i=0;
    for (i=0; i<3; i++){
    	if(i==0){
    		matrix[i][0]=1;
    	}
    	else{
    		if(i==1){
    			matrix[i][0]=2;
    		}
    		else{
    			matrix[i][0]=3;
    		}

    	}
    }



	int*** splitedMatrix = splitMatrixes (twoMaps,matrix,3,1);

	 ASSERT_TRUE (splitedMatrix != NULL);
	 ASSERT_TRUE (splitedMatrix[0][0][0] == 1);
	 ASSERT_TRUE (splitedMatrix[0][1][0] == 3);
	 ASSERT_TRUE (splitedMatrix[1][0][0] == 2);

	return true;
}

static bool testSPKDArrayDestroy(){
	double d [1] ={0.0};
	SPPoint* point = (SPPoint*)malloc (3*sizeof(SPPoint));
	point [0]= spPointCreate (d,1,0);
	SPKDArray arr = Init(point,1);
	SPKDArrayDestroy(arr,1);
	ASSERT_TRUE (arr == NULL);


	return true;
}


int main( int argc, const char* argv[] )
{
	testInit(); //ok
	testSplit();
	testCopySPPointArray(); //ok
	testCreateMatrix();  //ok
	testBuildTwoMaps(); //ok
	testSplitPointArray();//ok
	testSplitMatrixes(); //ok
	//testSPKDArrayDestroy(); //weird problem


}




