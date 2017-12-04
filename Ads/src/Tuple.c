

/** @cond ADS_INTERNAL */

#ifndef ADSTUPLE_C
#define ADSTUPLE_C

#include "../include/tuple.h"

typedef struct ads_tupleItem ads_tupleItem;

ads_class(ads_tuple)
{
	ads_init(ads_tuple);
    ads_tupleItem * raw;
    int size ;
    int memory;
};

ads_linkClass(ads_tuple,ads_tupleCopy,ads_tupleDelete);

struct ads_tupleItem {
	void*data;
	ads_td * dtd;
};

ads_tuple ads_initTuple(int size)
{
	ads_tuple l = ads_rawObject(ads_tuple);
	l->raw = (ads_tupleItem*)ads_calloc(size,sizeof(ads_tupleItem));
	l->size = 0;
	l->memory = size;
	int i;
	for(i=0;i<size;i++)
    {
        l->raw[i].dtd = ads_tdNew(NULL,NULL,0);
    }
	if(!ADS_MethodVariableName(ads_tuple).TypeCode) {
		ADS_MethodVariableName(ads_tuple).TypeCode = ads_genericIoInsert(ads_printTuple,"(#,#)",1,2);
		ads_genericIoInsert(ads_printTuple,"(#)",1,2);
		ads_genericIoInsert(ads_printTuple,"tup(#,#)",4,5);
		ads_genericIoInsert(ads_printTuple,"tup(#)",4,5);
	}
	return l;
}

int ads_tupleLen(ads_tuple self)
{
	return self->size;
}

void ads_tupleAppendCStr(ads_tuple self,const char * str)
{
    ads_tupleAppendTD_ByRef(self,ads_cstrTD,&str,sizeof(const char *));
}

ads_rt ads_tupleAppendTD_ByRef(ads_tuple self,const ads_td * td,void * value,int datatype)
{
    int index = self->size;
    if(index>=self->memory)
	{
		return ads_registerError("the tuple is full(current size is %d).",self->size);
	}
    ads_tdCopy(self->raw[index].dtd,td);
    ads_tupleAppend_ByRef(self,value,datatype);
    return ads_SUCCESS;
}

ads_rt ads_tupleAppend_ByRef(ads_tuple self,void * value,int datatype)
{
    int index = self->size;
	if(index>=self->memory)
	{
		return ads_registerError("the tuple has the maximum no of item it can hold.");
	}
	ads_tdSetSize(self->raw[index].dtd,datatype);
	self->raw[index].data = ads_appropriteCopy( self->raw[index].dtd , value );
	self->size++;
	return ads_SUCCESS;
}

void* ads_tupleGetItem_NoCopy_RtnRef(ads_tuple self,int index)
/*returns the address of the data stored at the index*/
{
	if(index >= self->size||index<0)
	{
		ads_registerError(" with index %d ",index);
		return NULL;
	}
	return self->raw[index].data;
}

void* ads_tupleGetItem_Copy_RtnRef(ads_tuple self,int index)
/*returns the address of the copied data stored at the index*/
{
	if(index>=self->size||index<0)
	{
		ads_registerError(" with index %d ",index);
		return NULL;
	}
	void * v =  ads_appropriteCopy(self->raw[index].dtd,self->raw[index].data);
	return v;
}

ads_rt ads_tupleSetItemTD_ByRef(ads_tuple self,int index,const ads_td * td,void*value,int datatype)
{
    if(index>=self->size||index<0)
	{
		return ads_registerError(" with index %d ",index);
	}
	if (self->raw[index].data != NULL )
	{
		ads_appropriateDelete(self->raw[index].dtd,self->raw[index].data);
		self->raw[index].data = NULL;
	}
    ads_tdCopy(self->raw[index].dtd,td);
    ads_tupleSetItem_ByRef(self,index,value,datatype);
    return ads_SUCCESS;
}

