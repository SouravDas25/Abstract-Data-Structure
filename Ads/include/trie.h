

#ifndef ADSTRIEC_H
#define ADSTRIEC_H

#include "utility.h"
#include "list.h"
#include "stack.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ads_trieNode ads_trieNode;

ads_list ads_trieNodeLevel(ads_trieNode*tn);

char ads_trieNodeKey(ads_trieNode*tn);

char ads_trieNodeEndOfStr(ads_trieNode*tn);

ads_classProto(ads_trie);

#define ads_trieNew(n) ads_initTrie(sizeof(n))
ads_trie ads_initTrie(int datatype);

void ads_trieSetTD(ads_trie self,const ads_td * td);

ads_trieNode* ads_trieRoot(ads_trie self);

int ads_trieNoOfChar(ads_trie self);
int ads_trieNoOfString(ads_trie sehlf);

int ads_trieSizeOf(ads_trie self);

ads_trieNode* ads_trieNodeContains(ads_trieNode* tn,char key);

#define ads_trieInsert(self,word,value,type) ADS_SETITEM(ads_trieInsert_ByRef,self,word,value,type)
void ads_trieInsert_ByRef(ads_trie self,const char * word,void * data);

bool ads_trieSearch(ads_trie self,const char*word);

#define ads_trieGetItem_NoCopy(self,word,type,result) ADS_GETITEM(ads_trieGetItem_NoCopy_ByRef,self,word,type,result)
void* ads_trieGetItem_NoCopy_ByRef(ads_trie self,const char * word);

bool ads_trieDelItem(ads_trie self,const char * word);

void ads_printTrie(ads_trie self,void (*printfunction)(void*));

void ads_trieDelete(ads_trie _self);

#ifdef __cplusplus
}
#endif

#endif

