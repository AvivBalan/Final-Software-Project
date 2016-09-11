/*
 * main.cpp
 *
 *  Created on: 11 בספט 2016
 *      Author: נגה
 */

#include <cstdlib> //include c library
#include <cstdio>
extern "C"{
//include your own C source files
#include "SPPoint.h"
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPFeaturesFiles.h"
#include "SPKDTree.h"
}
#include "SPImageProc.h"

int main(int argc,char** argv){
	char* configFilename;
	SPConfig config;
	SP_CONFIG_MSG *configMsg;


//Make sure that the logger was set at the beginning of the main function


//start
/*
 * TO BE FIXED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//COMMAND LINE ARGUMENT with the Error messages
	if (argc < 2){ //????????????????????????????????????????????????????
		configFilename = "spcbir.config";
	}
	else{
		configFilename = argv[2];
	}
*/

	//create config
	configMsg = (SP_CONFIG_MSG*) malloc (sizeof(SP_CONFIG_MSG));
	if(configMsg == NULL)
		return 1; //Allocation error
	config = spConfigCreate(configFilename, configMsg);  //should destroy it
	if(*configMsg != SP_CONFIG_SUCCESS)
		return 1;
	//Logger
	//????????????????????????????????????????????????????????????????????????????



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
	int * imgArray = (int*)malloc (sizeof(int)*configFile,spConfigMsg);
	if(imgArray ==NULL){
		// error
		spConfigDestroy (configFilename);
		free (imagePath);
		return -1;
	}
	int * SimilarImagesArr = (int*)malloc (NumOfSimilarImages*sizeof(int));
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
	SPPoint * queryFeatures = (SPPoint*) malloc (NumOfFeatures*sizeof (SPPoint));
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
	SPPoint* imagesFeaturesArray = (SPPoint*)malloc (numOfImages*numOfFeatures*sizeof (SPPoint));
	if(imagesFeaturesArray==NULL){
		// error
		spConfigDestroy (configFilename);
		free(imgArray);
		free(queryFeatures);
		free(SimilarImagesArr);
		free (imagePath);
		free (queryImgPath);
		return -1;
	}

	int i=0;


	for (i = 0; i < numOfImages; i++){
		imgArray[i]=0;
	}

	SPPoint* queryFeatures = (SPPoint*) malloc (numOfFeatures * sizeof (SPPoint));
	char* queryImgPath =(char*)  malloc (1024*sizeof(char)); // should free it

	int max=0;
	int j=0;
	int k =0;


// extract mode?
	ImageProc(config);

	if (spConfigIsExtractionMode(config,configMsg)){ //ExtractionMode

		for (i=1;i<numOfImages+1;i++){
			if (spConfigGetImagePath (imagePath,config,i) == SP_CONFIG_SUCCESS){
				// using queryFeatures because its only temp use
				queryFeatures =getImageFeatures (imagePath,int index,int* numOfFeats);
				for (j=0;j<numOfFeatures; j++){
					imagesFeaturesArray[k]= queryFeatures[j];
					k++;
				}
			}

		}


	}
	else{ //nonExtractionMode
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
free (imagesFeaturesArray);

return 0;
};


