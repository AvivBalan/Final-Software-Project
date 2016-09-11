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
	int numOfImages = spConfigGetNumOfImages(config, configMsg);
	char * imagePath = (char*) malloc (1024 * sizeof(char));  // should free it
	int spKNN = spConfigGetspKNN(config, configMsg);
	int numOfFeatures =  spConfigGetNumOfFeatures(config,configMsg);
	int* imgArray = (int*) calloc (numOfImages, sizeof(int)); // should free it
	int numOfSimilarImages=spConfigGetNumOfSimilarImages(config,configMsg);
	int* similarImagesArr = (int*) malloc (numOfSimilarImages * sizeof(int)); // should free it
	int i=0;


/*
	for (i = 0; i < numOfImages; i++){
		imgArray[i]=0;
	}
*/
	SPPoint* queryFeatures = (SPPoint*) malloc (numOfFeatures * sizeof (SPPoint)); // should free it
	char* queryImgPath =(char*)  malloc (1024*sizeof(char)); // should free it

	int max=0;
	int j=0;
// extract mode?
	if (spConfigIsExtractionMode(config,configMsg)){ //ExtractionMode
		// yes:

		//extract features

		// done?

		// no: repeat

		ImageProc(config);
		// yes: save to directory

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
for (i=0;i<NumOfFeatures;i++){
	spKNNSearch( tree,  queryFeatures[i],  spKNN, imgArray);
}
i=0;

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
if(spConfigMinimalGui(config,configMsg)){

	for (i=0;i<NumOfSimilarImages;i++){
		spConfigGetImagePath(imagePath, config, SimilarImagesArr[i]);
		showImage(imagePath);
		//wait
	}
}
else{
	printf("Best candidates for - %d - are:\n",queryImgPath);
	for (i=0;i<NumOfSimilarImages;i++){
		spConfigGetImagePath(imagePath, config, SimilarImagesArr[i]);
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

return 0;
};
