/** @cond ADS_INTERNAL */

#ifndef ADSEXTRIE_C
#define ADSEXTRIE_C



#include "../include/extrie.h"

ads_class(ads_extrie)
{
	ads_init(ads_extrie);
	ads_trie root;
};

ads_linkClass(ads_extrie,ads_extrieCopy,ads_extrieDelete);

ads_extrie ads_initExtrie(int datatype)
{
	ads_extrie t = ads_rawObject(ads_extrie);
	t->root = ads_initTrie(datatype);
	return t;
}

void ads_extrieSetTD(ads_extrie self,const ads_td * td)
{
    ads_trieSetTD(self->root,td);
}

ads_extrie ads_extrieCopy(ads_extrie _self)
{
	ads_extrie self ;
	ads_CheckAndCastPointer(_self,ads_extrie,self);
	return NULL;
}

void ads_extrieDelete(ads_extrie _self)
{
	ads_extrie self ;
	ads_CheckAndCastPointer(_self,ads_extrie,self);
	ads_trieDelete(self->root);
	ads_free(self);
}

void ads_printExtrie(ads_extrie self,void (*printfunction)(void*))
{
	ads_printTrie(self->root,printfunction);
}

void ads_extrieInsert_ByRef(ads_extrie self,const char*word,void*data)
{
	ads_trieInsert_ByRef(self->root,word,data);
}

bool ads_extrieSearch(ads_extrie self,const char*word)
{
	return ads_trieSearch(self->root,word);
}

bool ads_extrieRemove(ads_extrie self,const char* word)
{
	return ads_trieDelItem(self->root,word);
}

int ads_extrieNoOfChar(ads_extrie self)
{
	return ads_trieNoOfChar(self->root);
}

int ads_extrieNoOfString(ads_extrie self)
{
	return ads_trieNoOfString(self->root);
}

void trienode_getstring(ads_trieNode*en,ads_str s,ads_list l )
{
	char tmp[2] = {ads_trieNodeKey(en),ads_endOfStr} ;
	ads_strCat(s,tmp);
	if( ads_trieNodeEndOfStr(en) == true)
	{
		ads_listAppend(l,s,ads_str);
	}
	ads_trieNode*tmpen;
	ads_foreach(itm,ads_listIterator(ads_trieNodeLevel(en)),ads_trieNode*)
	{
		tmpen = itm;
		trienode_getstring(tmpen,(ads_str)ads_strCopy(s),l);
	} ads_next(itm);
	ads_strDelete(s);
}

ads_list ads_extrieGetAllWith(ads_extrie self,const char*word)
{
	ads_list l = ads_listNew(ads_str);
    ads_listSetTD(l,ads_strTD);
	ads_trie t = self->root;
	ads_trieNode*trav = ads_trieRoot(t),*next;
	int i,len=strlen(word);
	for(i=0;i<len;i++)
	{
		next = ads_trieNodeContains(trav,word[i]);
		if(next == NULL)
		{
			break;
		}
		trav = next;
	}
	ads_str s;
	if(len == 0 && i == len)
	{
		s = ads_strNew("");
		trienode_getstring(trav,s,l);
	}
	else if(i == len)
	{
		char tmps[len];
		memset(tmps,0,len);
		strncpy(tmps,word,len-1);
		s = ads_strNew(tmps);
		trienode_getstring(trav,s,l);
	}
	return l;
}





#endif

/** @endcond  */
