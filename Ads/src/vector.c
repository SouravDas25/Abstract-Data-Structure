
/**< @cond ADS_INTERNAL */

#ifndef ADSVECTOR_C
#define ADSVECTOR_C


#include "../include/vector.h"

typedef struct {
	void*data;
} ads_vectorItem ;

ads_class(ads_vector)
{
	ads_init(ads_vector);
    ads_vectorItem* raw;
    ads_td* dtd;
    unsigned short size;
    unsigned short memory;
};

ads_linkClass(ads_vector,ads_vectorCopy,ads_vectorDelete);


ads_vector ads_initVector(int typesize,int size)
{
	ads_vector l = ads_rawObject(ads_vector);
	if(!l)return NULL;
	size = size + 1;
	l->dtd = ads_tdNew(NULL,NULL,typesize);
	l->memory = size;
	l->raw = (ads_vectorItem*)ads_calloc(size,sizeof(ads_vectorItem));
	l->size = 0;
	if(!ADS_MethodVariableName(ads_vector).TypeCode) { /*adding the printtype and adstypes at runtime instead of compile time*/
		ADS_MethodVariableName(ads_vector).TypeCode = ads_genericIoInsert(ads_printVector,"vec(#)",4,5);
		ads_genericIoInsert(ads_printVector,"[#]",1,2);
	}
	return l;
}

ads_td* ads_vectorGetTD(ads_vector self)
{
    return self->dtd;
}

ads_rt ads_vectorSetTD(ads_vector self,const ads_td * td)
{
    if(ads_vectorLen(self) > 0 )
    {
        return ads_registerError("Inserting Type-Defination of a Vector After adding Elements.");
    }
    ads_tdCopy(self->dtd,td);
    return ads_SUCCESS;
}

int ads_vectorLen(ads_vector self)
{
    if(!self)return 0;
	return self->size;
}

int ads_vectorMemory(ads_vector self)
{
    if(!self)return 0;
	return self->memory;
}

ads_rt ads_vectorRellocate(ads_vector self, int newsize)
{
    if( ads_vectorLen(self) >= newsize )
    {
        return ads_SUCCESS;
    }
    ads_vectorItem* tmp;
    tmp = (ads_vectorItem*)ads_calloc(newsize,sizeof(ads_vectorItem));
	if(tmp==NULL)
	{
		return ads_registerError("In ads_vector Reallocte Function");
	}
	tmp = (ads_vectorItem*)memcpy(tmp,self->raw,self->size*sizeof(ads_vectorItem));
	ads_free(self->raw);
	self->raw = tmp;
	self->memory = newsize;
	return ads_SUCCESS;
}

static void ads_vectorRellocateInternal(ads_vector self)
{
	int newsize;
	if(ads_vectorLen(self)<5)
	{
		newsize = ads_vectorLen(self)*3.12;
	}
	else if(ads_vectorLen(self)>100)
	{
		newsize = ads_vectorLen(self)*1.5;
	}
	else
	{
		newsize = ads_vectorLen(self)*2.15;
	}
    ads_vectorRellocate(self,newsize);
}

ads_rt ads_vectorAppend_NoCopy(ads_vector self,void * value)
{
	int index = self->size;
	if(index>=self->memory-1)
	{
		ads_vectorRellocateInternal(self);
	}
	self->raw[index].data = value;
	self->size++;
	return ads_SUCCESS;
}

ads_rt ads_vectorAppend_ByRef(ads_vector self,void * value)
{
	void * val = ads_appropriteCopy(self->dtd,value);
	return ads_vectorAppend_NoCopy(self,val);
}

void* ads_vectorGetItem_NoCopy_RtnRef(ads_vector self,int index)
{
	if(index>=self->size||index<0)
	{
		ads_registerError("in vectorgetitem function with index %d ",index);
		return NULL;
	}
	return self->raw[index].data;
}

void* ads_vectorGetItem_Copy_RtnRef(ads_vector self,int index)
{
	void * v;
	if(index>=self->size||index<0)
	{
		ads_registerError("in ads_vector getitem function with index %d ",index);
		return NULL;
	}
	v =  ads_appropriteCopy(self->dtd,self->raw[index].data);
	return v;
}

ads_rt ads_vectorSetItem_ByRef(ads_vector self,int index,void*value)
{
	if(index>=self->size||index<0)
	{
		return ads_registerError("in vectorgetlength function with index %d ",index);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot ads_set null value to the ads_vector");
	}
	if(self->raw[index].data != NULL )
	{
		ads_appropriateDelete(self->dtd,self->raw[index].data);
	}
	self->raw[index].data = ads_appropriteCopy(self->dtd,value);
	return ads_SUCCESS;
}

ads_rt ads_vectorInsert_ByRef(ads_vector self,int index,void*value)
{
	int i;
	if(index>=self->size||index<0)
	{
		return ads_registerError("in vectorgetlength function with index %d ",index);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot ads_set null value to the ads_vector");
	}
	if(self->size >=self->memory-1)
	{
		ads_vectorRellocateInternal(self);
	}
	for(i=self->size-1;i>=index;i--)
	{
		self->raw[i+1].data = self->raw[i].data;
		self->raw[i].data = NULL;
	}
	ads_vectorSetItem_ByRef(self,index,value);
	self->size++;
	return ads_SUCCESS;
}

