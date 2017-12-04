
/** @cond ADS_INTERNAL */

#ifndef ADS_LnList_H
#define ADS_LnList_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../utility/error.h"
#include "../utility/gc.h"

typedef struct ads_structIList ads_structIList;

typedef struct ads_structIList* ads_iListNode;

typedef struct ads_structIListHead ads_structIListHead ;

typedef ads_structIListHead* ads_iList;

#define ads_iListNew(type) ads_initIlist(sizeof(type))
ads_iList ads_initIlist(int datatype);

ads_iListNode ads_getIListHead(ads_structIListHead * self);

void* ads_getIListValue(ads_structIList * trav);

ads_iListNode ads_getIListNext(ads_structIList * trav);

#define ads_iListAppend(self,value,type) {type * __ADS_lnlist_ = &value; ads_iListAppend_ByRef(self,__ADS_lnlist_);}
void ads_iListAppend_ByRef(ads_structIListHead * self,const void *value);

ads_rt ads_iListDelItem(ads_structIListHead * self,int index);

#define ads_iListGetItem(self,index,type,result) {void * __ADS__list_ = ads_iListGetValue_RtnRef(self,index);if(__ADS__list_)result=*(type*)__ADS__list_;}
void* ads_iListGetValue_RtnRef(ads_structIListHead * self,int index);

ads_rt ads_iListSetValue(ads_structIListHead * self,int index,void*value);

int ads_iListSizeOf(ads_structIListHead * self);

int ads_iListLen(ads_structIListHead * self);

void ads_deleteIList(ads_structIListHead * self);
#endif

/** @endcond */
