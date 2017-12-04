

/** @cond ADS_INTERNAL */

#ifndef ADSqueue_C
#define ADSqueue_C

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/queue.h"

ads_class(ads_queue)
{
	ads_init(ads_queue);
    ads_queueContainer * head;
	ads_queueContainer * tail;
	ads_td * dtd;
	uint32_t size;
};

ads_linkClass(ads_queue,ads_queueCopy,ads_queueDelete);

struct ads_queueContainer
{
	void * value;
	struct ads_queueContainer * next;
};

ads_queue ads_initQueue(int datatype)
{
	ads_queue  l = ads_rawObject(ads_queue);
	ads_checkMemory(l);
	l->head = NULL;
	l ->tail = NULL;
	l->size = 0;
	l->dtd = ads_tdNew(NULL,NULL,datatype);
	if(!ADS_MethodVariableName(ads_queue).TypeCode) {
		ADS_MethodVariableName(ads_queue).TypeCode = ads_genericIoInsert(ads_printQueue,"que(#)",4,5);
	}
	return l;
}

void ads_queueSetTD(ads_queue self,const ads_td * td)
{
    ads_tdCopy(self->dtd,td);
}

void ads_queueInsert_ByRef(ads_queue self,void * value)
{
	ads_queueContainer * l = (ads_queueContainer *)ads_calloc(1,sizeof(ads_queueContainer));
	ads_checkMemory(l);
	l->value = ads_appropriteCopy(self->dtd,value);
	l->next = NULL;
	if(self->head == NULL)
	{
		self->head = l;
		self->tail = l;
	}
	else
	{
		self->tail->next = l;
		self->tail = l;
	}
	self->size += 1;
}

void* ads_queuePop_RtnRef(ads_queue self)
{
	ads_queueContainer * trav = self->head;
	if(trav == NULL)
	{
		ads_registerError("Can't pop an empty ads_queue");
        return NULL;
	}
	self->head = self->head->next;
	self->size -= 1;
	void* v =trav->value;
	if(self->head == NULL)
	{
		self->tail = NULL;
	}
	ads_free(trav);
	return v;
}

void* ads_queueFront_Copy_RtnRef(ads_queue self)
{
	return ads_appropriteCopy(self->dtd,self->head->value);
}

bool ads_queueIsEmpty(ads_queue self)
{
	if(self->head == NULL)return true;
	return false;
}

int ads_queueLen(ads_queue self)
{
	return self->size;
}

void* __queueiterstart(ads_iterator* i)
{
	ads_queue q = (ads_queue)i->object;
	i->object = q->head;
	return ((ads_queueContainer*)i->object)->value;
}

void*  __queueiternext(ads_iterator* i)
{
	if(i->object != NULL)
	{
		i->object = ((ads_queueContainer*)i->object)->next;
		if(i->object != NULL) return ((ads_queueContainer*)i->object)->value;
		return NULL;
	}
	ads_registerError("ITERATOR OVERFLOW ERROR in Queueiterator");
	return NULL;
}

ads_iterator ads_queueIterator(ads_queue self)
{
	ads_iterator i = ads_initIterator(self,__queueiternext,__queueiterstart,ads_queueLen(self),NULL);
	return i;
}

void ads_printQueue(void*add,const char * cmd,int size)
{
    add = *(void**)add;
	ads_queue self;
	ads_CheckAndCastPointer(add,ads_queue,self);
	ads_queueContainer * trav = self->head;

	int cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint print ;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);

	ads_output("ads_queue( ");
	while(trav != NULL )
	{
		print(trav->value,&cmd[cutoff],size);
		trav = trav->next;
		if(trav != NULL) ads_output(" , ");
	}
	ads_output(" )");
}

ads_queue ads_queueCopy(ads_queue _self)
{
	ads_queue self ;
	ads_CheckAndCastPointer(_self,ads_queue,self);
	ads_queue q = ads_initQueue( ads_tdGetSize(self->dtd) );
    ads_tdCopy(q->dtd,self->dtd);
	void *val;ads_queueContainer * trav = self->head;
	while(trav != NULL )
	{
		val = trav->value;
		ads_queueInsert_ByRef(q,val);
		trav = trav->next;
	}
	return q;
}

void ads_queueDelete(ads_queue _self)
{
	ads_queue self ;
	ads_CheckAndCastPointer(_self,ads_queue,self);
	ads_queueContainer * trav = self->head,*p;
	while(trav != NULL)
	{
		p = trav;
		trav = trav->next;
		ads_appropriateDelete(self->dtd,p->value);
		ads_free(p);
	}
	ads_free(self->dtd);
	ads_free(self);
}

void* ads_queueCopy_ByRef_RtnRef(const void*v)
{
    ads_queue t ;
    ads_CheckAndCastPointer(*(ads_queue*)v,ads_queue,t);
    ads_queue cv = ads_queueCopy(t);
    return ads_toItem(cv);
}

void ads_queueDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_queue,vl);
    ads_queueDelete(vl);
}

ads_td ads_queueTDStruct = {ads_queueCopy_ByRef_RtnRef,ads_queueDelete_ByRef,sizeof(ads_queue)};

const ads_td * const ads_queueTD = &ads_queueTDStruct;

void ads_queueBuilt_FrmArray(ads_queue self,void*arr,int n)
{
	int i;char * c = (char*)arr;int datatype = ads_tdGetSize(self->dtd);
	for(i = 0;i<n;i++)
	{
		ads_queueInsert_ByRef(self,c);
		c = (c+datatype);
	}
}



#endif
/** @endcond  */
