

/** @cond ADS_INTERNAL */

#ifndef ADSTRIE_H
#define ADSTRIE_H



#include "../include/trie.h"

ads_class(ads_trie)
{
	ads_init(ads_trie);
	ads_trieNode* root;
	ads_td * dtd;
	unsigned short size;
	unsigned short noofstring;
};

ads_linkClass(ads_trie,ads_trieCopy,ads_trieDelete);

struct ads_trieNode
{
	ads_list  thislevel;
	void * data ;
	char key ;
	char endofstring ;
};

ads_list ads_trieNodeLevel(ads_trieNode*tn)
{
    return tn->thislevel;
}

char ads_trieNodeKey(ads_trieNode*tn)
{
    return tn->key;
}

char ads_trieNodeEndOfStr(ads_trieNode*tn)
{
    return tn->endofstring;
}

ads_trie ads_initTrie(int datatype)
{
	ads_trie t = ads_rawObject(ads_trie);
	t->root = NULL;
	t->size = 0;
	t->noofstring = 0;
	t->dtd = ads_tdNew(NULL,NULL,datatype);
	return t;
}

void ads_trieSetTD(ads_trie self,const ads_td * td)
{
    ads_tdCopy(self->dtd,td);
}

ads_trieNode* ads_trieRoot(ads_trie self)
{
    return self->root;
}

ads_trieNode* ADS_newtrienode(int key,void * data)
{
	ads_trieNode* t = (ads_trieNode*)ads_calloc(1,sizeof(ads_trieNode));
	ads_checkMemory(t);
	t->data = data;
	t->key = key;
	t->endofstring = false;
	t->thislevel = ads_listNew(ads_trieNode*);
	return t;
}

int ads_trieNoOfChar(ads_trie self)
{
	return self->size;
}

int ads_trieNoOfString(ads_trie self)
{
	return self->noofstring;
}

int ads_trieSizeOf(ads_trie self)
{
	int size = sizeof(ADS_StructOf(ads_trie));
	size += sizeof(ads_trieNode)*self->size;
	size += self->size* ads_tdGetSize(self->dtd);
	size += ads_listSizeOf(self->root->thislevel)*self->size;
	return size;
}

ads_trieNode* ads_trieNodeContains(ads_trieNode* tn,char key)
{
	if(tn == NULL) return NULL;
	ads_foreach(element,ads_listIterator(tn->thislevel),ads_trieNode*)
	{
		char a1 = element->key;
		if(key == a1)
		{
			return element;
		}
	}ads_next(element);
	return NULL;
}

void ads_trieInsert_ByRef(ads_trie self,const char * word,void * data)
{
	if(self->root == NULL)
	{
		self->root = ADS_newtrienode(0,NULL);
		self->size++;
	}
	void * value = ads_appropriteCopy(self->dtd,data);
	int i,len=strlen(word);
	ads_trieNode*trav= self->root,*next;
	for(i=0;i<len;i++)
	{
		next = ads_trieNodeContains(trav,word[i]);
		if(next==NULL)
		{
			ads_trieNode*ne = ADS_newtrienode(word[i],NULL);
			self->size++;
			ads_listAppend(trav->thislevel,ne,ads_trieNode*);
			trav = ne;
		}
		else
		trav = next;
	}
	trav->endofstring = true;
	trav->data = value;
	self->noofstring++;
}

bool ads_trieSearch(ads_trie self,const char*word)
{
	int i,len=strlen(word);
	if(self->root == NULL)
	{
		return false;
	}
	ads_trieNode*trav= self->root,*next;
	for(i=0;i<len;i++)
	{
		next = ads_trieNodeContains(trav,word[i]);
		if(next == NULL)
		{
			return false;
		}
		trav = next;
	}
	if(trav->endofstring == true) return true;
	return false;
}

