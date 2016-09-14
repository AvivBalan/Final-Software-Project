#include "SPLogger.h"
#include "SPConfig.h"
#include "SPKDTree.h"


/**
 * Frees all memory resources associate with the variables, depends on
 * the 'phase' parameter.
 * for phase > 0, it free - config, configMsg, imgCounterArray
 * for phase > 1 destroy the logger, with spLoggerDestroy function.
 * for phase > 2 free all imagesFeaturesArray.
 * for phase > 3 destroy kdTree with spKDTreeDestroy function
 *
 */

void spMainAuxFreeMem(int phase, SPConfig config, SP_CONFIG_MSG *configMsg, SPPoint* imagesFeaturesArray, int numOfFeaturesDir, SPKDTree kdTree, int* imgCounterArray);
