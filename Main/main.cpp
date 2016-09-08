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
//__FILE__ - The preprocessor replaces this macro with the filename in which it appeared.
//	__func__ - The preprocessor replaces this macro with the function name in which it appeared.
//	__LINE__ - The preprocessor replaces this macro with the line number in which it appeared.

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
	SPConfig configFile = spConfigCreate(configFilename,spConfigMsg);  //should destroy it
	if (spConfigMsg == SP_CONFIG_CANNOT_OPEN_FILE){
		if (configFilename == "spcbir.config" ){
			//“The default configuration file spcbir.config couldn’t be open” +new line
			return 0;
		}
		else{
			//#2 “The configuration file <filename> couldn’t be open” + new line
			return 0;
		}
	}


	//variables

	int NumOfImages = spConfigGetNumOfImages(configFile,spConfigMsg);
	int NumOfSimilarImages=spConfigGetNumOfSimilarImages(configFile,spConfigMsg);

	if (NumOfImages<NumOfSimilarImages){
		//ERROR
		spConfigDestroy (configFilename);
	}
	char * imagePath =(char*) malloc (1024*sizeof(char));  // should free it
	if(imagePath ==NULL){
		// error
		spConfigDestroy (configFilename);
		return -1;
	}
	int spKNN = spConfigGetspKNN(configFile,spConfigMsg);
	int NumOfFeatures =  spConfigGetNumOfFeatures(configFile,spConfigMsg);
	int * imgArray = (int*)malloc (sizeof(int)*configFile,spConfigMsg); // should free it
	if(imgArray ==NULL){
		// error
		spConfigDestroy (configFilename);
		free (imagePath);
		return -1;
	}
	int * SimilarImagesArr = (int*)malloc (NumOfSimilarImages*sizeof(int)); // should free it
	if( SimilarImagesArr==NULL){
		// error
		spConfigDestroy (configFilename);
		free(imgArray);
		free (imagePath);
		return -1;
	}
	int i=0;
	for (i=0;i<NumOfSimilarImages;i++){
		SimilarImagesArr[i]=0;
	}
	i=0;
	for (i=0; i<NumOfImages;i++){
		imgArray[i]=0;
	}
	i=0;
	SPPoint * queryFeatures = (SPPoint*) malloc (NumOfFeatures*sizeof (SPPoint)); // should free it
	if(queryFeatures ==NULL){
		// error
		spConfigDestroy (configFilename);
		free(imgArray);
		free(SimilarImagesArr);
		free (imagePath);
		return -1;
	}
	char * queryImgPath =(char*)  malloc (1024*sizeof(char)); // should free it
	if(queryImgPath==NULL){
		// error
		spConfigDestroy (configFilename);
		free(imgArray);
		free(queryFeatures);
		free(SimilarImagesArr);
		free (imagePath);
		return -1;
	}

	int max=0;
	int j=0;

//ERRORS with variables


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

    printf("Please enter an image path:\n");
    scanf("%s", queryImgPath);

// no: exit
if (queryImgPath== '<>'){
	goto ending;
}

// get query features
queryFeatures = getImageFeatures(queryImgPath,int index,int* numOfFeats);
//ERRORS with query features
if (queryFeatures== NULL){
	//ERROR
	goto ending;
}

for (i=0;i<NumOfFeatures;i++){
	spKNNSearch( tree,  queryFeatures[i],  spKNN, imgArray);
}
i=0;
//ERRORS with spKNNSearch



// get most similar images
for (i=0;i<NumOfSimilarImages;i++){
	for (j=0;j<NumOfFeatures;j++){
		if (imgArray[j]>imgArray[max]){
			max = j;
		}
	}
	SimilarImagesArr[i]= max;
	imgArray[max] =-1;
}


i=0;

// show results
if(spConfigMinimalGui(configFile,spConfigMsg)){

	for (i=0;i<NumOfSimilarImages;i++){
		spConfigGetImagePath(imagePath, configFile, SimilarImagesArr[i]);
		// imagepath problems..
		showImage(imagePath);
		getchar();

	}
}
else{
	printf("Best candidates for - %d - are:\n",queryImgPath);
	for (i=0;i<NumOfSimilarImages;i++){
		spConfigGetImagePath(imagePath, configFile, SimilarImagesArr[i]);
		// imagepath problems..
		printf ("%d\n",imagePath);

	}
}

goto query;
ending:
// ending
//frees ();

spConfigDestroy (configFilename);
free (imagePath);
free (SimilarImagesArr);
free (queryFeatures);
free (queryImgPath);
free (imgArray);
//ERRORS with free


return 0;




};