void* ads_trieGetItem_NoCopy_ByRef(ads_trie self,const char * word)
{
	int i,len=strlen(word);
	ads_trieNode*trav= self->root,*next;
	for(i=0;i<len;i++)
	{
		next = ads_trieNodeContains(trav,word[i]);
		if(next == NULL)
		{
			ads_registerError("String '%s' Not Fount in EXTRIE ",word);
			return NULL;
		}
		trav = next;
	}
	if( trav->endofstring == true )
	{
		if(trav->data != NULL)
		{
			return trav->data;
		}
		else
		{
			ads_registerError("String '%s' Not Fount in EXTRIE ",word);
			return NULL;
		}
	}
	ads_registerError("String '%s' Not Fount in EXTRIE ",word);
	return NULL;
}

bool ads_trieDelItem(ads_trie self,const char * word)
{
	ads_stack estck = ads_stackNew(ads_trieNode*);
	int i,len = strlen(word);
	ads_trieNode*trav= self->root,*next=NULL;
	for(i=0;i<len;i++)
	{
		next = ads_trieNodeContains(trav,word[i]);
		if(next == NULL)
		{
			ads_registerError("String '%s' Not Fount in EXTRIE ",word);
			ads_delete(estck);
			return false;
		}
		ads_stackPush(estck,trav,ads_trieNode*);
		trav = next;
	}
	ads_stackPush(estck,trav,ads_trieNode*);
	if( trav->endofstring != true )
	{
		ads_registerError("String '%s' Not Fount in EXTRIE ",word);
		ads_delete(estck);
        return false;
	}
	trav->endofstring = false;
	ads_free(trav->data);
	trav->data = NULL;
	while( ads_stackIsEmpty(estck) == false )
	{
		ads_stackPop(estck,ads_trieNode*,trav);
		if( trav->key == '\0') continue ;
		if( ads_listLen(trav->thislevel) > 0 ) break ;
		if( trav->data != NULL ) break ;
		ads_stackFront_Copy(estck,ads_trieNode*,next);
		ads_listRemove( next->thislevel , ads_cmpMem , trav , ads_trieNode* );
		ads_listDelete(trav->thislevel);
		ads_free(trav);
	}
	ads_delete(estck);
	self->noofstring--;
	return true;
}

void __printtrienode(ads_trieNode* tn,const char* buf,void (*printfunction)(void*))
{
	int n = strlen(buf);
	char b[n+5];
	ads_output(buf);
	strcpy(b,buf);
	ads_output("%c",tn->key);
	if(tn->data != NULL )
	{
		ads_output(": ");
		printfunction(tn->data);
	}
	ads_output("\n");
	int ii=0,len = ads_listLen(tn->thislevel);
	if(len == 1)
	{
		int l=-1;
		while(b[++l]!= ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
		strcat(b,"\xc0\xc4\xc4");
		__printtrienode(*(ads_trieNode**)ads_listGetItem_NoCopy_RtnRef(tn->thislevel,0),b,printfunction);
	}
	else if(len > 1)
	{
		int l=-1;
		while(b[++l] != ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
		strcat(b,"\xc3\xc4\xc4");
		ads_foreach(itm,ads_listIterator(tn->thislevel),ads_trieNode*)
		{
			if(ii==len-1)
			{
				b[n] = '\xc0';
				__printtrienode(itm,b,printfunction);
			}
			else
			{
				__printtrienode(itm,b,printfunction);
			}
			ii++;
		}ads_next(itm);
	}
}

void ads_printTrie(ads_trie self,void (*printfunction)(void*))
{
	ads_output("\nTrie BEGIN:\n\nRoot");
	if(self->root != NULL ) __printtrienode(self->root,"  ",printfunction);
	ads_output("\nTrie END:\n");
}

ads_trie ads_trieCopy(ads_trie _self)
{
	ads_trie self ;
	ads_CheckAndCastPointer(_self,ads_trie,self);
	return NULL;
}

void __freetrienode(ads_trieNode* en)
{
	ads_foreach(itm,ads_listIterator(en->thislevel),ads_trieNode*)
	{
		__freetrienode(itm);
	}ads_next(itm);
	ads_listDelete(en->thislevel);
	if(en->data != NULL) ads_free(en->data);
	ads_free(en);
}

void ads_trieDelete(ads_trie _self)
{
	ads_trie self ;
	ads_CheckAndCastPointer(_self,ads_trie,self);
	__freetrienode(self->root);
	ads_free(self->dtd);
	ads_free(self);
}

#endif

/** @endcond  */
