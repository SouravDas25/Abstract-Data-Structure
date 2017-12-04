




#ifndef stringpool_H
#define stringpool_H

#include "utility.h"
#include "list.h"
#include "stack.h"
#include "dict.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ads_structString ads_structString;

size_t ads_sizeOfStructString(ads_structString*s);

const char* ads_structStringData(ads_structString*s);

int ads_structStringLen(ads_structString*s);

ads_structString* ads_newStructString(const char * str);
ads_structString* ads_newStringObj(const char * str);
ads_structString* ads_copyStringObj(ads_structString* strRef);
ads_rt ads_freeStringObj(ads_structString* strRef);
void ads_printAdsStructString(void* value,const char * cmd,int size);
void ads_printStringPool();

#ifdef __cplusplus
}
#endif

#endif


