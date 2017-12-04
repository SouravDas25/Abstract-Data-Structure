


/** @cond ADS_INTERNAL */


#ifndef ADSDict_C
#define ADSDict_C


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>



#include "../include/dict.h"

ads_class(ads_dict)
{
	ads_init(ads_dict);  /*16 bytes*/
	ads_list * raw; /*8 bytes*/
    uint64_t (*hashfunction)(const void*); /*8 bytes*/
    ads_td*dtd;
    ads_td*ktd;
	uint32_t nfkeys ; /*4 bytes*/
 	uint32_t memory ; /*4 bytes*/
 	uint32_t buckets;
};

ads_linkClass(ads_dict,ads_dictCopy,ads_dictDelete);

struct AdsDictPair {
	void* key;
	void* data;
};

typedef struct AdsDictPair AdsDictPair;

ads_dict ads_initDict(int keytype,uint64_t (*hashfunction)(const void*),int datatype,int size)
{
	ads_dict d = ads_rawObject(ads_dict);
	size = ads_nextPrime(abs(size));
	d->raw = (ads_list*)ads_calloc(size,sizeof(ads_list));
 	d->memory = size;
	d->nfkeys = 0;
	d->dtd = ads_tdNew(NULL,NULL,datatype);
	d->ktd = ads_tdNew(NULL,NULL,keytype);
	d->hashfunction = hashfunction;
	d->buckets =0;
	if(!ADS_MethodVariableName(ads_dict).TypeCode) { /*adding the printtype and adstypes at runtime instead of compile time*/
		ADS_MethodVariableName(ads_dict).TypeCode = ads_genericIoInsert(ads_printDict,"dict(#:#)",5,6);
		ads_genericIoInsert(ads_printDict,"[#:#]",1,2);
	}
	return d;
}


void ads_dictSetDataTD(ads_dict self,const ads_td * td)
{
    ads_tdCopy(self->dtd,td);
}

void ads_dictSetKeyTD(ads_dict self,const ads_td * td)
{
    ads_tdCopy(self->ktd,td);
}

int ADS_dictgetindex(void*key,uint64_t (*hashfunction)(const void*),int capacity)
{
	int index = hashfunction(key)%capacity;
	return index;
}

void ADS_dictreallocation(ads_dict self)
{
	void * tmpkey;
	AdsDictPair tmpADP;
	uint_fast32_t newsize = ads_nextPrime((self->memory*2)),i,p;
	ads_list* h = (ads_list*)ads_calloc(newsize,sizeof(ads_list));
	self->buckets =0;
	for(i=0; i < self->memory ;i++)
	{
		if(self->raw[i]!=NULL)
		{
			for(p=0; p < ads_listLen(self->raw[i]) ;p++)
			{
				ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
				tmpkey = tmpADP.key;
				int newindex = ADS_dictgetindex(tmpkey,self->hashfunction,newsize);
				if(h[newindex] == NULL)
				{
					h[newindex] = ads_listNew(AdsDictPair);
					self->buckets++;
				}
				ads_listAppend(h[newindex],tmpADP,AdsDictPair);
			}
			ads_listDelete(self->raw[i]);
		}
	}
	self->memory = newsize;
	ads_free(self->raw);
	self->raw = h;
}

bool ads_dictHasKey_ByRef(ads_dict  self,void* key)
{
	uint32_t i,p;
	AdsDictPair tmpADP;
	void*tmpkey;
	i = ADS_dictgetindex(key,self->hashfunction,self->memory);
	if(self->raw[i] != NULL)
	{
		for(p=0;p< ads_listLen(self->raw[i]) ;p++)
		{
			ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
			tmpkey = tmpADP.key;
			if(self->hashfunction(tmpkey) == self->hashfunction(key) )
			{
				return true;
			}
		}
	}
	return false;
}

int ads_dictLen(ads_dict self)
{
	return self->nfkeys;
}

int ads_dictBuckets(ads_dict self)
{
    return self->buckets;
}

int ads_dictMemory(ads_dict self)
{
    return self->memory;
}

