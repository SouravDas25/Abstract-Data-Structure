
/** @file datareplicator.h
 * @brief a semi-automatic garbage collector for c.
 *
 */


#ifndef ADSDATAREPLICATOR_H
#define ADSDATAREPLICATOR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#include<math.h>
#include<stdarg.h>
#include <time.h>
#include<stdbool.h>

#include "Error.h"
#include "gc.h"
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*ads_cmpFunc)(const void*,const void*);

extern const ads_cmpFunc ads_cmpMem;

typedef struct _ads_td ads_td;
typedef void (*ads_destroyFunction)(void*);

struct _ads_td {
    void* (*copy) (const void*);
    ads_destroyFunction destroyer;
    size_t size;
} ;


/** @brief takes the address and size of a variable and return a dynamically allocated copy of it.
 *
 * @param address address of the value to copy
 * @param size no of bytes to copy
 * @return void* pointer to new copied value
 *
 */
void* ads_copyAlloc(const void * address,int size);

/** @cond ADS_INTERNAL */

ads_td * ads_tdNew(void* (*copy) (const void*),ads_destroyFunction destroyer,size_t size);

ads_rt ads_tdSet(ads_td *td,void* (*copy) (const void*),ads_destroyFunction destroyer,size_t s);

ads_rt ads_tdCopy(ads_td * destination,const ads_td * source);

bool ads_tdCmp(const ads_td * td1,const ads_td * td2);

size_t ads_tdGetSize(const ads_td *td);

ads_rt ads_tdSetSize(ads_td *td,size_t s);

void ** ads_toItem(void*v);
void * ads_fromItem(void**v);

/* make a copy of the datatype provided if datatype is ADS then it calls copyconstructor else copy function .*/
void* ads_appropriteCopy(ads_td * td,const void*value);

/*frees memory occupied by objects if ADS type then calls destructor.*/
void ads_appropriateDelete(ads_td * td,void*value);

void* ads_evaluatePointer(int isads,void**value) ;

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif
