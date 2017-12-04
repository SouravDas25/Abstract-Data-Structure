/** @cond ADS_INTERNAL */

#ifndef ADSLinklist_C
#define ADSLinklist_C

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/list.h"

struct ads_listContainer                            /*declaring ads_list node using structure*/
{
	void * value;                                   /*void* to hold any kind of datatype it is actually a pointer to ant kind of datatype*/
	struct ads_listContainer * next;                /*pointer to next node in ads_list or NULL if no node available*/
};

ads_listContainer* ads_listXor(ads_listContainer*arg1,ads_listContainer*arg2)
{
	return (ads_listContainer*) ((uintptr_t)arg1^(uintptr_t)arg2);
}

void* ads_listContainerValue(ads_listContainer * container)
{
    return container->value;
}

ads_listContainer* ads_listContainerNext(ads_listContainer * container)
{
    return container->next;
}

ads_class(ads_list) {                  /*ads_class is a macro define in object.h header file it is use to create object like structure*/
	ads_init(lnklist);                 /*ads_init is a macro define in object.h it is used to declare standard class method and class type*/
    ads_listContainer * head;         /*node where the ads_list start*/
	ads_listContainer * tail;         /*node where the ads_list end*/
	ads_listContainer * now;		  /*address of the recently accessed item*/
	ads_listContainer * previous;	  /*address of the previous node of recently accessed item*/
	ads_td * dtd;
	uint32_t nowindex;		  /*index of the recently accessed item*/
	uint32_t size;  					  /*no of node in ads_list*/
};

ads_linkClass(ads_list,ads_listCopy,ads_listDelete);


static void Adslist_setiterator(ads_list self,ads_listContainer*now,ads_listContainer*prev,int index)
{
	self->now = now;
	self->previous = prev;
	self->nowindex = index;
}

ads_list ads_initList(size_t datatype)
{
	ads_list  l = ads_rawObject(ads_list);
	if( ads_checkMemory(l) != ads_SUCCESS ) return l ;
	l->head = NULL;
	l ->tail = NULL;
	l->size = 0;
	l->dtd = ads_tdNew(NULL,NULL,datatype);
	l->now = NULL;
	l->previous = NULL;
	l->nowindex = -1;
	if(!ADS_MethodVariableName(ads_list).TypeCode) {
		ADS_MethodVariableName(ads_list).TypeCode = ads_genericIoInsert(ads_printList,"list(#)",5,6);
	}
	return l;
}

ads_listContainer *ads_linklistHead(ads_list self)
{
    return self->head;
}

ads_td* ads_listGetTD(ads_list self)
{
    return self->dtd;
}

ads_rt ads_listSetTD(ads_list self,const ads_td* td)
{
    if(!self) return ads_registerError("ads_list Not Initialized.");
    if( ads_listLen(self) > 0 ) return ads_registerError("Type definition Can Only Be Set Before Any Element is added.");
    ads_tdCopy(self->dtd,td);
    return ads_SUCCESS;
}

ads_rt ads_listAppend_NoCopy(ads_list self,void * value)
{
	ads_listContainer * l = (ads_listContainer*)ads_calloc(1,sizeof(ads_listContainer)),*prev = self->tail,*next = NULL ;
	if(ads_checkMemory(l) != ads_SUCCESS) return ads_ERROR;
	if(!value) return ads_registerError("Cannot Append Null Values.");
	l->value = value;
	l->next = ads_listXor(prev,next);
	if(self->head == NULL)
	{
		self->head = l;
		self->tail = l;
	}
	else
	{
		prev = ads_listXor(self->tail->next,NULL);
		self->tail->next = ads_listXor(prev,l);
		self->tail = l;
	}
	self->size++;
	return ads_SUCCESS;
}

ads_rt ads_listAppend_ByRef(ads_list self,void * value)
{
	if(value == NULL)
	{
		return ads_registerError("Cannot copy NULL in ads_list Append");
	}
	void * cal = ads_appropriteCopy(self->dtd,value);
	return ads_listAppend_NoCopy(self,cal);
}

static ads_listContainer* Ads_traverseThriter(ads_list self,int index,ads_listContainer **addofprev,ads_listContainer **addofnext)
{
	int i;ads_listContainer * trav = NULL,*prev = NULL,*next=NULL;
	if(self->now == NULL || self->previous == NULL) return NULL;
	if( ( abs(index-self->nowindex) < index) && ( abs(index-self->nowindex) < abs(ads_listLen(self)-1 - index)) )
	{
		trav = self->now;
		prev = self->previous;
		for(i=self->nowindex;i<index;i++)
		{
			next = ads_listXor(trav->next,prev);
			prev = trav;
			trav = next;
		}
		if(addofprev)*addofprev = prev;
		if(addofnext)*addofnext = ads_listXor(trav->next,prev);
		Adslist_setiterator(self,trav,prev,index);
	}
	else if( ( abs(self->nowindex-index) < index) &&  ( abs(self->nowindex-index) < abs(ads_listLen(self)-1-index)) )
	{
		trav = self->previous;
		next = self->now;
		for(i=self->nowindex;i>index;i--)
		{
			prev = ads_listXor(trav->next,next);
			next = trav;
			trav = prev;
		}
		if(addofprev)*addofnext = next;
		if(addofnext)*addofprev = ads_listXor(trav->next,next);
		Adslist_setiterator(self,trav,ads_listXor(trav->next,next),index);
	}
	return trav;
}

ads_listContainer* ADS_listtransverse(ads_list self,size_t index,ads_listContainer **addofprev,ads_listContainer **addofnext)
{
	if(index >= self->size)
	{
		ads_registerError("ads_list Transverse Function Index Error %d",index);
		return NULL;
	}
	size_t i ;ads_listContainer * trav = self->head,*prev = NULL,*next=NULL;
	if(trav==NULL)
	{
		ads_registerError("Can't access an empty ads_list");
		return NULL;
	}
	if(self->now != NULL)
	{
		trav = Ads_traverseThriter(self,index ,addofprev,addofnext);
		if(trav != NULL) return trav;
	}
	if(index <= self->size/2)
	{
		trav = self->head;
		prev = NULL;
		for(i=0;i<index;i++)
		{
			next = ads_listXor(trav->next,prev);
			prev = trav;
			trav = next;
		}
		if(addofprev)*addofprev = prev;
		if(addofnext)*addofnext = ads_listXor(trav->next,prev);
		Adslist_setiterator(self,trav,prev,index);
	}
	else
	{
		trav = self->tail;
		next = NULL;
		for(i=self->size-1;i>index;i--)
		{
			prev = ads_listXor(trav->next,next);
			next = trav;
			trav = prev;
		}
		if(addofprev)*addofnext = next;
		if(addofnext)*addofprev = ads_listXor(trav->next,next);
		Adslist_setiterator(self,trav,ads_listXor(trav->next,next),index);
	}
	return trav;
}

ads_rt ads_listSetItem_ByRef(ads_list self,size_t index,void*value)
{
	if(index >= self->size)
	{
		return ads_registerError("ads_list SetItem Function Index Error %d",index);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot copy NULL in ads_list Setitem");
	}
	ads_listContainer * trav = ADS_listtransverse(self,index,NULL,NULL);
	if(trav == NULL)
	{
		return ads_registerError("Can't access an empty ads_list");
	}
	void * cal = ads_appropriteCopy(self->dtd,value);
	if(!cal) return ads_ERROR;
	if(trav->value != NULL)  ads_appropriateDelete(self->dtd,trav->value) ;
	trav->value = cal;
	return ads_SUCCESS;
}

