


#ifndef ADSEXTRIE_H
#define ADSEXTRIE_H

#include "utility.h"
#include "list.h"
#include "stack.h"
#include "str.h"
#include "trie.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ads_extrieNode ads_extrieNode;

ads_classProto(ads_extrie);


#define ads_extrieNew(n) ads_initExtrie(sizeof(n))
ads_extrie ads_initExtrie(int datatype);

void ads_extrieSetTD(ads_extrie self,const ads_td * td);

short ads_extrieGetAds(ads_extrie self);

#define ads_extrieInsert(self,word,value,type) ADS_SETITEM(ads_extrieInsert_ByRef,self,word,value,type)
void ads_extrieInsert_ByRef(ads_extrie self,const char*word,void*data);

bool ads_extrieSearch(ads_extrie self,const char*word);

bool ads_extrieRemove(ads_extrie self,const char* word);

ads_list ads_extrieGetAllWith(ads_extrie self,const char*word);

int ads_extrieNoOfChar(ads_extrie self);

int ads_extrieNoOfString(ads_extrie self);

void ads_printExtrie(ads_extrie self,void (*printfunction)(void*));

ads_extrie ads_extrieCopy(ads_extrie _self);

void ads_extrieDelete(ads_extrie _self);

#ifdef __cplusplus
}
#endif

#endif

