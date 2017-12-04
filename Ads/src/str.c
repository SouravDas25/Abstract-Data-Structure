

/** @cond ADS_INTERNAL */


#ifndef ADSIString_C
#define ADSIString_C

#include "../include/str.h"

ads_class(ads_str)
{
	ads_init(ads_str);
	ads_structString * data;
};

ads_linkClass(ads_str,ads_strCopy,ads_strDelete);

static ads_str ads_initString(const char*oldstr)
{
	ads_str newstr = ads_rawObject(ads_str);
	newstr->data = ads_newStringObj(oldstr);
	if(!ADS_MethodVariableName(ads_str).TypeCode) {
		ADS_MethodVariableName(ads_str).TypeCode = ads_genericIoInsert(ads_printString,"'str'",0,0);
		ADS_MethodVariableName(ads_str).TypeCode = ads_genericIoInsert(ads_sprintString,"str",0,0);
	}
	return newstr;
}

ads_str ads_strNew(const char * format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return ads_initString(__formatbufers1__);
}

static const char* __getcstring(const ads_str self)
{
	return ads_structStringData(self->data);
}

const char* ads_strGetCstr(const ads_str self)
{
	return ads_structStringData(self->data);
}

char* ads_strToCstr(const ads_str self,char * destinaton)
{
    if(!destinaton)destinaton = (char*)ads_calloc(ads_strLen(self),sizeof(char));
	strcpy(destinaton,__getcstring(self));
	return destinaton;
}

ads_str ads_strCopy(ads_str _self)
{
	ads_str self;
	ads_CheckAndCastPointer(_self,ads_str,self);
	ads_str newstr = ads_rawObject(ads_str);
	newstr->data = ads_copyStringObj(self->data);
	return newstr;
}

void* ads_strCopy_ByRef_RtnRef(const void * s)
{
    ads_str ns = ads_strCopy(*(ads_str*)s);
    return ads_toItem(ns);
}

const ads_td const ads_strTD_Struct = {ads_strCopy_ByRef_RtnRef,ads_strDelete_ByRef,sizeof(ads_str)};

const ads_td * const ads_strTD = &ads_strTD_Struct;

void ads_sprintString(void*add,const char * cmd,int size)
{
    add = *(void**)add;
	cmd = cmd;
	size = size;
	ads_str self;
	ads_CheckAndCastPointer(add,ads_str,self);
	ads_output("%s",__getcstring(self));
}

void ads_printString(void*add,const char * cmd,int size)
{
    add = *(void**)add;
	cmd = cmd;
	size = size;
	ads_str self;
	ads_CheckAndCastPointer(add,ads_str,self);
	ads_output("'%s'",__getcstring(self));
}

void ads_printUnicodeString(void* _self)
{
	ads_str self;
	ads_CheckAndCastPointer(_self,ads_str,self);
	const wchar_t * w = (wchar_t *)__getcstring(self);
	wprintf(w);
}

void ads_strDelete(ads_str _self)
{
	ads_str self;
	ads_CheckAndCastPointer(_self,ads_str,self);
	ads_freeStringObj(self->data);
	ads_free(self);
}

void ads_strDelete_ByRef(void*s)
{
    ads_strDelete(ads_fromItem(s));
}


int ads_strCmp(const ads_str self,const char* format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return strcmp(__getcstring(self),__formatbufers1__);
}

int ads_cmpString( void* _self, void* _other)
{
	ads_str self;
	ads_CheckAndCastPointer(_self,ads_str,self);
	ads_str other;
	ads_CheckAndCastPointer(_other,ads_str,other);
	return strcmp(__getcstring(self),__getcstring(other));
}

int ads_strLen(const ads_str self)
{
	return ads_structStringLen(self->data);
}

int ads_strSizeOf(const ads_str self)
{
	return ads_sizeOfStructString(self->data)+sizeof(ADS_StructOf(ads_str));
}

#define __stringfindexcstring(self,index,ret) if(index >= (long long)strlen(self) || index < 0) {ads_registerError("In index = %d ",index);return ret;}

#define __stringfindexstring(self,index,ret) if(index>=ads_strLen(self)||index<0){ads_registerError("In index = %d ",index);return ret;}

