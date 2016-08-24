extern "C"{
	#include "SPConfig.h"
	#include "SPConfig_unit_test.h"
}

int main(){
	int i = spConfig_unit_testtestspConfigCreate();
	i = i * testspConfigIsExtractionMode();
	i = i * testspConfigMinimalGui();
	i = i * testspConfigGetNumOfImages();
	i = i * testspConfigGetNumOfFeatures();
	i = i * testspConfigGetPCADim();
	i = i * testspConfigGetImagePath();
	i = i * testspConfigGetPCAPath();
	i = i * testisEmpty();
	printf("%d\n",i);
	return i;
}