ads_vector ads_dictKeys(ads_dict self)
{
	uint32_t i,p;
	void* tmpkey;
	AdsDictPair tmpADP;
	ads_vector l = ads_initVector(ads_tdGetSize(self->ktd),self->nfkeys);
	ads_vectorSetTD(l,self->ktd);
	for(i = 0;i < self->memory ;i++)
	{
		if(self->raw[i]!=NULL)
		{
			for(p=0;p< ads_listLen(self->raw[i]) ;p++)
			{
				ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
				tmpkey = tmpADP.key;
				ads_vectorAppend_ByRef(l,tmpkey);
			}
		}
	}
	return l;
}

ads_vector ads_dictValues(ads_dict self)
{
	uint32_t i,p;
	AdsDictPair tmpADP;
	void* tmpval;
	ads_vector l = ads_initVector(ads_tdGetSize(self->dtd),self->nfkeys);
	ads_vectorSetTD(l,self->dtd);
	for(i = 0;i < self->memory ;i++)
	{
		if(self->raw[i] != NULL)
		{
			for(p=0;p< ads_listLen(self->raw[i]) ; p++)
			{
				ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
				tmpval = tmpADP.data;
				ads_vectorAppend_ByRef(l,tmpval);
			}
		}
	}
	return l;
}

double ads_dictLoad(ads_dict self)
{
	return (double) self->buckets/(double) self->memory;
}

ads_rt ads_dictSetitem_ByRef(ads_dict self,void* key,void*value)
{
	double load = (double) self->buckets/(double) self->memory;
	if( load >= 0.6000)
	{
		ADS_dictreallocation(self);
	}
	if(key == NULL)
	{
		return ads_registerError("Cannot Set Dict With Null Key");
	}
	AdsDictPair tmpADP,tmpgetADP;
	int index = ADS_dictgetindex(key,self->hashfunction,self->memory);
	if(value != NULL)
	{
		void * ckey = ads_appropriteCopy(self->ktd,key);
		void * v = ads_appropriteCopy(self->dtd,value);
		tmpADP.key = ckey;
		tmpADP.data = v;
		if( self->raw[index] == NULL )
		{
			self->raw[index] = ads_listNew(AdsDictPair);
			self->buckets++;
		}
		else
		{
			uint32_t i=0;
			void* tmpkey,*tmpval;
			for(i=0 ; i < ads_listLen(self->raw[index]) ; i++)
			{
				ads_listGetItem_Copy(self->raw[index],i,AdsDictPair,tmpgetADP);
				tmpkey = tmpgetADP.key;
				tmpval = tmpgetADP.data;
				if( self->hashfunction(tmpkey) == self->hashfunction(key) )
				{
					ads_listSetItem(self->raw[index],i,tmpADP,AdsDictPair);
					ads_appropriateDelete(self->ktd,tmpkey);
					ads_appropriateDelete(self->dtd,tmpval);
					return ads_SUCCESS;
				}
			}
		}
		ads_listAppend(self->raw[index],tmpADP,AdsDictPair);
		self->nfkeys++;
	}
	return ads_SUCCESS;
}


void* ads_dictGetItem_NoCopy_RtnRef(ads_dict self,void* key)
{
	void* tmpkey;
	AdsDictPair tmpADP;
	int index = ADS_dictgetindex(key,self->hashfunction,self->memory);
	if(self->raw[index] == NULL)
	{
		ads_registerError("Key '%p' Not Found",key);
		return NULL;
	}
	else
	{
		uint32_t i;
		for(i=0; i < ads_listLen(self->raw[index]) ;i++)
		{
			ads_listGetItem_Copy(self->raw[index],i,AdsDictPair,tmpADP);
			tmpkey = tmpADP.key;
			if( self->hashfunction(tmpkey) == self->hashfunction(key) )
			{
				return tmpADP.data ;
			}
		}
		ads_registerError("Key '%p' Not Found",key);
		return NULL;
	}
}

