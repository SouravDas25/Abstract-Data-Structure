
/** @cond ADS_INTERNAL */

#ifndef ADSUTILITY_C
#define ADSUTILITY_C

#include <stdio.h>

#include "../include/utility.h"




/*parse function*/

void* ads_parseInt(const char * stream,int len)
{
	int * x = (int*)ads_calloc(1,sizeof(int)) ;
	sscanf(ads_tempBuffer("%*s",len,stream),"%d",x);
	return x;
}

void* ads_parseInt64(const char * stream,int len)
{
	int64 * x = (int64*)ads_calloc(1,sizeof(int64)) ;
	sscanf(ads_tempBuffer("%*s",len,stream),"%I64d",x);
	return x;
}

void* ads_parseDouble(const char * stream,int len)
{
	double * c = (double*)ads_calloc(1,sizeof(double))  ;
	sscanf(ads_tempBuffer("%*s",len,stream),"%lf",c);
	return c;
}

void* ads_parseFloat(const char * stream,int len)
{
	float * c = (float*)ads_calloc(1,sizeof(float))  ;
	sscanf(ads_tempBuffer("%*s",len,stream),"%f",c);
	return c;
}

void* ads_parseBool(const char * stream,int len)
{
	bool * b =  (bool*)ads_calloc(1,sizeof(bool)) ;
	int i;
	for(i=0;i<len;i++)
	{
		if( isalpha(stream[i]) )
		{
			if( stream[i] == 't')
			{
				*b = true;
			}
			else if( stream[i] == 'f' )
			{
				*b = false;
			}
			break;
		}
	}
	return b;
}

bool ads_isEndBracket(int c)
{
    switch(c)
    {
        case ')':
        	return true;
        case '}':
        	return true;
        case ']':
        	return true;
		case '>':
        	return true;
        default:
        	return false;
    }
}

bool ads_isStartBracket(int c)
{
    switch(c)
    {
        case '(':
        	return true;
        case '{':
        	return true;
        case '[':
        	return true;
        case '<':
        	return true;
        default :
        	return false;
    }
}

char ads_opBracket(char c)
{
    switch(c)
    {
        case ')':
        return '(';
        case '}':
        return '{';
        case ']':
        return '[';
        case '(' :
        return ')';
        case '{':
        return '}';
        case '[':
        return ']';
        default :
            ads_registerError("character(\"%c\") not found ",c);
            return 0;
    }
    ads_registerError("character(\"%c\") not found ",c);
    return 0;
}

/*reprfunction*/


/*takes the address of the variable and send it to stdio stream */

void ads_printChar(void*add)
{
	ads_output("'%c'",*(char*)add);
}

void ads_printInt64(void*add)
{
	ads_output("%I64d",*(int64_t*)add);
}

void ads_printInt(void*add)
{
	ads_output("%d",*(int*)add);
}

void ads_printFloat(void*add)
{
	ads_output("%f",*(float*)add);
}

void ads_printDouble(void*add)
{
	ads_output("%f",*(double*)add);
}

void ads_printBool(void* a)
{
	if( (*(bool*)a) == true) ads_output("true");
	else ads_output("false");
}

/*compare*/
/*take the address of two variable and return int as compared value*/

int ads_cmpChar(const void*a,const void*b)
{
	const char x= *(const char*)a,y=*(const char*)b;
	if(x>y)return 1;
	if(x<y)return -1;
	return 0;
}

int ads_cmpInt(const void*a,const void*b)
{
	const int x= *(const int*)a,y=*(const int*)b;
	if(x>y)return 1;
	if(x<y)return -1;
	return 0;
}

int ads_cmpInt64(const void*a,const void*b)
{
	const int64_t x= *(const int64_t*)a,y=*(const int64_t*)b;
	if(x>y)return 1;
	if(x<y)return -1;
	return 0;
}

int ads_cmpFloat(const void*a,const void*b)
{
	const float x= *(float*)a,y=*(float*)b;
	if(x>y)return 1;
	if(x<y)return -1;
	return 0;
}

int ads_cmpDouble(const void*a,const void*b)
{
	const double x= *(const double*)a,y=*(const double*)b;
	if(x>y)return 1;
	if(x<y)return -1;
	return 0;
}

int ads_appropriateCmp(int (*comparefunction)(const void*,const void*),const void*data1,const void*data2,int datatype)
{
	int c ;
	if(comparefunction != ads_cmpMem)
	{
		c = comparefunction(data1,data2);
	}
	else c = memcmp(data1,data2,datatype);
	return c;
}

/*utility*/

int64_t ads_int64Abs(int64_t n)
{
    if(n<0) return n*-1;
    return n;
}

int64_t ads_roundOff(double number)
{
    double d = number - floor(number);
    if(d<0.5) return floor(number);
    return ceil(number);
}

bool ads_isPrime(int n)
{
	int i,sqr;
	if((n & 0x1)==0) return false;
	sqr = sqrt(n);
	for(i=3;i<=sqr;i+=2)
	{
		if(n%i==0) return false;
	}
	return true;
}

int ads_nextPrime(int start) /*return the prime number after the given number*/
{
	int i;
	for(i=start+1;;i++)
	{
		if(ads_isPrime(i)==true) return i;
	}
	return -1;
}

int ads_previousPrime(int start) /*return the prime number before the given number*/
{
	int i;
	for(i=start;i>2;i--)
	{
		if(ads_isPrime(i)==true) return i;
	}
	return 2;
}

uint64_t ads_hashInt(const void*add)
{
	uint64_t x = *(unsigned int*)add;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
}

uint64_t ads_hashInt64( const void*add)
{
	uint64_t x = *(uint64_t*)add;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
}

uint64_t ads_hashDouble(const void*add)
{
	double d =*(double*)add;
	if(d == 0) return 0;
	else
	{
		int exponent;
		double mantissa = frexp(d,&exponent);
		return (2*fabs(mantissa)-1)*~exponent;
	}
}

uint64_t ads_hashFloat(const void*add)
{
	double d =*(float*)add;
	if(d == 0) return 0;
	else
	{
		int exponent;
		double mantissa = frexp(d,&exponent);
		return (2*fabs(mantissa)-1)*~exponent;
	}
}


#endif
/** @endcond  */
