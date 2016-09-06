/*
 * main.cpp

 *
 *  Created on: 5 בספט 2016
 *      Author: נגה
 */

#include <cstdlib> //include c library
extern "C"{
//include your own C source files
#include "SPPoint.h"
#include "SPLogger.h"
}
int main(char * args){
//Make sure that the logger was set at the beginning of the main function


//start

// COMMAND LINE ARGUMENT with the Error messages
	char * configFilename;
	if (args == NULL){
		configFilename = "spcbir.config";
	}
	else{
		configFilename = args;
	}
	//“Invalid command line : use -c <config_filename>” - check

//init from config
	SP_CONFIG_MSG spConfigMsg;
	SPConfig configFile = spConfigCreate(configFilename,spConfigMsg);
	if (spConfigMsg == SP_CONFIG_CANNOT_OPEN_FILE){
		if (configFilename == "spcbir.config" ){
			//“The default configuration file spcbir.config couldn’t be open” +new line + exit?
		}
		else{
			//#2 “The configuration file <filename> couldn’t be open” + new line +exit?
		}
	}


// extract mode?
	if (spConfigIsExtractionMode(configFile,spConfigMsg)){
		// yes:

		//extract features

		// done?

		// no: repeat

		// yes: save to directory

	}
	else{
		//no:

		//	extract from file
	}

// init data structures

// recive command

// query?
	query:
	char queryImgPath[1024];
    printf("Please enter an image path:\n");
    scanf("%s", queryImgPath);

// no: exit
if (queryImgPath== '<>'){
	goto ending;
}
int NumOfImages = spConfigGetNumOfImages(configFile,spConfigMsg);
int spKNN = spConfigGetspKNN(configFile,spConfigMsg);
int NumOfFeatures =  spConfigGetNumOfFeatures(configFile,spConfigMsg);
int * imgArray = (int*)malloc (sizeof(int)*configFile,spConfigMsg);
int NumOfSimilarImages=spConfigGetNumOfSimilarImages(configFile,spConfigMsg);
int * SimilarImagesArr = (int*)malloc (NumOfSimilarImages*sizeof(int));
int i=0;
for (i=0;i<NumOfSimilarImages;i++){
	SimilarImagesArr[i]=0;
}
i=0;
for (i=0; i<NumOfImages;i++){
	imgArray[i]=0;
}
i=0;
SPPoint * queryFeatures = (SPPoint*) malloc (NumOfFeatures*sizeof (SPPoint));
queryFeatures = getImageFeatures(queryImgPath,int index,int* numOfFeats);
for (i=0;i<NumOfFeatures;i++){
	spKNNSearch( tree,  queryFeatures[i],  spKNN, imgArray);
}
i=0;
int max=0;
int j=0;
for (i=0;i<NumOfSimilarImages;i++){
	for (j=0;j<NumOfFeatures;j++){
		if (imgArray[j]>imgArray[max]){
			max = j;
		}
	}
	SimilarImagesArr[i]= max;
	imgArray[max] =-1;
}
char * imagePath = malloc (1024*sizeof(char));
i=0;
if(spConfigMinimalGui(configFile,spConfigMsg)){

	for (i=0;i<NumOfSimilarImages;i++){
		spConfigGetImagePath(imagePath, configFile, SimilarImagesArr[i]);
		showImage(imagePath);
		//wait
	}
}
else{
	printf("Best candidates for - %d - are:\n",queryImgPath);
	for (i=0;i<NumOfSimilarImages;i++){
		spConfigGetImagePath(imagePath, configFile, SimilarImagesArr[i]);
		printf ("%d\n",imagePath);

	}
}
goto query;
ending:
// ending
//frees ();
return 0;




};


