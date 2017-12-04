#ifndef DATAREPLICATOR_C
#define DATAREPLICATOR_C

#include "../../include/utility/datareplicator.h"


const ads_cmpFunc ads_cmpMem = (int(*)(const void*,const void*))-1;

void* ads_copyAlloc(const void * b,int size) /*takes the address and size of a variable and return a dynamically allocated copy of it*/
{
	void * v;
	if(size == 0)
	{
		ads_registerError("Cannot allocate memory with 0 size ");
		return NULL;
	}
	if( b == NULL)
	{
		ads_registerError("Cannot copy a value pointed by NULL pointer.");
		return NULL;
	}
	v = ads_calloc(1,size);
	ads_checkMemory(v);
	return memcpy(v,b,size);
}

ads_td * ads_tdNew(void* (*copy) (const void*),void (*destroyer)(void*),size_t size)
{
    ads_td * td = (ads_td *)ads_calloc(1,sizeof(ads_td));
    td->copy = copy;
    td->destroyer = destroyer;
    td->size = size;
    return td;
}

ads_rt ads_tdCopy(ads_td * ctd,const ads_td * td)
{
    *ctd = *td;
    return ads_SUCCESS;
}

bool ads_tdCmp(const ads_td *td1,const ads_td *td2)
{
    if(td1->copy != td2->copy )return false;
    if(td1->destroyer != td2->destroyer )return false;
    if(td1->size != td2->size )return false;
    return true;
}


ads_rt ads_tdSet(ads_td *td,void* (*copy) (const void*),ads_destroyFunction destroyer,size_t s)
{
    td->copy = copy;
    td->destroyer = destroyer;
    td->size = s;
    return ads_SUCCESS;
}

size_t ads_tdGetSize(const ads_td *td)
{
    return td->size;
}

ads_rt ads_tdSetSize(ads_td *td,size_t s)
{
    td->size = s;
    return ads_SUCCESS;
}

void ** ads_toItem(void*v)
{
    void ** c = ads_calloc(1,sizeof(void*));
    *c = v;
    return c;
}

void* ads_fromItem(void**v)
{
    void *c = *v;
    ads_free(v);
    return c;
}

void* ads_appropriteCopy(ads_td * td,const void*value)
{
	void * cal ;
	if( td && td->copy )
	{
		cal = td->copy(value) ;
	}
	else cal = ads_copyAlloc(value,td->size);
	return cal;
}

void ads_appropriateDelete(ads_td * td,void*value)
{
	if ( td && td->destroyer )
	{
		td->destroyer( value );
	}
	else ads_free(value);
}

void* ads_evaluatePointer(int isads,void**value)
{
	void * val;
	if(isads == 1)
	{
		val = *value;
	}
	else val = (void*)value;
	return val;
}

#endif
/** @endcond */
