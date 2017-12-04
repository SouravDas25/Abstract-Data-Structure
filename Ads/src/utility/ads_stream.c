#ifndef ADSSTREAM_C
#define ADSSTREAM_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/utility/ads_stream.h"

/*IO Stream*/
struct __adsoutputstream
{
	FILE * stream ;
	char * buffer;
	int size;
}ads_outStream = {NULL,NULL,0};


FILE* ads_getOutputStream()
{
	if(ads_outStream.stream != NULL) return ads_outStream.stream;
	else return stdout;
}

char * ads_getOutBuffer()
{
	return ads_outStream.buffer;
}

int ads_getBufferLen()
{
	return ads_outStream.size;
}

void ads_setOutputStream(FILE*ostream)
{
	ads_outStream.stream = ostream;
	ads_outStream.buffer = NULL;
}

void ads_setOutputBuffer(char * buffer,int size)
{
	ads_outStream.buffer = buffer;
	ads_outStream.size = size;
}

bool ads_getOutType()
{
	if(ads_outStream.buffer)return true;
	return false;
}

int ads_output(const char * format,...)
{
	int x;
	va_list v;
	va_start(v,format);
	if(ads_getOutType())
	{
		x = vsnprintf(ads_getOutBuffer(),ads_getBufferLen(),format,v);
		if(x<0) x = ads_getBufferLen();
		ads_setOutputBuffer(ads_getOutBuffer()+x,ads_getBufferLen()-x);
	}
	else x = vfprintf(ads_getOutputStream(),format,v);
	va_end(v);
	return x;
}

void ads_putChar(int c)
{
	if(ads_getOutType())
	{
		int x = snprintf(ads_getOutBuffer(),ads_getBufferLen(),"%c",c);
		if(x<0) x = ads_getBufferLen();
		ads_setOutputBuffer(ads_getOutBuffer()+x,ads_getBufferLen()-x);
	}
	else putc(c,ads_getOutputStream());
}



#endif
