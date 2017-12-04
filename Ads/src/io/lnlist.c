/** @cond ADS_INTERNAL */

#ifndef ADS_LnList_C
#define ADS_LnList_C


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../../include/io/lnlist.h"

struct ads_structIList
{
	void* value;
	struct ads_structIList *next;
};

struct ads_structIListHead
{
	ads_structIList * head;
	ads_structIList * last;
	int datatype;
	int size;
};

ads_iList ads_initIlist(int datatype)
{
	ads_iList l = (ads_iList)ads_calloc(1,sizeof(ads_structIListHead));
	if( ads_checkMemory(l) != ads_SUCCESS ) return NULL;
	l->head = NULL;
	l->last = NULL;
	l->size = 0;
	l->datatype = datatype;
	return l;
}

ads_structIList* ads_getIListHead(ads_structIListHead * self)
{
    if(!self)return NULL;
    return self->head;
}

void* ads_getIListValue(ads_structIList * trav)
{
    if(!trav)return NULL;
    return trav->value;
}

ads_structIList* ads_getIListNext(ads_structIList * trav)
{
    if(!trav)return NULL;
    return trav->next;
}

void ads_iListAppend_ByRef(ads_structIListHead * self,const void *value)
{
	ads_structIList * l = (ads_structIList*)ads_calloc(1,sizeof(ads_structIList));
	if(!l) return ;
	l->value = ads_calloc(1,self->datatype);
	memcpy(l->value,value,self->datatype);
	l->next = NULL;
	if(self->last != NULL )
	{
		self->last->next = l;
	}
	if(self->head == NULL )
	{
		self->head = l;
	}
	self->last = l;
	self->size += 1;
}

ads_rt ads_iListDelItem(ads_structIListHead * self,int index)
{
    if(!self)
    {
        return ads_registerError("Struct_I_List_Head Not Initialized.");
    }
	if(index<0 || index>=self->size)
	{
		return ads_registerError("Index out of range in ads_structIList delete function");
	}
	int i ;ads_structIList * trav = self->head,*p;
	for(i=0;i<index-1;i++)
	{
		trav = trav->next;
	}
	if(index == 0)
	{
		p = trav;
		self->head = trav->next;
		if(trav->next == NULL)
		{
			self->last = NULL;
		}
	}
	else
	{
		p = trav->next;
		trav->next = trav->next->next;
		if(trav->next == NULL)
		{
			self->last = trav;
		}
 	}
	ads_free(p->value);
	ads_free(p);
	self->size -= 1;
	return ads_SUCCESS;
}

void* ads_iListGetValue_RtnRef(ads_structIListHead * self,int index)
{
    if(!self)
    {
        ads_registerError("Struct_I_List_Head Not Initialized.");
        return NULL;
    }
	if(index<0 || index >= self->size)
	{
		ads_registerError("Index out of range in Linklist delete function");
		return NULL;
	}
	int i ;ads_structIList * trav = self->head;
	for(i=0;i<index;i++)
	{
		trav = trav->next;
	}
	return trav->value;
}

ads_rt ads_iListSetValue(ads_structIListHead * self,int index,void*value)
{
    if(!self)return ads_registerError("I_list Not initialized.");
	if(index<0 || index>=self->size)
	{
		return ads_registerError("Index out of range in Linklist delete function");
	}
	int i ;ads_structIList * trav = self->head;
	for(i=0;i<index;i++)
	{
		trav = trav->next;
	}
	ads_free(trav->value);
	trav->value = ads_calloc(1,self->datatype);
	memcpy(trav->value,value,self->datatype);
	return ads_SUCCESS;
}

int ads_iListLen(ads_structIListHead * self)
{
    if(self) return self->size;
    return 0;
}

int ads_iListSizeOf(ads_structIListHead * self)
{
	int cou = 0 ;
	cou += self->size*(sizeof(ads_structIList)+self->datatype);
	cou += sizeof(ads_structIListHead);
	return cou;
}

void ads_deleteIList(ads_structIListHead * self)
{
    if(!self) return;
	int i ;ads_structIList * trav = self->head,*p;
	for(i=0;trav != NULL;i++)
	{
		p = trav;
		trav = trav->next;
		ads_free(p->value);
		ads_free(p);
	}
	ads_free(self);
}
#endif
/** @endcond */