void* ads_dictGetItem_Copy_RtnRef(ads_dict self,void* key)
{
	void* tmpkey;
	AdsDictPair tmpADP;
	int index = ADS_dictgetindex(key,self->hashfunction,self->memory);
	if(self->raw[index] == NULL)
	{
		ads_registerError("Key '%p' Not Found",key);
		return NULL;
	}
	else
	{
		uint32_t i;
		for(i=0;i< ads_listLen(self->raw[index]) ;i++)
		{
			ads_listGetItem_Copy(self->raw[index],i,AdsDictPair,tmpADP);
			tmpkey = tmpADP.key;
			if( self->hashfunction(tmpkey) == self->hashfunction(key) )
			{
				return ads_appropriteCopy(self->dtd,tmpADP.data);
			}
		}
		ads_registerError("Key '%p' Not Found",key);
		return NULL;
	}
}

void* ads_dictPopItem_RtnRef(ads_dict self,void* key)
{
	int index = ADS_dictgetindex(key,self->hashfunction,self->memory);
	if(self->raw[index] == NULL)
	{
		ads_registerError("Key '%p' Not Found",key);
		return NULL;
	}
	AdsDictPair tmpADP;
	uint32_t i;void* tmpkey;
	for(i=0;i< ads_listLen(self->raw[index]) ;i++)
	{
		ads_listGetItem_Copy(self->raw[index],i,AdsDictPair,tmpADP);
		tmpkey = tmpADP.key;
		if( self->hashfunction(tmpkey) == self->hashfunction(key) )
		{
			ads_listDelItem(self->raw[index],i);
			ads_appropriateDelete(self->ktd,tmpkey);
			self->nfkeys--;
			if( ads_listLen(self->raw[index]) <= 0 )
			{
				ads_listDelete(self->raw[index]);
				self->raw[index] = NULL;
				self->buckets--;
			}
			return tmpADP.data ;
		}
	}
	ads_registerError("Key '%p' Not Found",key);
    return NULL;
}

ads_rt ads_dictDelItem_ByRef(ads_dict self,void* key)
{
	int index = ADS_dictgetindex(key,self->hashfunction,self->memory);
	if(self->raw[index] == NULL)
	{
		return ads_registerError("Key '%p' Not Found",key);
	}
	uint32_t i;
	void* tmpkey;
	AdsDictPair tmpADP;
	for(i=0;i< ads_listLen(self->raw[index]) ;i++)
	{
		ads_listGetItem_Copy(self->raw[index],i,AdsDictPair,tmpADP);
		tmpkey = tmpADP.key;
		if( self->hashfunction(tmpkey) == self->hashfunction(key) )
		{
			ads_listDelItem(self->raw[index],i);
			ads_appropriateDelete(self->ktd,tmpkey);
			ads_appropriateDelete(self->dtd,tmpADP.data);
			self->nfkeys--;
			if( ads_listLen(self->raw[index]) <= 0 )
			{
				ads_listDelete(self->raw[index]);
				self->raw[index] = NULL;
				self->buckets--;
			}
			return ads_SUCCESS;
		}
	}
	return ads_registerError("Key '%p' Not Found",key);
}

int ads_dictSizeOf(ads_dict self)
{
	uint32_t i,cou=0;
	for(i = 0;i<self->memory;i++)
	{
		if(self->raw[i]!=NULL)
		{
			cou += ads_listSizeOf(self->raw[i]);
		}
	}
	cou += self->nfkeys*(ads_tdGetSize(self->dtd) + ads_tdGetSize(self->ktd));
	cou += self->memory*sizeof(ads_list*);
	cou += sizeof(ADS_StructOf(ads_dict));
	return cou;
}

ads_rt ads_dictExtend(ads_dict self,ads_dict other)
{
	if(!ads_tdCmp(self->dtd,other->dtd)||!ads_tdCmp(self->ktd,other->ktd))
	{
		return ads_registerError("Type Mismatch in ads_dict Extend Function two dicts are for different datatypes");
	}
	ads_vector l = ads_dictKeys(other);void * value,*key;
	int i;
	for(i=0;i<ads_vectorLen(l);i++)
	{
		key = ads_vectorGetItem_NoCopy_RtnRef(l,i);
		value = ads_dictGetItem_Copy_RtnRef(other,key);
		ads_dictSetitem_ByRef(self,key,value);
	}
	ads_vectorDelete(l);
	return ads_SUCCESS;
}