void ads_strSet(ads_str self,const char * format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	ads_freeStringObj(self->data);
	self->data = ads_newStringObj(__formatbufers1__);
}

void ads_strCat(ads_str self,const char* format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);

	int len1 = ads_strLen(self),len2 = strlen(__formatbufers1__);
	char buf[len1+len2+2];
	strcpy(buf,__getcstring(self));
	strcat(buf,__formatbufers1__);
	ads_strSet(self,buf);
}

char ads_strCharAt(ads_str self,int index)
{
	__stringfindexstring(self,index,-1);
	return __getcstring(self)[index];
}

ads_str ads_cstrGetSlice(const char* self,int lowerlimit,int upperlimit)
{
	if (upperlimit < 0 ) upperlimit = strlen(self)-1;
	else __stringfindexcstring(self,upperlimit,ads_initString(""));
	__stringfindexcstring(self,lowerlimit,ads_initString(""));
	int len = (upperlimit - lowerlimit) + 1;
	char tmp[len+1];
	int i,p=0;
	for(i=lowerlimit;i<=upperlimit;i++)
	{
		tmp[p++] = self[i];
	}
	tmp[p] = ads_endOfStr;
	return ads_initString(tmp);
}

ads_str ads_strGetSlice(const ads_str self,int lowerlimit ,int upperlimit )
{
	return ads_cstrGetSlice(__getcstring(self),lowerlimit,upperlimit);
}

ads_rt ads_strSetSlice(const ads_str self,int lowerlimit,int upperlimit,const char * format,... )
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);

	__stringfindexstring(self,upperlimit,ads_ERROR);
	__stringfindexstring(self,lowerlimit,ads_ERROR);
	int len1 = strlen(__formatbufers1__),cou =0,i,len = ads_strLen(self);
	char data[len1+len+1];
	for(i =0;i<lowerlimit;i++)
	{
		data[cou++] = __getcstring(self)[i];
	}
	for(i=0;i<len1;i++)
	{
		data[cou++] = __formatbufers1__[i];
	}
	for(i =upperlimit+1;i<len;i++)
	{
		data[cou++] = __getcstring(self)[i];
	}
	data[cou++] = ads_endOfStr;
	ads_strSet(self,data);
	return ads_SUCCESS;
}

void ads_strCapitalize(const ads_str self)
{
	int i,len = ads_strLen(self);
	char data[len+1];
	strcpy(data,__getcstring(self));
	for (i=0;i<len;i++)
	{
		if (isalpha(data[i]) != 0)
		{
			data[i]= toupper(data[i]);
			break;
		}
	}
	ads_strSet(self,data);
}

int ads_cstrCount(const char* self,const char*other)
{
	int i,len = strlen(self),len2 = strlen(other),p,con=0;
	for(i=0;i<len;i++)
	{
		if(self[i]==other[0])
		{
			int k = i,g  = false;
			for(p = 0;p<len2;p++)
			{
				if(other[p] != self[k++])
				{
					g = true;
					break;
				}
			}
			if (g== false)
			{
				i+=len2;
				con++;
			}
		}
	}
	return con;
}

int ads_strCount(ads_str self,const char* format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);

	return ads_cstrCount(__getcstring(self),__formatbufers1__);
}

int ads_cstrFind(const char* self,const char*other)
{
	int i,len = strlen(self),len2 = strlen(other),p,con=false;
	for(i=0;i<len;i++)
	{
		if(self[i]==other[0])
		{
			int k = i,g  = false;
			for(p = 0;p<len2;p++)
			{
				if(other[p] != self[k++])
				{
					g = true;
					break;
				}
			}
			if (g== false)
			{
				con = true;
				break;
			}
		}
	}
	if(con == true)
	{
		return i;
	}
	else
	{
		return -1;
	}
}

int ads_strFind(ads_str self,const char* format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return ads_cstrFind(__getcstring(self),__formatbufers1__);
}

bool ads_cstrContains(const char* self,const char* other)
{
	if(ads_cstrFind(self,other) != -1)
	{
		return true;
	}
	return false;
}

bool ads_strContains(ads_str self,const char*format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return ads_cstrContains(__getcstring(self),__formatbufers1__);
}