ads_rt ads_listInsert_ByRef(ads_list self,size_t index,void*value)
{
	if(index >= self->size)
	{
		return ads_registerError("ads_list Insert Function Index Error %d",index);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot copy NULL in ads_list Setitem");
	}
	if(self->head == NULL)
	{
		return ads_registerError("Can't access an empty ads_list");
	}
	void * cal = ads_appropriteCopy(self->dtd,value);
	if(!cal) return ads_ERROR;
	ads_listContainer * l = (ads_listContainer*)ads_calloc(1,sizeof(ads_listContainer));
	if(ads_checkMemory(l) != ads_SUCCESS) return ads_ERROR;
	l->value = cal;
	ads_listContainer * trav ,*prev ,*next=NULL;
	if(index == 0)
	{
		trav = self->head;
		prev = NULL;
		next = ads_listXor(trav->next,prev);
		self->head = l;
		trav->next = ads_listXor(l,next);
		l->next = ads_listXor(prev,trav);
		Adslist_setiterator(self,self->head,NULL,index);
	}
	else
	{
		trav = ADS_listtransverse(self,index,&prev,&next);
		ads_listContainer * prevofprev = ads_listXor(prev->next,trav);
		l->next = ads_listXor(prev,trav);
		prev->next = ads_listXor(prevofprev,l);
		trav->next = ads_listXor(l,next);
		Adslist_setiterator(self,l,prev,index);
	}
	self->size++;
	return ads_SUCCESS;
}

uint32_t ads_listLen(ads_list self)
{
    if(!self)return 0;
	return self->size;
}

size_t ads_listSizeOf(ads_list self)
{
	size_t s = sizeof(ADS_StructOf(ads_list));
	s+= ads_listLen(self) * (sizeof(ads_listContainer) + ads_tdGetSize(self->dtd));
	return s;
}

void* ads_listGetItem_NoCopy_RtnRef(ads_list  self,size_t index)
{
	if(index >= self->size)
	{
		ads_registerError("ads_list Getitem Function Index Error %d",index);
		return NULL;
	}
	ads_listContainer * trav = ADS_listtransverse(self,index,NULL,NULL);
	if(trav == NULL)
	{
		ads_registerError("Can't access an empty ads_list");
		return NULL;
	}
	return trav->value;
}

void* ads_listGetItem_Copy_RtnRef(ads_list  self,size_t index)
{
	void * cal ,*value = ads_listGetItem_NoCopy_RtnRef(self,index) ;
	cal = ads_appropriteCopy(self->dtd,value);
	return cal;//*(int64_t*)cal
}

ads_rt ads_listDelItem(ads_list  self,size_t index)
{
	if(index >= self->size)
	{
		return ads_registerError("ads_list Delitem Function Index Error %d",index);
	}
	ads_listContainer * trav = self->head,*p,*prev = NULL,*next,*prevofprev,*nextofnext;
	if(trav== NULL)
	{
		return ads_registerError("Can't delete from an empty ads_list");
	}
	trav = ADS_listtransverse(self,index,&prev,&next);
	if(index == 0)
	{
		p = self->head;
		next = ads_listXor(self->head->next,NULL);
		if(next != NULL)
		{
			nextofnext = ads_listXor(next->next,p);
			next->next = ads_listXor(nextofnext,NULL);
		}
		else
		{
			self->tail = next;
		}
		self->head = next ;
		Adslist_setiterator(self,self->head,NULL,index);
	}
	else
	{
		p = trav;
		prevofprev = ads_listXor(prev->next,trav);
		prev->next = ads_listXor(prevofprev,next);
		if(next != NULL)
		{
			nextofnext = ads_listXor(next->next,trav);
			next->next = ads_listXor(nextofnext,prev);
		}
		else
		{
			self->tail = prev;
		}
		Adslist_setiterator(self,next,prev,index);
	}
	self->size--;
	ads_appropriateDelete(self->dtd,p->value);
	ads_free(p);
	return ads_SUCCESS;
}

bool ads_listEmpty(ads_list self)
{
	if(self->head == NULL)return true;
	return false;
}

