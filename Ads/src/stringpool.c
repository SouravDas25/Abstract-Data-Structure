
/** @cond ADS_INTERNAL */


#ifndef ADSstringpool_C
#define ADSstringpool_C



#include "../include/Stringpool.h"



struct ads_structString {
	char * data ;
	unsigned int refcount;
	unsigned int length;
};

size_t ads_sizeOfStructString(ads_structString*s)
{
    return s->length*sizeof(char)+sizeof(ads_structString);
}

const char* ads_structStringData(ads_structString*s)
{
    return s->data;
}

int ads_structStringLen(ads_structString*s)
{
    return s->length;
}

static ads_dict ads_strDict = NULL;

ads_structString* ads_newStructString(const char * str)
{
	int len = strlen(str);
	ads_structString * newstr = (ads_structString*)ads_calloc(1,sizeof(ads_structString));
	char * data = (char*)ads_calloc(len+1,sizeof(char));
	strcpy(data,str);
	newstr->length = len;
	newstr->data = data;
	newstr->refcount = 1;
	return newstr;
}

ads_structString* ads_newStringObj(const char * str)
{
	if(ads_strDict == NULL)
	{
		ads_strDict = ads_dictNew(uint64_t,ads_hashInt64,ads_structString*,10);
	}
	ads_structString* st = NULL;
	uint64_t hasdedvalue = ads_cstrHash(str);
	if( ads_dictHasKey_ByRef(ads_strDict,&hasdedvalue) == false )
	{
		st = ads_newStructString(str);
		ads_dictSetitem_ByRef(ads_strDict,&hasdedvalue,&st);
	}
	else
	{
		st = *(ads_structString**)ads_dictGetItem_NoCopy_RtnRef(ads_strDict,&hasdedvalue);
		st->refcount++;
	}
	return st;
}

ads_structString* ads_copyStringObj(ads_structString* strRef)
{
	strRef->refcount++;
	return strRef;
}

ads_rt ads_freeStringObj(ads_structString* strRef)
{
	char* str = strRef->data;
	if(ads_strDict == NULL)
	{
		return ads_registerError("String Object Not Created To Delete");
	}
	ads_structString * st = NULL;
	uint64_t hasdedvalue = ads_cstrHash(str);
	if( ads_dictHasKey_ByRef(ads_strDict,&hasdedvalue) ==  true )
	{
		st = strRef;
		st->refcount--;
		if( st->refcount <= 0 )
		{
			ads_dictDelItem_ByRef(ads_strDict,&hasdedvalue);
			ads_free(st->data);
			ads_free(st);
			if(ads_dictLen(ads_strDict) <= 0)
			{
				ads_dictDelete(ads_strDict);
				ads_strDict = NULL;
			}
			return ads_SUCCESS;
		}
		return ads_SUCCESS;
	}
	return ads_registerError("ads_str '%s' is already freed",strRef->data);
}

void ads_printAdsStructString(void* value,const char * cmd,int size)
{
	cmd= cmd;
	size=size;
	ads_output("'%s'",(*(ads_structString**)value)->data);
}

void ads_printStringPool()
{
	static bool isthere;
	if(!isthere)
	{
		ads_genericIoInsert(ads_printAdsStructString,"s3",0,0);
	}
	if(ads_strDict!=NULL)
    {
        ads_output("\nPrint-stringPool : load = %lf\n",ads_dictLoad(ads_strDict));
        ads_output("Print-stringPool : buckets = %d , memory = %d\n",ads_dictBuckets(ads_strDict),ads_dictMemory(ads_strDict));
        ads_printDict(ads_strDict,"ld:s3",5);
    }
}


#endif

/** @endcond  */