int ads_cstrIndex(const char*self,const char*other)
{
	int l = ads_cstrFind(self,other);
	if(l != -1)
	{
		return l;
	}
	else
	{
		ads_registerError("Substring '%s' Not Found in ads_str '%s' ",other,self);
		return -1;
	}
}

int ads_strIndex(ads_str self,const char* format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return ads_cstrIndex(__getcstring(self),__formatbufers1__);
}

bool ads_cstrIsAlnum(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(isalnum(self[i]) == false)
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsAlnum(ads_str self)
{
	return ads_cstrIsAlnum(__getcstring(self));
}

bool ads_cstrIsAlpha(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(isalpha(self[i]) == false)
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsAlpha(ads_str self)
{
	return ads_cstrIsAlpha(__getcstring(self));
}

bool ads_cstrIsDouble(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(isdigit(self[i]) == false && self[i] != '.')
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsDouble(ads_str self)
{
	return ads_cstrIsDouble(__getcstring(self));
}

bool ads_cstrIsInteger(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(isdigit(self[i]) == false )
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsInteger(ads_str self)
{
	return ads_cstrIsInteger(__getcstring(self));
}

bool ads_cstrIsUpper(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(isupper(self[i]) == false)
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsUpper(ads_str self)
{
	return ads_cstrIsUpper(__getcstring(self));
}

bool ads_cstrIsLower(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(islower(self[i]) == false)
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsLower(ads_str self)
{
	return ads_cstrIsLower(__getcstring(self));
}

bool ads_cstrIsSpace(const char* self)
{
	int i,len = strlen(self);
	if(len == 0)
	{
		return false;
	}
	for (i=0;i<len;i++)
	{
		if(isspace(self[i]) == false)
		{
			return false;
		}
	}
	return true;
}

bool ads_strIsSpace(ads_str self)
{
	return ads_cstrIsSpace( __getcstring(self));
}

void ads_strToLower(const ads_str self)
{
	int i,len = ads_strLen(self);
	char data[len+1];
	for (i=0;i<len;i++)
	{
		data[i] = tolower(__getcstring(self)[i]);
	}
	data[len] = ads_endOfStr;
	ads_strSet(self,data);
}

void ads_strToUpper(const ads_str self)
{
	int i,len = ads_strLen(self);
	char data[len+1];
	for (i=0;i<len;i++)
	{
		data[i] = toupper(__getcstring(self)[i]);
	}
	data[len] = ads_endOfStr;
	ads_strSet(self,data);
}


ads_rt ads_cstrReplace(const ads_str self,const char*other,const char*newstr)
{
	if (ads_strContains(self,other) == false)
	{
		return ads_registerError("substring %s Not Found.",other);
	}
	int i,len = ads_strLen(self),len2 = strlen(other),p,con=0,count = ads_strCount(self,other),len3 =strlen(newstr) ;
	int sz = len + 1 + ((len3-len2)*count);
	if (sz <=0)
	{
		return ads_registerError("Cannot replace the substring");
	}
	char tmp [sz];
	for(i=0;i<sz;)
	{
		if(__getcstring(self)[con]==other[0])
		{
			int k = con,g  = false;
			for(p = 0;p<len2;p++)
			{
				if(other[p] != __getcstring(self)[k++])
				{
					g = true;
					break;
				}
			}
			if (g == false)
			{
				for(p = 0;p<len3;p++)
				{
					tmp[i++] = newstr[p];
				}
				con += len2;
				continue;
			}
		}
		tmp[i++] = __getcstring(self)[con++];
	}
	tmp[i]=ads_endOfStr;
	ads_strSet(self,tmp);
	return ads_SUCCESS;
}

void ads_strReplace(ads_str self,ads_str other,ads_str newstr)
{
	ads_cstrReplace(self,__getcstring(other),__getcstring(newstr));
}

void ads_strStrip(const ads_str self)
{
	int i,len = ads_strLen(self);
	int k=0,lspacecount=0,rspacecount=0;
	char tmp[len+1];
	for(i=0;i<len;i++)
	{
		if(isspace(__getcstring(self)[i]) == false)
		{
			break;
		}
		lspacecount++;
	}
	for(i=len-1;i>=0;i--)
	{
		if(isspace(__getcstring(self)[i]) == false)
		{
			break;
		}
		rspacecount++;
	}
	for(i=lspacecount;i<len-rspacecount;i++)
	{
		tmp[k++]= __getcstring(self)[i];
	}
	tmp[k]=ads_endOfStr;
	ads_strSet(self,tmp);
}

void ads_strSwapCase(const ads_str self)
{
	int i,len = ads_strLen(self);
	char data[len+1];
	for(i=0;i<len;i++)
	{
		if(isupper(__getcstring(self)[i]))
		{
			data[i] = tolower(__getcstring(self)[i]);
		}
		else if(islower(__getcstring(self)[i]))
		{
			data[i] = toupper(__getcstring(self)[i]);
		}
		else
		{
			data[i] = __getcstring(self)[i];
		}
	}
	data[i] = ads_endOfStr;
	ads_strSet(self,data);
}

ads_vector ads_cstrSplit(const char* str,const char* format,...)
{
	char sep[32];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(sep,32,format,&ap);
	va_end(ap);

	int i = 0,prev = -1,len = strlen(str);
	ads_vector  l1 = ads_vectorNew(ads_str,4);
	ads_vectorSetTD(l1,ads_strTD);
	ads_str s  ;
	char tmp[2] = {0,0};
	for(i=0;i<len+1;i++)
	{
		tmp[0] = str[i];
		if(ads_cstrContains(sep,tmp) == true ||  str[i] == ads_endOfStr)
		{
			if(i-prev > 1)
			{
				if(prev != 0) prev++;
				s = ads_cstrGetSlice(str,prev,i-1);//ads_printf("%'str'",s)
				ads_vectorAppend(l1,s,ads_str);
				ads_strDelete(s);
			}
			prev = i;
		}
	}
	ads_vectorShrink(l1);
	return l1;
}

ads_vector ads_strSplit(const ads_str self, const char *format,...)
{
	char sep[32];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(sep,32,format,&ap);
	va_end(ap);

	ads_vector l = ads_cstrSplit(__getcstring(self),sep);
	return l;
}


ads_str ads_cstrJoin(ads_vector l1,const char * delimiter)
{
	ads_str v;
	int i = 0,len = ads_vectorLen(l1),sum=0,delimlen = strlen(delimiter);
	for(i=0;i<len;i++)
	{
		ads_CheckAndCastPointer(*(ads_str*)ads_vectorGetItem_NoCopy_RtnRef(l1,i),ads_str,v);
		sum+= ads_strLen(v);
	}
	char data[sum+((len-1)*delimlen)+1];
	data[0] = ads_endOfStr;
	ads_str kil = *(ads_str*)ads_vectorGetItem_NoCopy_RtnRef(l1,0);
	strcat(data,__getcstring(kil));
	for(i=1;i<len;i++)
	{
		strcat(data,delimiter);
		kil = *(ads_str*)ads_vectorGetItem_NoCopy_RtnRef(l1,i);
		strcat(data,__getcstring(kil));
	}
	return ads_strNew(data);
}

ads_str ads_strJoin(ads_vector l1,const ads_str delimiter)
{
	return ads_cstrJoin(l1,__getcstring(delimiter));
}

ads_rt ads_strMul(const ads_str self,int multiplier)
{
	if(multiplier <= 0)
	{
		return ads_registerError("multiplier should be greater than 0 got %d",multiplier);
	}
	int len = ads_strLen(self),i;
	char data[len*multiplier+1] ;
	strcpy(data,__getcstring(self));
	for(i= 0;i< multiplier-1 ;i++)
	{
		strcat(data,__getcstring(self));
	}
	ads_strSet(self,data);
	return ads_SUCCESS;
}

void ads_strCentered(const ads_str self,int width,const char  fill )
{
	int i,len = ads_strLen(self),pad = (width-len)/2,con = 0;
	char data[width];
	for(i=0;i<pad;i++)
	{
		data[con++]= fill;
	}
	for(i=0;i<len;i++)
	{
		data[con++]= __getcstring(self)[i];
	}
	for(i=0;i<pad;i++)
	{
		data[con++]= fill;
	}
	while(con < width)
	{
		data[con++]= fill;
	}
	data[con] = ads_endOfStr;
	ads_strSet(self,data);
}

void ads_strZFill(const ads_str self,int width)
{

	int i, len = ads_strLen(self);
	if(width<len)
	{
		width = len;
	}
	char data[width];
	for(i= 0;i<width-len;i++)
	{
		data[i] = '0';
	}
	data[i]=ads_endOfStr;
	strcat(data,__getcstring(self));
	ads_strSet(self,data);
}

void ads_strReverse(const ads_str self)
{
	int size = ads_strLen(self),i,con=0;
	char data[size];
	for(i=size-1;i>=0;i--)
	{
		data[con++] = __getcstring(self)[i];
	}
	data[con]=ads_endOfStr;
	ads_strSet(self,data);
}

bool ads_cstrEndWith(const char * self,const char * substring)
{
	int i ,len = strlen(substring),len2 = strlen(self);
	for(i = len2-len; i < len2;i++)
	{
		if(self[i] != substring[i-(len2-len)])
		return false;
	}
	return true;
}

bool ads_strEndWith(ads_str self,const char * format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return ads_cstrEndWith(__getcstring(self),__formatbufers1__);
}

bool ads_cstrStartWith(const char *self,const char * substring)
{
	int i ,len = strlen(substring);
	for(i = 0; i < len;i++ )
	{
		if(self[i] != substring[i])
		return false;
	}
	return true;
}

bool ads_strStartWith(ads_str self,const char * format,...)
{
	char __formatbufers1__[ads_BUFSIZ];
	va_list ap ;
    va_start(ap,format);
	ads_vsprintf(__formatbufers1__,ads_BUFSIZ,format,&ap);
	va_end(ap);
	return  ads_cstrStartWith(__getcstring(self),__formatbufers1__);
}

uint64_t ads_strHash(ads_str _self)
{
	ads_str self = (ads_str)_self;
	return ads_cstrHash(__getcstring(self));
}

uint64_t ads_strHash_ByRef(void** _self)
{
	ads_str self = *(ads_str*)_self;
	return ads_cstrHash(__getcstring(self));
}

static int numtoal(int ch)
{
	int i = ch;
	if (i>=10 && i <= 35)
	{
		return i+87;
	}
	else if (i>=0 && i<= 9)
	{
		return i+48;
	}
	else
	{
		ads_registerError("character('%c') not a numeric character ascii(%d)",ch,ch);
		return -1;
	}
}

static int altonum(char ch)
{
	int i = ch;
	if(i>=97 && i <=122)
	{
		return i-87;
	}
	else if (i>=65 && i<=90)
	{
		return i-55;
	}
	else if (i>=48 && i<= 57)
	{
		return i-48;
	}
	else
	{
		ads_registerError("character('%c') not in numeric range ascii(%d)",ch,ch);
		return -1;
	}
}

int64_t ads_cstrToInt64(const char * src ,int base)
{
	if(base < 2 || base > 36)
	{
		ads_registerError("base %d is not valid values between (2-36) is allowed",base);
		return 0;
	}
	int sign = 1;
	if( *src == '-' )sign=-1,src++;
	int i,len = strlen(src),alpha;int64_t con = 0;
	for(i=len-1;i>=0;i--)
	{
		alpha = altonum(src[len-i-1]);
		if( alpha >= base)
		{
			ads_registerError("character('%c') not in recognised in base %d",src[len-i-1],base);
			return 0;
		}
		con += (alpha) * ads_roundOff(pow(base,i));
	}
	return con*sign;
}

int64_t ads_strToInt64(ads_str self,int base )
{
	return ads_cstrToInt64(__getcstring(self),base);
}

ads_str ads_strFrmInt64(int64_t num,int base)
{
	if(base < 2 || base > 36)
	{
		ads_registerError("base %d is not valid values between (2-36) is allowed",base);
		return ads_strNew("");
	}
	char rec[20+1];
	int sign = (num<0)?1:0;
	int64_t b,a = ads_int64Abs(num);
    int i=0;
	do{
		b = a%base;
        a = a/base;
        b = numtoal(b);
        rec[i++]=b;
	}while(a>=base-1);
	if(a>0) rec[i++]= numtoal(a);
	if(sign)rec[i++]='-';
	rec[i]= ads_endOfStr;
	ads_str s = ads_strNew(rec);
	ads_strReverse(s);
	return s;
}

ads_str ads_strFrmInt(int num,int base )
{
	return ads_strFrmInt64(num,base);
}

double ads_cstrToDouble(const char* self ,int base )
{
	if( base < 2 || base > 36 )
	{
		ads_registerError("base %d is not valid, values between (2-36) is allowed",base);
		return 0;
	}
	ads_vector l = ads_cstrSplit(self,".");
	if(ads_vectorLen(l) < 1)
	{
		ads_registerError("Provided String dosn't contain a floating point number");
		return 0;
	}
	int64_t ipart = ads_strToInt64(*(ads_str*)ads_vectorGetItem_NoCopy_RtnRef(l,0),base);const char * src;
	if( ads_vectorLen(l) == 1)
	{
		src = "0";
	}
	else
	{
		src = __getcstring((*(ads_str*)ads_vectorGetItem_NoCopy_RtnRef(l,1)));
	}
	int i,len = strlen(src),alpha;double con = 0.0;
	for(i=0;i<len;i++)
	{
		alpha = altonum(src[i]);
		if( alpha >= base )
		{
			ads_registerError("character('%c') not in recognised in base %d",src[len-i-1],base);
			ads_vectorDelete(l);
			return 0;
		}
		con += (double)alpha * pow(base,(i+1)*-1);
	}
	con += (double)ipart ;
	ads_vectorDelete(l);
	return con;
}

double ads_strToDouble(const ads_str self ,int base )
{
	return ads_cstrToDouble(__getcstring(self),base);
}

ads_str ads_strFrmDouble(double num ,int base,int digit)
{
	int64_t ipart = (int64_t)num, n;
	double fpart = num  - (double)ipart  ;
	int i;char tmp[digit+1];
	n = (int64_t)fpart ;
	fpart = fpart  - (double)n;
	for(i=0;i<digit;i++)
	{
		fpart = fpart* base;
		n = (int64_t)fpart ;
		fpart = fpart  - (double)n;
		tmp[i] = numtoal(n);
	}
	tmp[i]=ads_endOfStr;
	ads_str s = ads_strFrmInt64(ipart,base);
	ads_strCat(s,".{s}",tmp);
	return s;
}

ads_str ads_hexadecimal(int64_t n)
{
	ads_str s = ads_strFrmInt64(n,16);
	return s;
}

ads_str ads_binary(int64_t n)
{
	ads_str s = ads_strFrmInt64(n,2);
	return s;
}

void* __stringiterstart(ads_iterator* i)
{
	ads_str s = (ads_str)i->object;
	i->object = (void*)__getcstring(s);
	return i->object;
}

void* __stringiternext(ads_iterator* i)
{
	if(i->object != NULL)
	{
		char *c = ((char*)i->object);
		i->object = ++c;
		if(*c != ads_endOfStr) return i->object;
		return NULL;
	}
	ads_registerError("overflow in Stringiterator");
	return NULL;
}

ads_iterator ads_strIterator(ads_str self)
{
	ads_iterator i = ads_initIterator(self,__stringiternext,__stringiterstart,ads_strLen(self),NULL);
	return i;
}

ads_str ads_getLineInput(const char * prompt)
{
	fflush(stdin);
	ads_output(prompt);
	char buffer[ads_BUFSIZ];
	gets(buffer);
	return ads_strNew(buffer);
}

void* ads_strParse(const char * stream,int len)
{
	int i ,start=-1,end=-1;
	for(i=0;i<len;i++)
	{
		if( stream[i] == '\'' || stream[i] == '"'  )
		{
			start = i+1;
			break;
		}
	}
	for(i=len;i>=0;i--)
	{
		if( stream[i] == '\'' || stream[i] == '"'  )
		{
			end = i-1;
			break;
		}
	}
	if(start<0 || end <0 ) return ads_strNew("");
	return ads_cstrGetSlice(stream,start,end);
}

ads_str ads_strRand(int size)
{
    static bool dne ;
    if(!dne)srand(time(NULL));
    dne = 1;
    int i ;
    char s[size+1];
    for(i=0;i<size;i++)
    {
        s[i] = rand()%(27) + 65;
    }
    s[size] = ads_endOfStr;
    return ads_strNew(s);
}

#undef __stringfindexcstring
#undef __stringfindexstring

#endif
/** @endcond  */
