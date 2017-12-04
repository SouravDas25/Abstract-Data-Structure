/** @cond ADS_INTERNAL */

#ifndef ADSSET_C
#define ADSSET_C

#include "../include/set.h"

ads_class(ads_set)
{
	ads_init(ads_set);
    uint64 (*reprfunction)(const void*);
	ads_list  * raw ;
	ads_td * dtd;
	uint32_t size;
	uint32_t memory ;
};

ads_linkClass(ads_set,ads_setCopy,ads_setDelete);

ads_set ads_initSet(int datatype,uint64 (*reprfunction) (const void*),int size )
{
	ads_set l = ads_rawObject(ads_set);
	l->reprfunction = reprfunction;
	l->dtd = ads_tdNew(NULL,NULL,datatype);
	l->size = 0;
	l->memory = ads_nextPrime(size);
	l->raw = (ads_list*)ads_calloc(l->memory,sizeof(ads_list));
	if(!ADS_MethodVariableName(ads_set).TypeCode) {
		ADS_MethodVariableName(ads_set).TypeCode = ads_genericIoInsert(ads_printSet,"set(#)",4,5);
		ads_genericIoInsert(ads_printSet,"<#>",1,2);
	}
	return l;
}

void ads_setSetTD(ads_set self,const ads_td*td)
{
    ads_tdCopy(self->dtd,td);
}

static uint64 __sethashed(ads_set self,void * object )
{
	return  self->reprfunction(object);
}

static int __setgetindex(ads_set self,void * object )
{
	uint64 hash =  self->reprfunction(object);
	return hash % self->memory;
}

int ads_setLen(ads_set self)
{
	return self->size;
}

static void __setreallocation(ads_set self)
{
	int newsize = ads_nextPrime((self->memory*2)),i,p,len1 = self->memory ;
	ads_list* data = (ads_list*)ads_calloc(newsize,sizeof(ads_list));
	self->memory = newsize;
	for(i=0;i<len1;i++)
	{
		if(self->raw[i]!=NULL)
		{
			int len = ads_listLen(self->raw[i]);
			for(p=0;p<len;p++)
			{
				void*value = ads_listGetItem_NoCopy_RtnRef(self->raw[i],p);
				int newindex = __setgetindex(self,value);
				if(data[newindex] == NULL)
				{
					data[newindex] = ads_initList( ads_tdGetSize(self->dtd) );
				}
				ads_listAppend_NoCopy(data[newindex],value);
			}
			ads_deleteList_NoDelete(self->raw[i]);
		}
	}
	ads_free(self->raw);
	self->raw = data;
}

ads_rt ads_setAddItem_NoCopy(ads_set self,void * value)
{
	double load = (double) self->size/(double) self->memory;
	if( load >= 0.6)
	{
		__setreallocation(self);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot Add NULL Value to Set in Set add Function");
	}
	int index = __setgetindex(self,value);
	if (self->raw[index] == NULL)
	{
		self->raw[index] = ads_initList( ads_tdGetSize(self->dtd) );
	}
	int len = ads_listLen(self->raw[index]),p,hashnv = __sethashed(self,value);;
	for(p=0;p<len;p++)
	{
		void*val = ads_listGetItem_NoCopy_RtnRef(self->raw[index],p);
		int hashval = __sethashed(self,val);
		if(hashnv == hashval )
		{
			return ads_SUCCESS;
		}
	}
	ads_listAppend_NoCopy(self->raw[index],value);
	self->size++;
	return ads_SUCCESS;
}

ads_rt ads_setAddItem_ByRef(ads_set self,void * value)
{
	double load = (double) self->size/(double) self->memory;
	if( load >= 0.6)
	{
		__setreallocation(self);
	}
	if(value == NULL)
	{
		return ads_registerError("Cannot Add NULL Value to Set in Set add Function");
	}
	int index = __setgetindex(self,value);
	if (self->raw[index] == NULL)
	{
		self->raw[index] = ads_initList( ads_tdGetSize(self->dtd) );
	}
	int len = ads_listLen(self->raw[index]),p;uint64 hashnv = __sethashed(self,value);
	for(p=0;p<len;p++)
	{
		void*val = ads_listGetItem_NoCopy_RtnRef(self->raw[index],p);
		uint64 hashval = __sethashed(self,val);
		if(hashnv == hashval )
		{
			return ads_SUCCESS;
		}
	}
	ads_listAppend_ByRef(self->raw[index],value);
	self->size++;
	return ads_SUCCESS;
}

