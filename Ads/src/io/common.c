
/** @cond ADS_INTERNAL */

#ifndef ADS_IO_COMMON_C
#define ADS_IO_COMMON_C

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>

#include "../../include/io/common.h"

bool ads_isCommanPresent(const char * cmd,int size)
{
	int i,clevel=0;
	for(i=0;i<=size;i++)
	{
		if(ads_isStartBracket(cmd[i]) == true ) clevel++;
		else if(ads_isEndBracket(cmd[i]) == true ) clevel--;
		if( clevel == 0 )
		{
			if(cmd[i] == ',' || cmd[i] == ':' ) return true;
		}
	}
	return false;
}

int ads_isValidFormatting(const char* format,int * adi)
{
	int i = *adi;
	if(format[i] == '{')
	{
		if(format[i+1] == ads_endOfStr) return 0;
		else if(format[i+1] == '{') {
			(*adi)++;
			return 0;
		}
		else if(i>1) {
			if(format[i-1] == '{' ) return 0;
		}
		return 1;
	}
	else if(format[i] == '%')
	{
		if(format[i+1] == ads_endOfStr) return 0;
		else if(format[i+1] == '%') {
			(*adi)++;
			return 0;
		}
		else if(i>1) {
			if(format[i-1] == '%' ) return 0;
		}
		return 2;
	}
	return 0;
}

const char* ads_getOldCMD(const char * format,int* i)
{
	static char cmd[16] = "";
	int cnt=0;
	while( (isalnum(format[*i+1]) || ispunct(format[*i+1])) && format[*i+1] != ads_endOfStr && cnt < 15 )
	{
		cmd[cnt++] = format[*i+1];
		(*i)++;
	}
	cmd[cnt++] = '\0';
	return cmd;
}

const char* ads_getCMD(const char * format,int* i)
{
    int cmdmaxsize = 64;
	static char cmd[64] = "";
	int cnt=0;
	(*i)++;
	while( (format[*i] != '}' && format[*i] != ads_endOfStr) && cnt < cmdmaxsize-1 )
	{
		if(!isspace(format[*i]))cmd[cnt++] = format[*i];
		(*i)++;
	}
	cmd[cnt++] = '\0';
	return cmd;
}

#endif

/** @endcond */
