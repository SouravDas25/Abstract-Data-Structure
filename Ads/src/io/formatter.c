#ifndef ADS_FORMATTER_C
#define ADS_FORMATTER_C

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>


#include "../../include/io/formatter.h"

struct ads_format_detail {
	ads_typeCode tc;
	const char * typname;
	int typlen;
};

typedef struct ads_format_detail ads_format_detail;

struct _ads_formatInfo {
	ads_format_detail array[ADS_MAXTYPE];
	unsigned int size;
}ads_formatINFO={.size=0};

void ads_formatAppendInternal(ads_typeCode tc,const char * format,int len)
{
    ads_formatINFO.array[ads_formatINFO.size].tc = tc;
    ads_formatINFO.array[ads_formatINFO.size].typname = format;
    ads_formatINFO.array[ads_formatINFO.size].typlen = len;
    ads_formatINFO.size++;
}

ads_typeCode ads_formatAppend(const char * format,int len)
{
    ads_formatINFO.array[ads_formatINFO.size].tc = ads_formatINFO.size;
    ads_formatINFO.array[ads_formatINFO.size].typname = format;
    ads_formatINFO.array[ads_formatINFO.size].typlen = len;
    return  ads_formatINFO.size++;
}

void ads_formatInit()
{
	ads_formatAppendInternal(CHAR_TC,"char",1);
	ads_formatAppendInternal(INT_TC,"int",3);
	ads_formatAppendInternal(INT32_TC,"int32",1);
	ads_formatAppendInternal(INT64_TC,"int64",2);
	ads_formatAppendInternal(FLOAT_TC,"float",1);
	ads_formatAppendInternal(DOUBLE_TC,"double",2);
	ads_formatAppendInternal(STRING_TC,"varchar",1);
	ads_formatAppendInternal(STRING_TC,"string",1);
	ads_formatAppendInternal(ZBUFFER_TC,"buffer[#]",1);
}

ads_format_detail* ads_parseSubFormat(const char * cmd,int size)
{
	unsigned int i;
	for(i=0; i < ads_formatINFO.size; i++)
	{
		ads_format_detail * tmp = ads_formatINFO.array+i;
		if(ads_regExpCmp(cmd,size,tmp->typname,tmp->typlen) == true)
		{
			return tmp;
		}
	}
	return ads_formatINFO.array;
}

ads_iList ads_parseFormat(const char * cmd,int len)
{
	if(!ads_formatINFO.size) ads_formatInit();
	ads_iList l = ads_iListNew(ads_typeCode);
	int i,p=0;ads_format_detail * tmp;
	if(ads_isCommanPresent(cmd,len))
	{
		for(i=0;i<len;i++)
		{
			if( cmd[i] == ',' || cmd[i] == ':' )
			{
				tmp = ads_parseSubFormat(&cmd[p],(i-p));
				p = i+1;
				ads_iListAppend(l,tmp->tc,ads_typeCode);
			}
		}
		tmp = ads_parseSubFormat(&cmd[p],(i-p));
		p = i+1;
		ads_iListAppend(l,tmp->tc,ads_typeCode);
	}
	else
	{
		tmp = ads_parseSubFormat(cmd,len);
		ads_iListAppend(l,tmp->tc,ads_typeCode);
	}
	return l;
}


ads_typeCode ads_typeCodeOne(const char * cmd,int len)
{
    ads_iList l = ads_parseFormat(cmd,len);
    ads_typeCode tc ;
    if(ads_iListLen(l)>0)
    {
        ads_iListGetItem(l,0,ads_typeCode,tc);
    }
    else tc = VOID_TC;
    ads_deleteIList(l);
    return tc;
}


#endif // ADS_FORMATTER_C
