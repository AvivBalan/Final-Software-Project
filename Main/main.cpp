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
	char queryImgPath[1024];
    printf("Please enter an image path:\n");
    scanf("%s", queryImgPath);

// no: exit
if (queryImgPath== '<>'){
	//exit
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
i=0;
for (i=0;i<NumOfSimilarImages;i++){

}



};
// yes: find similar images
/*
 * In the final project, we will search for the spNumOfSimilarImages most similar images (notice that spNumOfSimilarImages in Assignment no. 2 was 5) as follow:
1. We will store all features in a special data-structure called KD-TREE (refer to the lecture slides for more information). The construction time of the KD-TREE should be in 𝑂(𝑑×𝑛𝑙𝑜𝑔(𝑛)) where 𝑑=𝑠𝑝𝑃𝐶𝐴𝐷𝑖𝑚𝑒𝑛𝑠𝑖𝑜𝑛 and 𝑛=𝑡ℎ𝑒 𝑡𝑜𝑡𝑎𝑙 𝑛𝑢𝑚𝑏𝑒𝑟 𝑜𝑓 𝑓𝑒𝑎𝑡𝑢𝑟𝑒𝑠 𝑓𝑜𝑟 𝑎𝑙𝑙 𝑖𝑚𝑎𝑔𝑒𝑠 𝑖𝑛 𝑠𝑝𝐼𝑚𝑎𝑔𝑒𝑠𝐷𝑖𝑟𝑒𝑐𝑡𝑜𝑟𝑦
2. For each feature 𝑓𝑖 of the query image, find the k-nearest features (k = spKNN) with respect to 𝑓𝑖 using the k-nearest-neighbor search algorithm (more later).
3. For each image 𝑖𝑚𝑔𝑖 in spImagesDirectory , keep track of the number of times a feature of 𝑖𝑚𝑔𝑖 is among the spKNN nearest features with respect to some feature 𝑓𝑖 in step 2.
4. Display the images in spImagesDirectory with the highest spNumOfSimilarImages ranks as calculated in step 3.
 *
 */
/*
// show results



return 0;
}