void* ads_listPop_RtnRef(ads_list self)
{
	ads_listContainer * p = self->head,*next,*nextofnext;
	if(p == NULL)
	{
		ads_registerError("Cannot Pop From Empty ads_list");
		return NULL;
	}
	void * val = p->value;
	next = ads_listXor(self->head->next,NULL);
	if(next != NULL)
	{
		nextofnext = ads_listXor(next->next,p);
		next->next = ads_listXor(nextofnext,NULL);
	}
	else
	{
		self->tail = NULL;
	}
	Adslist_setiterator(self,self->head,NULL,0);
	self->head = next ;
	ads_free(p);
	self->size--;
	return val;
}

void* ads_listPopItem_RtnRef(ads_list self,size_t index)
{
	if(index >= self->size)
	{
		ads_registerError("ads_list Popitem Function Index Error %d",index);
		return NULL;
	}
	void * v = ads_listGetItem_Copy_RtnRef(self,index);
	if ( ads_listDelItem(self,index) != ads_SUCCESS )
    {
        ads_appropriateDelete(self->dtd,v);
        return NULL;
    }
	return v;
}

ads_rt ads_listExtend(ads_list self,ads_list other)
{
    if(!ads_tdCmp(self->dtd,other->dtd))
	{
		return ads_registerError("Type mismatch of two ads_list ");
	}
	ads_listContainer * trav=NULL,*p=NULL,*prev = NULL,*next=NULL;
	trav = other->head;
	while(trav != NULL)
	{
		p = trav;
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		ads_listAppend_ByRef(self,p->value);
	}
	return ads_SUCCESS;
}

int ads_listIndex_ByRef(ads_list self,void * value,int (*comparefunction)(const void*,const void*))
{
	ads_listContainer * trav,*prev = NULL,*next;
	trav = self->head;int i = 0;
	while(trav != NULL)
	{
		if (ads_appropriateCmp(comparefunction,trav->value,value,ads_tdGetSize(self->dtd)) == 0 )
		{
			return i;
		}
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		i++;
	}
	return -1;
}

void ads_listRemove_ByRef(ads_list self,void * value,int (*comparefunction)(const void*,const void*))
{
	int index = ads_listIndex_ByRef(self,value,comparefunction);
	if(index != -1 ) ads_listDelItem(self,index);
}

ads_rt ads_listReverse(ads_list self)
{
	ads_listContainer * trav = self->head;
	self->head = self->tail;
	self->tail = trav;
	self->nowindex = ads_listLen(self) - self->nowindex;
	trav = self->now;
	self->now = self->previous;
	self->previous = self->now;
	return ads_SUCCESS;
}

void* __listiterstart(ads_iterator* i)
{
	ads_list s = (ads_list)i->object;
	return ads_listGetItem_NoCopy_RtnRef(s,i->count);
}

void* __listiternext(ads_iterator* i)
{
	ads_list s = (ads_list)i->object;
	i->max = (unsigned)ads_listLen(s);
	if((unsigned)ads_listLen(s)>i->count)
	{
		return ads_listGetItem_NoCopy_RtnRef(s,i->count);
	}
	return NULL;
}

ads_iterator ads_listIterator(ads_list self)
{
	ads_iterator i = ads_initIterator(self,__listiternext,__listiterstart,ads_listLen(self),NULL);
	return i;
}


void ads_printList(void*add,const char * cmd,int size)
{
	ads_list self ;
	add = *(void**)add;
	ads_CheckAndCastPointer(add,ads_list,self);
	ads_listContainer * trav,*prev = NULL,*next;
	trav = self->head;
	int cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint print;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);
	printf("ads_list[ ");
	while(trav != NULL)
	{
		print(trav->value,&cmd[cutoff],size);
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		if(trav != NULL)
		{
			printf(" , ");
		}
	}
	printf(" ]");
}

