
/** @cond ADS_INTERNAL */

#ifndef ADSDequeue_C
#define ADSDequeue_C

#include "../include/deque.h"

ads_class(ads_deque)
{
	ads_init(ads_deque);
    ads_list __ldeque;
};

ads_linkClass(ads_deque,ads_dequeCopy,ads_dequeDelete);

ads_deque ads_initDeque(int datatype)
{
	ads_deque q = ads_rawObject(ads_deque);
	q->__ldeque = ads_initList(datatype);
	if(!ADS_MethodVariableName(ads_deque).TypeCode) {
		ADS_MethodVariableName(ads_deque).TypeCode = ads_genericIoInsert(ads_printDeque,"deq(#)",4,5);
	}
	return q;
}

void ads_dequeSetTD(ads_deque self,const ads_td * td)
{
    ads_listSetTD(self->__ldeque,td);
}

void ads_dequePush_ByRef(ads_deque self,void*value)
{
	ads_listAppend_ByRef(self->__ldeque,value);
}

void ads_dequePushFront_ByRef(ads_deque self,void*value)
{
	ads_listInsert_ByRef(self->__ldeque,0,value);
}

void* ads_dequePopFront_RtnRef(ads_deque self)
{
	return ads_listPop_RtnRef(self->__ldeque);
}

void* ads_dequePop_RtnRef(ads_deque self)
{
	return ads_listPopItem_RtnRef(self->__ldeque,ads_listLen(self->__ldeque)-1);
}

void* ads_dequeFront_Copy_RtnRef(ads_deque self)
{
	return ads_listGetItem_Copy_RtnRef(self->__ldeque,0);
}

void* ads_dequeEnd_Copy_RtnRef(ads_deque self)
{
	return ads_listGetItem_Copy_RtnRef(self->__ldeque,ads_listLen(self->__ldeque)-1);
}

bool ads_dequeIsEmpty(ads_deque self)
{
	return ads_listEmpty(self->__ldeque);
}

int ads_dequeLen(ads_deque self)
{
	return ads_listLen(self->__ldeque);
}

ads_iterator ads_dequeIterator(ads_deque self)
{
	return ads_listIterator(self->__ldeque);
}

void ads_printDeque(void*add,const char * cmd,int size)
{
	ads_deque self ;
	ads_listContainer * trav,*prev = NULL,*next;
	int cutoff;
	add = *(void**)add;
	ads_CheckAndCastPointer(add,ads_deque,self);
	trav = ads_linklistHead(self->__ldeque);

	cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint print ;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);

	ads_output("ads_deque( ");
	while(trav != NULL)
	{
		print(ads_listContainerValue(trav),&cmd[cutoff],size);
		next = ads_listXor(ads_listContainerNext(trav),prev);
		prev = trav;
		trav = next;
		if(trav != NULL)
		{
			ads_output(" , ");
		}
	}
	ads_output(" )");
}

ads_deque ads_dequeCopy( ads_deque _self)
{
	ads_deque self,q ;
	ads_CheckAndCastPointer(_self,ads_deque,self);

	q = ads_rawObject(ads_deque);
	q->__ldeque = (ads_list)ads_listCopy(self->__ldeque);
	return q;
}

void ads_dequeDelete(ads_deque _self)
{
	ads_deque self ;
	ads_CheckAndCastPointer(_self,ads_deque,self);
	ads_listDelete(self->__ldeque);
	ads_free(self);
}

static void* ads_dequeCopy_ByRef_RtnRef(const void*v)
{
    ads_deque t ;
    ads_CheckAndCastPointer(*(ads_deque*)v,ads_deque,t);
    ads_deque cv = ads_dequeCopy(t);
    return ads_toItem(cv);
}

static void ads_dequeDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_deque,vl);
    ads_dequeDelete(vl);
}

ads_td ads_dequeTDStruct = {ads_dequeCopy_ByRef_RtnRef,ads_dequeDelete_ByRef,sizeof(ads_deque)};

const ads_td * const ads_dequeTD = &ads_dequeTDStruct;

void ads_dequeBuilt_FrmArray(ads_deque self,void*arr,int n)
{
	ads_listBuilt_FrmArray(self->__ldeque,arr,n);
}

#endif

/** @endcond */
