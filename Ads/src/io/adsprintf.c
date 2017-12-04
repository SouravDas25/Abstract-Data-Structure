
/** @cond ADS_INTERNAL */
#ifndef ADS_PRINTF_C
#define ADS_PRINTF_C


#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>


#include "../../include/io/adsprintf.h"

extern struct _ads_genIoInfo ads_genIoInfo;

ads_outputInfo* ads_parseType(const char * cmd,int size)
{
	unsigned int i;ads_outputInfo * tmp;
	for(i=0; i < ads_genIoInfo.size; i++)
	{
		tmp = ads_genIoInfo.OutputArray+i;
		if(ads_regExpCmp(cmd,size,tmp->typname,tmp->typlen) == true)
		{
			return tmp;
		}
	}
	return ads_genIoInfo.OutputArray;
}

ads_iList ads_getTypeCode(const char * cmd,int *size ,int * cutoff)
{
	ads_genericIoInit();
	ads_iList l = ads_iListNew(ads_outputInfo*);
	int i,p=0;ads_outputInfo* tmp;
	if(ads_isCommanPresent(cmd,*size))
	{
		for(i=0;i<*size;i++)
		{
			if( cmd[i] == ',' || cmd[i] == ':' )
			{
				tmp = ads_parseType(&cmd[p],(i-p));
				p = i+1;
				ads_iListAppend(l,tmp,ads_outputInfo*);
			}
		}
		tmp = ads_parseType(&cmd[p],(i-p));
		p = i+1;
		ads_iListAppend(l,tmp,ads_outputInfo*);
	}
	else
	{
		tmp = ads_parseType(cmd,*size);//tmp->tc
		*cutoff = tmp->cutoffstart;
		*size = *size - tmp->cutoffsize;
		ads_iListAppend(l,tmp,ads_outputInfo*);
	}
	return l;
}

ads_iList ads_getPrintFunction(const char * cmd,int *size,int * cutoff)
{
	ads_iList ltc = ads_getTypeCode(cmd,size,cutoff);
	ads_iList lgf = ads_iListNew(ads_genericPrint);
	ads_outputInfo* tmp;
	ads_iListNode trav = ads_getIListHead(ltc);
	while(trav != NULL)
	{
		tmp = ads_refItem(ads_getIListValue(trav),ads_outputInfo*);
        ads_genericPrint p =  tmp->printfunction;
        if(p){
            ads_iListAppend(lgf,p,ads_genericPrint);
        }
        else {
            p = ads_genIoInfo.OutputArray[0].printfunction;
            ads_iListAppend(lgf,p,ads_genericPrint);
        }
		trav = ads_getIListNext(trav);
	}
	ads_deleteIList(ltc);
	if(ads_iListLen(lgf)<=0)
	{
		ads_iListAppend(lgf,ads_genIoInfo.OutputArray[0].printfunction,ads_genericPrint);
	}
	return lgf;
}

ads_iList ads_getprintfunction_withTC(const char * cmd,int * size,int * cutoff,ads_typeCode * tp)
{
	ads_iList ltc = ads_getTypeCode(cmd,size,cutoff);
	ads_iList lgf = ads_iListNew(ads_genericPrint);

	ads_outputInfo* tmp;

	if(ads_iListLen(ltc) <= 0 )*tp = VOID_TC;
	else
    {
        ads_iListGetItem(ltc,0,ads_outputInfo*,tmp);
        *tp = tmp->tc;
    }

	ads_iListNode trav = ads_getIListHead(ltc);
	while(trav != NULL)
	{
		tmp = ads_refItem(ads_getIListValue(trav),ads_outputInfo*);
        ads_genericPrint p =  tmp->printfunction;
        if(p) {
            ads_iListAppend(lgf,p,ads_genericPrint);
        }
        else {
            p = ads_genIoInfo.OutputArray[0].printfunction;
            ads_iListAppend(lgf,p,ads_genericPrint);
        }
		trav = ads_getIListNext(trav);
	}
	ads_deleteIList(ltc);
	if(ads_iListLen(lgf)<=0)
	{
		ads_iListAppend(lgf,ads_genIoInfo.OutputArray[0].printfunction,ads_genericPrint);
	}
	return lgf;
}

void ads_Cmd2Out(const char * cmd,va_list * pointer_arg)
{
	int64_t si;double d;const char *s;void*add;int cutoff = 0;
	int size = strlen(cmd);
	ads_typeCode t ;
	ads_iList l = ads_getprintfunction_withTC(cmd,&size,&cutoff,&t);
	ads_genericPrint print ;
	ads_iListGetItem(l,0,ads_genericPrint,print);
	ads_deleteIList(l);
	switch( t )
	{
		case CHAR_TC : {
			si = va_arg(*pointer_arg,int);
			ads_output("%c",si);
			break;
		}
		case BOOL_TC : {
			si = va_arg(*pointer_arg,int);
			ads_output("%s",(si)?"True":"False");
			break;
		}
		case INT_TC : {
			si = va_arg(*pointer_arg,int);
			ads_output("%d",si);
			break;
		}
		case INT32_TC : {
			si = va_arg(*pointer_arg,int32_t);
			ads_output("%I32d",si);
			break;
		}
		case INT64_TC : {
			si = va_arg(*pointer_arg,int64_t);
			ads_output("%I64d",si);
			break;
		}
		case OCT_TC : {
			si = va_arg(*pointer_arg,int);
			ads_output("%o",si);
			break;
		}
		case STRING_TC : {
			s = va_arg(*pointer_arg, char*);
			ads_output("%s",s);
			break;
		}
		case QSTRING_TC : {
			s = va_arg(*pointer_arg, char*);
			ads_output("'%s'",s);
			break;
		}
		case HEX_TC : {
			si = va_arg(*pointer_arg,unsigned int);
			ads_output("%x",si);
			break;
		}
		case UINT_TC : {
			si = va_arg(*pointer_arg,unsigned int);
			ads_output("%u",si);
			break;
		}
		case FLOAT_TC :
		case DOUBLE_TC : {
			d = va_arg(*pointer_arg,double);
			ads_output("%g",d);
			break;
		}
		case VOID_TC :{
			break;
		}
		default : {
			add = va_arg(*pointer_arg,void*);
			print(&add,&cmd[cutoff],size);
			break;
		}
	}
}

void ads_vprintf(const char * format,va_list * arg)
{
	int i,len = strlen(format),formattype;
	const char * cmd = NULL;
	for(i=0;i<len;i++)
	{
		formattype = ads_isValidFormatting(format,&i);
		if( formattype )
		{
			if(formattype == 1)cmd = ads_getCMD(format,&i);
			else cmd = ads_getOldCMD(format,&i);
			ads_Cmd2Out( cmd , arg );
		}
		else ads_putChar(format[i]);
	}
}

void ads_printf(const char * format,...)
{
	va_list arg;
	va_start(arg, format);
	ads_vprintf(format,&arg);
	va_end(arg);
}

void ads_vsprintf(char * des,int size,const char*format,va_list * arg)
{
	ads_setOutputBuffer(des,size);
	ads_vprintf(format,arg);
	ads_setOutputBuffer(NULL,0);
}

void ads_sprintf(char * des,int size,const char*format,...)
{
	va_list arg;
	va_start(arg, format);
	ads_vsprintf(des,size,format,&arg);
	va_end(arg);
}


#endif

/** @endcond */