ads_rt ads_tupleSetItem_ByRef(ads_tuple self,int index,void*value,int datatype)
/*frees the previous data stored at the index and insert a copy of the original data given*/
{
	if(index>=self->size||index<0)
	{
		return ads_registerError(" with index %d ",index);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot set null value to the ads_tuple");
	}
	if (self->raw[index].data != NULL )
	{
		ads_appropriateDelete(self->raw[index].dtd,self->raw[index].data);
	}
    ads_tdSetSize(self->raw[index].dtd,datatype);
	self->raw[index].data = ads_appropriteCopy( self->raw[index].dtd , value );
	return ads_SUCCESS;
}

void ads_tupleSetItemCStr(ads_tuple self,int index,const char * str)
{
    ads_tupleSetItemTD_ByRef(self,index,ads_cstrTD,&str,sizeof(const char *));
}

ads_tuple ads_tupleConcat(ads_tuple t1,ads_tuple t2)
{
    int size = ads_tupleLen(t1)+ads_tupleLen(t2);
    ads_tuple n = ads_initTuple(size);
	int i,cnt = 0;
	for(i=0;i<ads_tupleLen(t1);i++)
	{
		n->raw[cnt].data = ads_appropriteCopy(t1->raw[i].dtd,t1->raw[i].data);
        ads_tdCopy(n->raw[cnt++].dtd,t1->raw[i].dtd);
	}
	for(i=0;i<ads_tupleLen(t2);i++)
	{
		n->raw[cnt].data = ads_appropriteCopy(t2->raw[i].dtd,t2->raw[i].data);
        ads_tdCopy(n->raw[cnt++].dtd,t2->raw[i].dtd);
	}
	n->size = size;
	return n;
}

void ads_printTuple(void*add,const char * cmd,int size)
{
    add= *(void**)add;
	ads_tuple self ;
	ads_CheckAndCastPointer(add,ads_tuple,self);
	int i,l = ads_tupleLen(self);

	int cutoff=0;
	ads_iList l2 = ads_getPrintFunction(cmd,&size,&cutoff);
	if(ads_iListLen(l2) > l && ads_iListLen(l2) > 0)
	{
		ads_output("( Incorrect Formatting )");
		ads_deleteIList(l2);
		return ;
	}
	ads_genericPrint print;

	l = (l<ads_iListLen(l2))?l:ads_iListLen(l2);
	ads_output("(");
	for(i=0;i<l;i++)
	{
		ads_iListGetItem(l2,i,ads_genericPrint,print);
		print(self->raw[i].data,&cmd[cutoff],size);
		if(i!=l-1)
		{
			ads_output(" , ");
		}
	}
	ads_output(")");
	ads_deleteIList(l2);
}

ads_tuple ads_tupleCopy(ads_tuple  _self)
{
	ads_tuple self ;
	ads_CheckAndCastPointer(_self,ads_tuple,self);
	ads_tuple n = ads_initTuple(self->memory);
	int i,l = ads_tupleLen(self);
	for(i=0;i<l;i++)
	{
		n->raw[i].data = ads_appropriteCopy(self->raw[i].dtd,self->raw[i].data);
        ads_tdCopy(n->raw[i].dtd,self->raw[i].dtd);
	}
    n->size = ads_tupleLen(self);
	return n;
}

void ads_tupleDelete(ads_tuple _self)
{
	ads_tuple self ;
	ads_CheckAndCastPointer(_self,ads_tuple,self);
	int i,l = ads_tupleLen(self);
	for(i=0;i<self->memory;i++)
	{
		if(i<l)ads_appropriateDelete(self->raw[i].dtd,self->raw[i].data);
		ads_free(self->raw[i].dtd);
	}
	ads_free(self->raw);
	ads_free(self);
}

void* ads_tupleCopy_ByRef_RtnRef(const void*v)
{
    ads_tuple t ;
    ads_CheckAndCastPointer(*(ads_tuple*)v,ads_tuple,t);
    ads_tuple cv = ads_tupleCopy(t);
    return ads_toItem(cv);
}

void ads_tupleDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_tuple,vl);
    ads_tupleDelete(vl);
}

ads_td ads_tupleTDStruct = {ads_tupleCopy_ByRef_RtnRef,ads_tupleDelete_ByRef,sizeof(ads_tuple)};

const ads_td * const ads_tupleTD = &ads_tupleTDStruct;

#endif

/** @endcond  */