/*
#define ads_parseList(stream,len,parsefunc,type) ads_parseList_ByRef(stream,len,parsefunc,sizeof(type),ads_isAdsType(#type))
static ads_list ads_parseList_ByRef(const char * stream,int len,void*parsefunction(const char * ,int),int datatype,short ads)
{
	int i,start=-1;
	ads_list l = ads_initList(datatype);
	for(i=0;i<len;i++)
	{
		if(stream[i] == '[')
		{
			start = ++i;
			break;
		}
	}
	bool in = false;
	for(i=i;i<len;i++)
	{
		if( ( stream[i] == ',' || stream[i] == ']') && !in )
		{
			void * val = parsefunction(&stream[start],(i-1)-start);
			ads_listAppend_NoCopy(l,val);
			start = i+1;
		}
		if( ads_isStartBracket(stream[i]) == true && !in )
		{
			in = true;
		}
		if( ads_isEndBracket(stream[i]) == true && in )
		{
			in = false;
		}
	}

	return l;
}*/

ads_list ads_listCopy(ads_list _self)
{
	ads_list self ;
	ads_CheckAndCastPointer(_self,ads_list,self);
	ads_list l = ads_initList(ads_tdGetSize(self->dtd));
	ads_listSetTD(l,self->dtd);
	ads_listContainer * trav,*p,*prev = NULL,*next;
	trav = self->head;
	while(trav != NULL)
	{
		p = trav;
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		ads_listAppend_ByRef(l,p->value);
	}
	return l;
}

ads_rt ads_listClear(ads_list self)
{
	ads_listContainer * trav=NULL,*p=NULL,*prev = NULL,*next=NULL;
	trav = self->head;
	while(trav != NULL)
	{
		p = trav;
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		ads_appropriateDelete(self->dtd,p->value) ;
		ads_free(p);
	}
	self->head = NULL;
	self->tail = NULL;
	self->size = 0;
	Adslist_setiterator(self,NULL,NULL,-1);
	return ads_SUCCESS;
}

ads_rt ads_deleteList_NoDelete(ads_list _self)
{
	ads_list self ;
	ads_CheckAndCastPointer(_self,ads_list,self);
	ads_listContainer * trav,*p,*prev = NULL,*next;
	trav = self->head;
	while(trav != NULL)
	{
		p = trav;
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		ads_free(p);
	}
	ads_free(self->dtd);
	ads_free(self);
	return ads_SUCCESS;
}

void ads_listDelete(ads_list _self)
{
	ads_list self ;
	ads_CheckAndCastPointer(_self,ads_list,self);
	ads_listContainer * trav=NULL,*p=NULL,*prev = NULL,*next=NULL;
	trav = self->head;
	while(trav != NULL)
	{
		p = trav;
		next = ads_listXor(trav->next,prev);
		prev = trav;
		trav = next;
		ads_appropriateDelete(self->dtd,p->value) ;
		ads_free(p);
	}
	ads_free(self->dtd);
	ads_free(self);
}

void* ads_listCopy_ByRef_RtnRef(const void*v)
{
    ads_list t ;
    ads_CheckAndCastPointer(*(ads_list*)v,ads_list,t);
    ads_list cv = ads_listCopy(t);
    return ads_toItem(cv);
}

void ads_listDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_list,vl);
    ads_listDelete(vl);
}

ads_td ads_listTDStruct = {ads_listCopy_ByRef_RtnRef,ads_listDelete_ByRef,sizeof(ads_list)};

const ads_td * const ads_listTD = &ads_listTDStruct;

ads_rt ads_listBuilt_FrmArray(ads_list self,void*arr,int n)
{
    if(!self) return ads_registerError("ads_list Not Initialized.");
	int i;char * c = (char*)arr;int datatype = ads_tdGetSize(self->dtd);
	for(i = 0;i<n;i++)
	{
		ads_listAppend_ByRef(self,c);
		c = (c+datatype);
	}
	return ads_SUCCESS;
}

#endif
/** @endcond  */