void ads_printDict(void*add,const char * cmd,int size)
{
    add = *(void**)add;
	uint32_t i,p;void* tmpkey;void* val;bool fir = true;
	ads_dict self = (ads_dict)add;
	int cutoff=0;
	ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint keyprint,valueprint;
	if(ads_iListLen(l)>1)
	{
		ads_iListGetItem(l,0,ads_genericPrint,keyprint);
		ads_iListGetItem(l,1,ads_genericPrint,valueprint);
	}
	else
	{
		ads_output("ads_dict Print Error");
		return ;
	}
	ads_deleteIList(l);

	ads_output("{ ");
	AdsDictPair tmpADP;
	for(i = 0 ; i<self->memory ; i++)
	{
		if(self->raw[i] != NULL)
		{
			for( p = 0 ; p < ads_listLen(self->raw[i]) ; p++ )
			{
				if(fir == false) ads_output(" , ");
				ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
				tmpkey = tmpADP.key;
				val = tmpADP.data;
				keyprint(tmpkey,&cmd[cutoff],size);
				ads_output(" : ");
				valueprint(val,&cmd[cutoff],size);
				fir = false;
			}
		}
	}
	ads_output(" }");
}

ads_dict ads_dictCopy(ads_dict _self)
{
	ads_dict self;
	ads_CheckAndCastPointer(_self,ads_dict,self);
	ads_dict d = ads_initDict( ads_tdGetSize(self->ktd) ,self->hashfunction, ads_tdGetSize(self->dtd) ,self->memory);
	ads_tdCopy(d->dtd,self->dtd);
	ads_tdCopy(d->ktd,self->ktd);
	ads_vector  l =  ads_dictKeys(self);
	int i ;void* key;void * value;
	for(i=0;i<ads_vectorLen(l);i++)
	{
		key = ads_vectorGetItem_NoCopy_RtnRef(l,i);
		value = ads_dictGetItem_NoCopy_RtnRef(self,key);
		ads_dictSetitem_ByRef(d,key,value);
	}
	ads_vectorDelete(l);
	return d;
}

void ads_dictClear(ads_dict self)
{
	uint32_t i,p;
	AdsDictPair tmpADP;
	for(i=0;i<self->memory;i++)
	{
		if(self->raw[i] != NULL)
		{
			for( p = 0 ; p < ads_listLen(self->raw[i]) ; p++ )
			{
				ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
				ads_appropriateDelete(self->ktd,tmpADP.key);
				ads_appropriateDelete(self->dtd,tmpADP.data);
			}
			ads_listDelete(self->raw[i]);
			self->raw[i] = NULL;
		}
	}
	self->nfkeys = 0;
	self->buckets = 0;
}

void ads_dictDelete(ads_dict _self)
{
    uint32_t i,p;
	ads_dict self;
	ads_CheckAndCastPointer(_self,ads_dict,self);
	AdsDictPair tmpADP;
	for(i=0;i<self->memory;i++)
	{
		if(self->raw[i] != NULL)
		{
			for( p = 0 ; p < ads_listLen(self->raw[i]) ; p++ )
			{
				ads_listGetItem_Copy(self->raw[i],p,AdsDictPair,tmpADP);
				ads_appropriateDelete(self->ktd,tmpADP.key);
				ads_appropriateDelete(self->dtd,tmpADP.data);
			}
			ads_listDelete(self->raw[i]);
			self->raw[i] = NULL;
		}
	}
	ads_free(self->raw);
	ads_free(self->dtd);
	ads_free(self->ktd);
	ads_free(self);
}

void* ads_dictCopy_ByRef_RtnRef(const void*v)
{
    ads_dict t,cv ;
    ads_CheckAndCastPointer(*(ads_dict*)v,ads_dict,t);
    cv = ads_dictCopy(t);
    return ads_toItem(cv);
}

void ads_dictDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_dict,vl);
    ads_dictDelete(vl);
}

ads_td ads_dictTDStruct = {ads_dictCopy_ByRef_RtnRef,ads_dictDelete_ByRef,sizeof(ads_dict)};

const ads_td * const ads_dictTD = &ads_dictTDStruct;


#endif

/** @endcond */
