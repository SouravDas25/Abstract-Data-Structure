
#ifndef ADSCSTR_H
#define ADSCSTR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "ads_stream.h"
#include "datareplicator.h"

extern const ads_td * const ads_cstrTD;

void ads_printCStr(const void*add);
void ads_printCStr_ByRef(const void*add);

void* ads_cstrCopy(const void*v);
void* ads_cstrCopy_ByRef_RtnRef(const void*v);

void ads_cstrDelete( void*v);
void ads_cstrDelete_ByRef( void*v);

int ads_cmpCStr(const void*a,const void*b);
int ads_cmpCStr_ByRef(const void*a,const void*b);

uint64_t ads_cstrHash(const void*buffer); /**< produces a hash value of a ads_str */
uint64_t ads_cstrHash_ByRef(const void * _buffer);
#ifdef __cplusplus
}
#endif

#endif // ADSCSTR_H
