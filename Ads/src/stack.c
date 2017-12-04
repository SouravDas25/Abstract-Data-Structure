
/** @cond ADS_INTERNAL */


#ifndef ADS_Stck_C
#define ADS_Stck_C

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/stack.h"

ads_class(ads_stack)
{
	ads_init(ads_stack);
    ads_stackContainer * head;
    ads_td * dtd;
	unsigned int size;
};

ads_linkClass(ads_stack,ads_stackCopy,ads_stackDelete);

struct ads_stackContainer
{
	void * value;
	struct ads_stackContainer * next;
};

ads_stack ads_initStack(int datatype )
{
	ads_stack  l = ads_rawObject(ads_stack);
	ads_checkMemory(l);
	l->head = NULL;
	l->size = 0;
	l->dtd = ads_tdNew(NULL,NULL,datatype);
	if(!ADS_MethodVariableName(ads_stack).TypeCode) {
		ADS_MethodVariableName(ads_stack).TypeCode = ads_genericIoInsert(ads_printStack,"stk(#)",4,5);
	}
	return l;
}

void ads_stackSetTD(ads_stack self,const ads_td * td)
{
    ads_tdCopy(self->dtd,td);
}

void ads_stackPush_ByRef(ads_stack self,void * value)
/*insert a new element in the ads_stack (makes a copy of the data and stores the copy dosn't do any thing with the original)
Adds the new data to head and increment size */
{
	ads_stackContainer * l = (ads_stackContainer*)ads_calloc(1,sizeof(ads_stackContainer));
	ads_checkMemory(l);
	l->value = ads_appropriteCopy(self->dtd,value);
	l->next = self->head;
	self->head = l;
	self->size += 1;
}

void* ads_stackPop_RtnRef(ads_stack self)
/*removes a element from the ads_stack and return it*/
{
	ads_stackContainer * trav = self->head;
	if(trav == NULL)
	{
		ads_registerError("Can't pop an empty ads_stack");
		return NULL;
	}
	self->head = self->head->next;
	self->size -= 1;
	void* v = trav->value;
	ads_free(trav);
	return v;
}

void* ads_stackFront_Copy_RtnRef(ads_stack self)
{
	return ads_appropriteCopy(self->dtd,self->head->value);
}

int ads_stackLen(ads_stack  self)
{
	return self->size;
}

void ads_printStack(void*add,const char * cmd,int size)
{
    add = *(void**)add;
	ads_stack self ;
	ads_CheckAndCastPointer(add,ads_stack,self);
	ads_stackContainer * trav = self->head;

	int cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint print;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);

	ads_output("ads_stack( ");
	while(trav != NULL)
	{
		print(trav->value,&cmd[cutoff],size);
		if(trav->next != NULL)
		{
			ads_output(" , ");
		}
		trav = trav->next;
	}
	ads_output(" )");
}

bool ads_stackIsEmpty(ads_stack self)
{
	if(self->head == NULL) return true;
	return false;
}

void ads_stackReverse(ads_stack self)
{
	ads_stackContainer * trav = self->head,*previous = NULL,*next;
	while(trav != NULL)
	{
		next = trav->next;
		trav->next = previous;
		previous = trav;
		trav = next;
	}
	self->head = previous;
}

void* __stackiterstart(ads_iterator* i)
{
	ads_stack s = (ads_stack)i->object;
	i->object = s->head;
	return ((ads_stackContainer*)i->object)->value;
}

void* __stackiternext(ads_iterator* i)
{
	if(i->object != NULL)
	{
		i->object = ((ads_stackContainer*)i->object)->next;
		if(i->object != NULL) return ((ads_stackContainer*)i->object)->value;
		return NULL;
	}
	ads_registerError("OverFlow in Stack iterator");
	return NULL;
}

ads_iterator ads_stackIterator(ads_stack self)
{
	ads_iterator i = ads_initIterator(self,__stackiternext,__stackiterstart,ads_stackLen(self),NULL);
	return i;
}


ads_stack ads_stackCopy(ads_stack _self)
{
	ads_stack self ;
	ads_CheckAndCastPointer(_self,ads_stack,self);
	ads_stack nwstck = ads_initStack( ads_tdGetSize(self->dtd) );
    ads_tdCopy(nwstck->dtd,self->dtd);
	ads_stackContainer * trav = self->head;
	while(trav != NULL)
	{
		ads_stackPush_ByRef(nwstck,trav->value);
		trav = trav->next;
	}
	ads_stackReverse(nwstck);
	return nwstck;
}

void ads_stackDelete(ads_stack _self)
{
	ads_stack self ;
	ads_CheckAndCastPointer(_self,ads_stack,self);
	int i ;ads_stackContainer * trav = self->head,*p;
	for(i=0;trav != NULL;i++)
	{
		p = trav;
		trav = trav->next;
		ads_appropriateDelete(self->dtd,p->value);
		ads_free(p);
	}
	ads_free(self->dtd);
	ads_free(self);
}

void* ads_stackCopy_ByRef_RtnRef(const void*v)
{
    ads_stack t ;
    ads_CheckAndCastPointer(*(ads_stack*)v,ads_stack,t);
    ads_stack cv = ads_stackCopy(t);
    return ads_toItem(cv);
}

void ads_stackDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_stack,vl);
    ads_stackDelete(vl);
}

ads_td ads_stackTDStruct = {ads_stackCopy_ByRef_RtnRef,ads_stackDelete_ByRef,sizeof(ads_stack)};

const ads_td * const ads_stackTD = &ads_stackTDStruct;

void ads_stackBuilt_FrmArray(ads_stack self,void*arr,int n)
{
	int i;char * c = (char*)arr;int datatype = ads_tdGetSize(self->dtd);
	for(i = 0;i<n;i++)
	{
		ads_stackPush_ByRef(self,c);
		c = (c+datatype);
	}
}

#endif
/** @endcond  */