ads_rt ads_setDiscardItem_ByRef(ads_set self,void * value)
{
	if(value == NULL)
	{
		return ads_registerError("Cannot Delete NULL Value From Set in Set discard Function");
	}
	int index = __setgetindex(self,value);
	if (self->raw[index] != NULL)
	{
		if(ads_listEmpty(self->raw[index]) == false)
		{
			int len = ads_listLen(self->raw[index]),p;uint64 hashnv = __sethashed(self,value);
			for(p=0;p<len;p++)
			{
				void*val = ads_listGetItem_NoCopy_RtnRef(self->raw[index],p);
				uint64 hashval = __sethashed(self,val);
				if(hashnv == hashval )
				{
					ads_listDelItem(self->raw[index],p);
					self->size--;
					return ads_SUCCESS;
				}
			}
		}
	}
	return ads_SUCCESS;
}

bool ads_setContains_ByRef(ads_set self,void * value)
{
	if(value == NULL)
	{
		ads_registerError("Cannot verify NULL Value From Set in Setcontains Function");
		return false;
	}
	int index = __setgetindex(self,value);
	if (self->raw[index] != NULL)
	{
		if(ads_listEmpty(self->raw[index]) == false)
		{
			int len = ads_listLen(self->raw[index]),p;uint64 hashnv = __sethashed(self,value);;
			for(p=0;p<len;p++)
			{
				void*val = ads_listGetItem_NoCopy_RtnRef(self->raw[index],p);
				uint64 hashval = __sethashed(self,val);
				if(hashnv == hashval )
				{
					return true;
				}
			}
		}
	}
	return false;
}

void* ads_setPop_RtnRef(ads_set self)
{
	if(self->size == 0)
	{
		ads_registerError("Cannot pop an Empty set in setpopitem Function");
		return NULL;
	}
	int i,len = self->memory;
	for(i=0;i<len;i++)
	{
		if(self->raw[i]!=NULL)
		{
			if(ads_listEmpty(self->raw[i]) == false)
			{
				void * val = ads_listPop_RtnRef(self->raw[i]);
				self->size--;
				return val;
			}
		}
	}
	ads_registerError("Cannot pop an Empty set in setpopitem Function");
	return NULL;
}

ads_set ads_setCopy(ads_set _self)
{
	ads_set self ;
	ads_CheckAndCastPointer(_self,ads_set,self);
	ads_set s = ads_initSet( ads_tdGetSize(self->dtd) ,self->reprfunction,self->memory );
	ads_tdCopy(s->dtd,self->dtd);
	int i,len = self->memory,p;
	for(i=0;i<len;i++)
	{
		if(self->raw[i]!=NULL)
		{
			int len1 = ads_listLen(self->raw[i]);
			for(p=0;p<len1;p++)
			{
				void*value = ads_listGetItem_NoCopy_RtnRef(self->raw[i],p);
				ads_setAddItem_ByRef(s,value);
			}
		}
	}
	return s;
}

void ads_printSet(void*add,const char * cmd,int size)
{
    add = *(void**)add;
	ads_set self;
	ads_CheckAndCastPointer(add,ads_set,self);
	int i,len = self->memory;bool fir = true;

	int cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint print;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);

	ads_output("Set(");
	for(i=0;i<len;i++)
	{
		if(self->raw[i] != NULL)
		{
			if(ads_listEmpty(self->raw[i]) == false)
			{
				int len1 = ads_listLen(self->raw[i]),p;
				for(p=0;p<len1;p++)
				{
					if(fir == false) ads_output(" , ");
					void*val = ads_listGetItem_NoCopy_RtnRef(self->raw[i],p);
					print(val,&cmd[cutoff],size);
					fir = false;
				}
			}
		}
	}
	ads_output(")\n");
}

void ads_setClear(ads_set self)
{
	int i,len = self->memory;
	for(i=0;i<len;i++)
	{
		if(self->raw[i] != NULL)
		{
			ads_listDelete(self->raw[i]);
			self->raw[i] = NULL;
		}
	}
	self->size =0;
}

