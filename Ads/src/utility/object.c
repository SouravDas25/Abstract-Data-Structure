

 /** @cond ADS_INTERNAL */

#ifndef C_ObjectOriented_C
#define C_ObjectOriented_C

#include<assert.h>



#include "../../include/utility/object.h"

struct ads_objectMethod{
	void* (*copyconstructor)(void*);
    void (*destructor)(void*);
    const char * type;
    int TypeCode;
};

struct ads_object{
	ads_objectMethod * tomethod;
};

void* ads_rawObject_ByMacro(int objectsize,void*mp)
{
	void * v = ads_rawCalloc(1,objectsize);
	if(v == NULL) return v;
	ads_objectMethod * o = (ads_objectMethod*)mp;
	((ads_object*)v)->tomethod = o;
	return v;
}

void* ads_duplicate(void*var)
{
	return ((ads_object*)var)->tomethod->copyconstructor(var);
}

void ads_callDestructor(void*var)
{
	((ads_object*)var)->tomethod->destructor(var);
}

const char * ads_getObjectTypeName(void*var)
{
    return ((ads_object*)var)->tomethod->type;
}

void ads_deleteOne(void*var)
{
	ads_rawFree(var);
}

void ads_deleteMany(int n,...)
{
    int i;void*var;
    va_list arg;
    va_start(arg,n);
	for(i=0;i<n;i++)
	{
	    var = va_arg(arg,void*);
		ads_rawFree(var);
	}
	va_end(arg);
}

#endif


/** @endcond */