ads_rt ads_vectorExtend(ads_vector self,ads_vector other)
{
	int le,i;
	if(!ads_tdCmp(self->dtd,other->dtd))
	{
		return ads_registerError("Type mismatch of two ads_vector in ads_vector extend");
	}
	le = ads_vectorLen(other);
	for(i=0;i<le;i++)
	{
		ads_vectorAppend_ByRef(self,ads_vectorGetItem_NoCopy_RtnRef(other,i));
	}
	return ads_SUCCESS;
}

ads_rt ads_vectorSwap(ads_vector self,int index1,int index2)
{
	void *tmp;
	if(index1>=self->size||index1<0)
	{
		return ads_registerError("in vectorgetlength function with index %d ",index1);
	}
	if(index2>=self->size||index2<0)
	{
		return ads_registerError("in vectorgetlength function with index %d ",index2);
	}
	tmp = self->raw[index1].data;
	self->raw[index1].data = self->raw[index2].data;
	self->raw[index2].data = tmp;
	return ads_SUCCESS;
}


ads_rt ads_vectorReverse(ads_vector self)
{
	int size = ads_vectorLen(self)-1,i;
	for(i=0; i <= (size-1)>>1 ;i++)
	{
		ads_vectorSwap(self,i,size-i);
	}
	return ads_SUCCESS;
}

ads_rt ads_vectorDelItem(ads_vector self,int index)
{
	int i;
	if(index>=self->size||index<0)
	{
		return ads_registerError("in vectorgetitem function with index %d ",index);
	}
	ads_appropriateDelete(self->dtd,self->raw[index].data);
	i = index;
	for(i=index;i<self->size;i++)
	{
		self->raw[i].data = self->raw[i+1].data;
		self->raw[i+1].data = NULL;
	}
	self->size--;
	return ads_SUCCESS;
}

void* ads_vectorPop_RtnRef(ads_vector self)
{
	void* v = ads_vectorGetItem_Copy_RtnRef(self,0);
	ads_vectorDelItem(self,0);
	return v;
}

ads_rt ads_vectorShrink(ads_vector self)
{
	int newsize = ads_vectorLen(self);
	if(!newsize) newsize = 1;
	ads_vectorItem* tmp = (ads_vectorItem*)ads_calloc(newsize,sizeof(ads_vectorItem));
	if(tmp==NULL)
	{
		return ads_registerError("In List Shrink Function");
	}
	tmp = (ads_vectorItem*)memcpy(tmp,self->raw,self->size*sizeof(ads_vectorItem));
	ads_free(self->raw);
	self->raw = tmp;
	self->memory = newsize;
	return ads_SUCCESS;
}

void ads_vectorMap(ads_vector self,void(*function)(void*value))
{
	int i,l = ads_vectorLen(self);
	for(i=0;i<l;i++)
	{
		function(ads_vectorGetItem_NoCopy_RtnRef(self,i));
	}
}

static void* ads_vectorIterStart(ads_iterator* i)
{
	ads_vector s = (ads_vector)i->object;
	return ads_vectorGetItem_NoCopy_RtnRef(s,i->count);
}

static void* ads_vectorIterNext(ads_iterator* i)
{
	ads_vector s = (ads_vector)i->object;
	i->max = (unsigned)ads_vectorLen(s);
	if((unsigned)ads_vectorLen(s)>i->count)
	{
		return ads_vectorGetItem_NoCopy_RtnRef(s,i->count);
	}
	return NULL;;
}

ads_iterator ads_vectorIterator(ads_vector self)
{
	ads_iterator i = ads_initIterator(self,ads_vectorIterNext,ads_vectorIterStart,ads_vectorLen(self),NULL);
	return i;
}

void ads_printVector(void*add,const char * cmd,int size)
{
	ads_vector self ;
	add = *(void**)add;
	ads_CheckAndCastPointer(add,ads_vector,self);
	int i,len = ads_vectorLen(self),cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint print;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);
	ads_output("[ ");
	for(i=0;i<len;i++)
	{
		print(ads_vectorGetItem_NoCopy_RtnRef(self,i),&cmd[cutoff],size);
		if(i!=len-1)
		{
			ads_output(" , ");
		}
	}
	ads_output(" ]");
}

ads_vector ads_vectorCopy( ads_vector l)
{
	ads_vector self;
	ads_CheckAndCastPointer(l,ads_vector,self);
	ads_vector nwlst = ads_initVector(ads_tdGetSize(self->dtd),self->size);
    ads_tdCopy(nwlst->dtd,self->dtd);
	int len = ads_vectorLen(self),i ;
	for(i=0;i<len;i++)
	{
		ads_vectorAppend_ByRef(nwlst,ads_vectorGetItem_NoCopy_RtnRef(self,i));
	}
	return nwlst;
}

