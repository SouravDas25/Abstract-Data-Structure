/** @cond ADS_INTERNAL */

#ifndef ADSError_C
#define ADSError_C

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#include<math.h>
#include<stdarg.h>
#include <time.h>
#include<stdbool.h>

#include "../../include/utility/error.h"

char * last_error_msg = NULL;

const char* ads_tempBuffer(const char * format,...)
{
	static char __formatbufers1__[ads_BUFSIZ>>1];
	va_list ap ;
    va_start(ap,format);
	vsnprintf(__formatbufers1__,ads_BUFSIZ,format,ap);
	va_end(ap);
	return __formatbufers1__;
}

ads_rt __ads_assert(bool exp,const char*_exp,const char* file ,int line,const char * func,const char* emsg)
{
	if(!exp)
	{
		return ads_registerError("in the Expression : \"%s\" on file: \"%s\" line: \"%d\"in Function: \"%s\" \n msg : %s",_exp,file,line,func,emsg);
	}
	return ads_SUCCESS;
}

ads_rt ads_checkMemory(void * n)
{
    if(n == NULL)
    {
        return ads_registerError("Out of Memory");
    }
    return ads_SUCCESS;
}

ads_rt ads_registerFatalError(const char * msg,...)
{
    va_list v;
    va_start(v,msg);
    int len = vsnprintf(NULL,0,msg,v);
    ads_resetLastError();
    if(len>0)
    {
        len++;
        last_error_msg = calloc(len,sizeof(char));
        vsprintf(last_error_msg,msg,v);
    }
    va_end(v);
    return ads_FATAL_ERROR;
}


ads_rt ads_registerError(const char * msg,...)
{
    va_list v;
    va_start(v,msg);
    int len = vsnprintf(NULL,0,msg,v);
    ads_resetLastError();
    if(len>0)
    {
        len++;
        last_error_msg = calloc(len,sizeof(char));
        vsprintf(last_error_msg,msg,v);
    }
    va_end(v);
    return ads_ERROR;
}

const char * ads_lastErrorMsg(void)
{
    return last_error_msg;
}

void ads_resetLastError()
{
    if(last_error_msg)free(last_error_msg);
    last_error_msg = NULL;
}

#endif
/** @endcond */
