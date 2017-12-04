/** @cond ADS_INTERNAL */
#ifndef ADS_GPF_C
#define ADS_GPF_C

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>

#include "../../include/io/gpf.h"

struct _ads_genIoInfo ads_genIoInfo;

void ads_printvoid(void*add,const char * cmd,int size)
{
	add = add;
	cmd = cmd;
	size = size;
	return ;
}

void ads_printchar(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%c",*(char*)add);
}

void ads_printbool(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%s",(*(char*)add)?"True":"False");
}

void ads_printint(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%d",*(int*)add);
}

void ads_printint32(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%I32d",*(int32_t*)add);
}

void ads_printint64(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%I64d",*(int64_t*)add);
}

void ads_printuint(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%u",*(unsigned int*)add);
}

void ads_printhex(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%x",*(unsigned int*)add);
}

void ads_printoctal(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%o",*(unsigned int*)add);
}

void ads_printfloat(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%g",*(float*)add);
}

void ads_printdouble(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%g",*(double*)add);
}

void ads_printcstring(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("%s",*(char**)add);
}

void ads_print_qouted_cstring(void*add,const char * cmd,int size)
{
	cmd = cmd;
	size = size;
	ads_output("'%s'",*(char**)add);
}

void ads_genericIoInternalInsert(ads_genericPrint pfunction,ads_typeCode tc,const char * typregexp,int cutoff,int size)
{
	if(ads_genIoInfo.size < ADS_MAXTYPE )
	{
		ads_genIoInfo.OutputArray[ads_genIoInfo.size] = (ads_outputInfo){pfunction,tc,typregexp,strlen(typregexp),cutoff,size};
		ads_genIoInfo.size++;
	}
}

void ads_genericIoInit()
{
    static bool ioinit = false;
	if(ioinit) return ;
    ioinit = true ;
    ads_genIoInfo.size = 0;
    ads_genericIoInternalInsert(ads_printvoid,VOID_TC,"",0,0);
    ads_genericIoInternalInsert(ads_printchar,CHAR_TC,"c",0,0);
    ads_genericIoInternalInsert(ads_printbool,BOOL_TC,"bl",0,0);
    ads_genericIoInternalInsert(ads_printint,INT_TC,"d",0,0);
    ads_genericIoInternalInsert(ads_printint32,INT32_TC,"I32d",0,0);
    ads_genericIoInternalInsert(ads_printint64,INT64_TC,"I64d",0,0);
    ads_genericIoInternalInsert(ads_printuint,UINT_TC,"u",0,0);
    ads_genericIoInternalInsert(ads_printhex,HEX_TC,"x",0,0);
    ads_genericIoInternalInsert(ads_printoctal,OCT_TC,"o",0,0);
    ads_genericIoInternalInsert(ads_printfloat,FLOAT_TC,"f",0,0);
    ads_genericIoInternalInsert(ads_printdouble,DOUBLE_TC,"lf",0,0);
    ads_genericIoInternalInsert(ads_printcstring,STRING_TC,"s",0,0);
    ads_genericIoInternalInsert(ads_print_qouted_cstring,QSTRING_TC,"'s'",0,0);
}

bool ads_IsTypeRegistered(const char * cmd,int size)
{
	unsigned int i;
	for(i=0;i < ads_genIoInfo.size;i++)
	{
		ads_outputInfo * tmp = ads_genIoInfo.OutputArray+i;
		if(strncmp(cmd,tmp->typname,size) == 0)
		{
			return true ;
		}
	}
	return false;
}

int ads_genericIoInsert(ads_genericPrint pfunction,const char * typregexp,int cutoff,int size)
{
    ads_genericIoInit();
	if(ads_genIoInfo.size < ADS_MAXTYPE )
	{
		ads_genIoInfo.OutputArray[ads_genIoInfo.size] = (ads_outputInfo){pfunction,ads_genIoInfo.size,typregexp,strlen(typregexp),cutoff,size};
		ads_genIoInfo.size++;
	}
	return ads_genIoInfo.size-1;
}

struct _typelist {
	const char ** typelist ;
	int size;
};
struct _typelist ads_typelist  = {NULL,0} ;

const char *  __Ads_typelist[] = {"avl","heap","minheap","maxheap",
"extrie","graph","tree","treeLeaf","trie","string","list","vector","dict","stack","queue","deque","set","tuple"};

const char * __ADS_ACtual_typelist[ADS_MAXTYPE] ;

void ads_typeInit()
{
	ads_typelist.typelist = __ADS_ACtual_typelist;
	ads_typelist.size = sizeof(__Ads_typelist)/sizeof(char*);
	memcpy(__ADS_ACtual_typelist,__Ads_typelist,sizeof(__Ads_typelist));
}

void ads_typeInsert(const char * typname)
{
	if(!ads_typelist.typelist) ads_typeInit();
	if(ads_typelist.size < ADS_MAXTYPE )
	{
		ads_typelist.typelist[ads_typelist.size++] = typname;
	}
}

int ads_isAdsType(const char * typname)
{
	if(!ads_typelist.typelist) ads_typeInit();
	int i ;
    if(strncmp(typname,"ads_",4)==0)
    {
        typname = typname+4;
    }
	for(i=0;i<ads_typelist.size;i++)
	{
		if(strcmp(typname,ads_typelist.typelist[i]) == 0) return ads_typeTrue;
	}
	return ads_typeFalse;
}

bool ads_regExpCmp(const char * s1,int ls,const char * regexp,int n)
{
	int i = -1,cnt=0,clevel=0;
	if(!n)return false;
	if(ls < n)return false; /*regexp[i]*/
	while(regexp[++i] != '\0')
	{
		if(s1[cnt] == '\0') return false;
		if(regexp[i] != '#')
		{
			if(regexp[i] != s1[cnt++] ) return false;
		}
		else
		{
			while( s1[cnt] != '\0' )
			{
				if(s1[cnt] == ':' || s1[cnt] == ',')
				{
					if(!strchr(regexp,s1[cnt]))return false;
				}
				if( regexp[i+1] == s1[cnt] && clevel == 0) break;
				if(ads_isStartBracket(s1[cnt]) == true ) clevel++;
				else if(ads_isEndBracket(s1[cnt]) == true ) clevel--;
				cnt++;
			}
		}
	}
	if(clevel==0 && cnt >= ls)return true;
	return false;
}

#endif

/** @endcond */
