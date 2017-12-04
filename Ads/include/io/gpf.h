

/** @cond ADS_INTERNAL */

#ifndef ADS_GPF_H
#define ADS_GPF_H

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>

#include "typecode.h"
#include "../utility.h"

#define TypeName(ads) #ads

/** @brief signature of ads print function.
 */
typedef void(*ads_genericPrint)(void*,const char*,int);

struct _ads_outputInfo {
	ads_genericPrint printfunction;
	ads_typeCode tc;
	const char * typname;
	int typlen;
	int cutoffstart;
	int cutoffsize;
};

typedef struct _ads_outputInfo ads_outputInfo;

struct _ads_genIoInfo {
	ads_outputInfo OutputArray[ADS_MAXTYPE];
	unsigned int size;
};

void ads_genericIoInit();

bool ads_regExpCmp(const char * s1,int ls,const char * regexp,int n);

int ads_genericIoInsert(ads_genericPrint pfunction,const char * typregexp,int fntcutoff,int subtrSize);

void ads_typeInsert(const char * typname);

int ads_isAdsType(const char * typname);

#endif

/** @endcond */
