
/** @cond ADS_INTERNAL */

#ifndef ADSITERATOR_C
#define ADSITERATOR_C



#include "../../include/utility/iterator.h"

ads_iterator ads_initIterator(void* object, ads_iterator_function next, ads_iterator_function start, int max, void* extra) /*iterator constructor */
{
	ads_iterator i ;
	i.object = object;
	i.nextitem = next;
	i.startfunc = start;
	i.max = max;
	i.count = 0;
	i.extra = extra;
	return i;
}

void* ads_iteratorStart(ads_iterator* _i)
{
	ads_iterator* i = (ads_iterator*)_i;
	i->count = 0;
	if(i->max <= i->count)
	{
		return NULL;
	}
	return i->startfunc(i);
}

void* ads_iteratorNext(ads_iterator* _i)
{
	void*v;
	ads_iterator* i = (ads_iterator*)_i;
	i->count++;
	v= i->nextitem(i);
	if(i->max <= i->count)
	{
		v = NULL;
	}
	return v;
}

int ads_enumerate_ByRef(ads_iterator*i)
{
    return i->count;
}

void* ads_arriterstart(ads_iterator* i)
{
	return i->object;
}

void* ads_arriternext(ads_iterator* i)
{
	long datatype ;char * c;
    memcpy(&datatype,&i->extra,sizeof(i->extra));
	c = (char*)i->object;
	i->object = c+datatype;
	return i->object;
}

ads_iterator __ads_arrayIterator(void * arr,int64_t datatype,int size)
{
    void * v ;
	ads_iterator i;
    memcpy(&v,&datatype,sizeof(int64_t));
	i = ads_initIterator(arr,ads_arriternext,ads_arriterstart,size,v);
	return i;
}


#endif
/** @endcond */