ads_rt ads_vectorClear(ads_vector l )
{
	ads_vector self ;
	ads_CheckAndCastPointer(l,ads_vector,self);
	int i ;
	for(i=0;i<self->size;i++)
	{
		ads_appropriateDelete(self->dtd,self->raw[i].data);
		self->raw[i].data = NULL;
	}
	l->size = 0;
	return ads_SUCCESS;
}

void ads_vectorDelete(ads_vector l )
{
	ads_vector self ;
	ads_CheckAndCastPointer(l,ads_vector,self);
	int i ;
	for(i=0;i<self->size;i++)
	{
		ads_appropriateDelete(self->dtd,self->raw[i].data);
	}
	ads_free(self->raw);
	ads_free(self->dtd);
	ads_free(self);
}

void* ads_vectorCopy_ByRef_RtnRef(const void*v)
{
    ads_CheckAndCastPointer(*(ads_vector*)v,ads_vector,*(ads_vector*)v);
    ads_vector cv = ads_vectorCopy(*(ads_vector*)v);
    return ads_toItem(cv);
}

void ads_vectorDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_vector,vl);
    ads_vectorDelete(vl);
}

int ads_vectorIndex_ByRef(ads_vector self,void *value,int (*compare)(const void*,const void*))
{
	int i ;
	for(i=0;i<self->size;i++)
	{
		int c = ads_appropriateCmp(compare,value,ads_vectorGetItem_NoCopy_RtnRef(self,i),ads_tdGetSize(self->dtd));
		if(c == 0)
		{
			return i;
		}
	}
	return -1;
}

int ads_vectorCount_ByRef(ads_vector self,void * value,int (*compare)(const void*,const void*))
{
	int i,cou=0 ;
	for(i=0;i<self->size;i++)
	{
		int c = ads_appropriateCmp(compare,value,ads_vectorGetItem_NoCopy_RtnRef(self,i),ads_tdGetSize(self->dtd));
		if(c == 0)
		{
			cou++;
		}
	}
	return cou;
}

ads_rt ads_vectorRemove_ByRef(ads_vector self,void * value,int (*compare)(const void*,const void*))
{
	int i ;
	for(i=0;i<self->size;i++)
	{
		int c ;
		c = ads_appropriateCmp(compare,value,ads_vectorGetItem_NoCopy_RtnRef(self,i),ads_tdGetSize(self->dtd));
		if(c == 0)
		{
			ads_vectorDelItem(self,i);
			i--;
		}
	}
	return ads_SUCCESS;
}

ads_rt ads_vectorSort(ads_vector self,int (*compare)(const void*,const void*))
{
	int i,p;void * curr;
    for(i=1;i<self->size;i++)
    {
        curr = self->raw[i].data ;
        for(p=i-1;p>=0;p--)
        {
        	int c = ads_appropriateCmp(compare,self->raw[p].data,curr,ads_tdGetSize(self->dtd));
            if(c == -1)
            break;
            self->raw[p+1].data = self->raw[p].data;
        }
        self->raw[p+1].data = curr;
    }
    return ads_SUCCESS;
}

int ads_vectorBinarySearch_ByRef(ads_vector self,void * value ,int (*compare)(const void*,const void*) )
{
    ads_vectorSort(self,compare);
    int high = self->size-1,low=0,index;
    int c1 = compare(value,self->raw[high].data),
    c2 = compare(value,self->raw[low].data);
    if(c1 > 0 || c2 < 0) return -1;
    if(c1 == 0) return high;
    if(c2 == 0) return low;
    while(low<=high)
    {
        index = (high+low)>>1;
        c1 = compare(value,self->raw[index].data);
        if (c1 == 0 ) return index;
        else if(c1 < 0) high = index-1;
        else if(c1 > 0) low = index+1;
    }
    return -1;
}
/*
ads_vector ads_vectorParse_ByRef(const char * stream,int len,void*parsefunction(const char * ,int),int datatype)
{
	int i,start=-1;
	ads_vector l = ads_initVector(datatype,5,NULL);
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
			void * val = parsefunction(&stream[start],i-start);
			ads_vectorAppend_NoCopy(l,val);
			start = i+1;
		}
		if( ads_isStartBracket(stream[i]) == true && !in)
		{
			in = true;
		}
		if( ads_isEndBracket(stream[i]) == true && in)
		{
			in = false;
		}
	}
	ads_vectorShrink(l);
	return l;
}*/

void ads_vectorBuilt_FrmArray(ads_vector self,void*arr,int n)
{
	int i;void * c = arr;int datatype = ads_tdGetSize(self->dtd);
	for(i = 0;i<n;i++)
	{
		ads_vectorAppend_ByRef(self,c);
		c = (c+datatype);
	}
}

ads_td ads_vectorTDStruct = {ads_vectorCopy_ByRef_RtnRef,ads_vectorDelete_ByRef,sizeof(ads_vector)};

const ads_td * const ads_vectorTD = &ads_vectorTDStruct;

#endif
/** @endcond  */