void ads_setDelete(ads_set _self)
{
	ads_set self = (ads_set)_self;
	ads_setClear(self);
	ads_free(self->dtd);
	ads_free(self->raw);
	ads_free(self);
}

void* ads_setCopy_ByRef_RtnRef(const void*v)
{
    ads_set t ;
    ads_CheckAndCastPointer(*(ads_set*)v,ads_set,t);
    ads_set cv = ads_setCopy(t);
    return ads_toItem(cv);
}

void ads_setDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_set,vl);
    ads_setDelete(vl);
}

ads_td ads_setTDStruct = {ads_setCopy_ByRef_RtnRef,ads_setDelete_ByRef,sizeof(ads_set)};

const ads_td * const ads_setTD = &ads_setTDStruct;

ads_set ads_setIntersection(ads_set self,ads_set other)
{
	ads_set cs = (ads_set)ads_setCopy(self);
	ads_set rs = ads_initSet( ads_tdGetSize(self->dtd) ,self->reprfunction,self->size);
	ads_tdCopy(rs->dtd,self->dtd);
	while(ads_setLen(cs)>0)
	{
		void*value = ads_setPop_RtnRef(cs);
		if(ads_setContains_ByRef(other,value) == true)
		{
			ads_setAddItem_NoCopy(rs,value);
		}
		else ads_appropriateDelete(cs->dtd,value);
	}
	ads_setDelete(cs);
	return rs;
}

bool ads_setIsSubset(ads_set self,ads_set other)
{
	int i,len = other->memory,p;
	for(i=0;i<len;i++)
	{
		if(other->raw[i]!=NULL)
		{
			int len1 = ads_listLen(other->raw[i]);
			for(p=0;p<len1;p++)
			{
				void*value = ads_listGetItem_NoCopy_RtnRef(other->raw[i],p);
				if (ads_setContains_ByRef(self,value) != true)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool ads_setIsDisjoint(ads_set self,ads_set other)
{
	int i,len = other->memory,p,cou = 0;
	for(i=0;i<len;i++)
	{
		if(other->raw[i]!=NULL)
		{
			int len1 = ads_listLen(other->raw[i]);
			for(p=0;p<len1;p++)
			{
				void*value = ads_listGetItem_NoCopy_RtnRef(other->raw[i],p);
				if (ads_setContains_ByRef(self,value) == true)
				{
					cou++;
				}
			}
		}
	}
	if(cou > 0) return true;
	return false;
}

ads_set ads_setDifference(ads_set self,ads_set other)
{
	int i,len = other->memory,p;
	ads_set rs = ads_initSet( ads_tdGetSize(self->dtd) ,self->reprfunction,self->size);
	ads_tdCopy(rs->dtd,self->dtd);
	for(i=0;i<len;i++)
	{
		if(other->raw[i]!=NULL)
		{
			int len1 = ads_listLen(other->raw[i]);
			for(p=0;p<len1;p++)
			{
				void*value = ads_listGetItem_NoCopy_RtnRef(other->raw[i],p);
				if(ads_setContains_ByRef(self,value) == false)
				{
					ads_setAddItem_ByRef(rs,value);
				}
			}
		}
	}
	return rs;
}

ads_set ads_setUnion(ads_set self,ads_set other)
{
	ads_set rs = (ads_set)ads_setCopy(self);
	int i,len = other->memory,p;
	for(i=0;i<len;i++)
	{
		if(other->raw[i]!=NULL)
		{
			int len1 = ads_listLen(other->raw[i]);
			for(p=0;p<len1;p++)
			{
				void*value = ads_listGetItem_NoCopy_RtnRef(other->raw[i],p);
				ads_setAddItem_ByRef(rs,value);
			}
		}
	}
	return rs;
}

void ads_setBuilt_FrmArray(ads_set self,void*arr,int n)
{
	int i;char * c = (char*)arr;int datatype = ads_tdGetSize(self->dtd);
	for(i = 0;i<n;i++)
	{
		ads_setAddItem_ByRef(self,c);
		c = (c+datatype);
	}
}


#endif
/** @endcond  */
