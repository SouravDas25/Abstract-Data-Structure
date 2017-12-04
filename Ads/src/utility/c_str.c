#ifndef ADSCSTR_C
#define ADSCSTR_C

#include "../../include/utility/c_str.h"

void ads_printCStr(const void*add)
{
    if(add)ads_output("%s",(char*)add);
}

void ads_printCStr_ByRef(const void*add)
{
    if(add)ads_output("%s",*(char**)add);
}

int ads_cmpCStr(const void*a,const void*b)
{
    return strcmp((char*)a,(char*)b);
}

int ads_cmpCStr_ByRef(const void*a,const void*b)
{
    return strcmp(*(char**)a,*(char**)b);
}

void* ads_cstrCopy(const void*v)
{
    if(!v)return NULL;
    char * c = (char*)ads_calloc(strlen((char*)v),sizeof(char));
    strcpy(c,(char*)v);
    return c;
}

void* ads_cstrCopy_ByRef_RtnRef(const void*v)
{
    if(!v)return NULL;
    char * c = (char*)ads_calloc(strlen(*(char**)v),sizeof(char));
    strcpy(c,*(char**)v);
    return ads_toItem( c );
}

void ads_cstrDelete( void*v)
{
    if(v)
    ads_free( v );
}

void ads_cstrDelete_ByRef(void*v)
{
    if(v)
    ads_free( ads_fromItem(v) );
}

uint64_t ads_cstrHash(const void * _buffer) /*produces a hash value of a c-string */
{
    if(!_buffer)return 0;
    char * buffer = (char*)_buffer;
	uint64_t val=0,i,len=strlen(buffer);
	val = buffer[0]>>7;
	for(i=0;i<len;i++)
	{
		val += 1000003*val^buffer[i];
	}
	val = val ^ len ;
	return val;
}

uint64_t ads_cstrHash_ByRef(const void * _buffer)
{
    char * buffer = *(char**)_buffer;
    return ads_cstrHash(buffer);
}

const ads_td ads_cstrTDStruct = {ads_cstrCopy_ByRef_RtnRef,ads_cstrDelete_ByRef,sizeof(char*)};

const ads_td * const ads_cstrTD = &ads_cstrTDStruct;


#endif // ADSCSTR_C
