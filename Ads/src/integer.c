
/** @cond ADS_INTERNAL */

#ifndef ADSIMPROVEDINTEGER_C
#define ADSIMPROVEDINTEGER_C

#include "../include/integer.h"

char RightNibble(char bcddigit)
{
	return bcddigit&15;
}

char LeftNibble(char bcddigit)
{
	return (bcddigit & 240)>>4;
}

char PackedBCD(char msb,char lsb)
{
	char bcd = 0;
	bcd |= RightNibble(lsb);
	bcd <<= 4;
	bcd |= RightNibble(msb);
	return bcd;
}


int __integer__altonum(char ch)
{
	int i = ch;
	if (i>=48 && i<= 57)
	{
		return i-48;
	}
	else
	{
		ads_registerError("character('%c') not in numeric range ascii(%d)",ch,ch);
		return -1;
	}
}

integer newintegerobject(const char *  num,int blocksize,int sign)
{
	if(blocksize == 1 && num[0] == 0) sign = 0;
	integer inte = (integer)ads_calloc(1,sizeof(structinteger));
	int8_t *tmp = (int8_t*)ads_calloc(blocksize,1);
	memcpy(tmp,num,blocksize);
	inte->data = tmp;
	inte->blocksize = blocksize;
	inte->sign = sign;
	return inte;
}

char PackedBCD9compliment(char a)
{
	char ls = 9 - PackedBCDLSB(a) ;
	char ms = 9 - PackedBCDMSB(a) ;
	return PackedBCD(ms,ls);
}

void nines_compliment( char* self,int size)
{
	int i;char z;
    for(i=0;i<size-1;i++)
    {
    	z = self[i];
        self[i] = PackedBCD9compliment(z);
    }
    z = self[i];
    self[i] = PackedBCD9compliment(z) ;
}

integer integer_NinesCompliment(integer a)
{
	char tmp[a->blocksize] ;
	memset(tmp,0,a->blocksize);
	memmove(tmp,a->data,a->blocksize);
	nines_compliment(tmp,a->blocksize);
	integer inte = newintegerobject(tmp,a->blocksize,0);
	return inte;
}


void freeinteger(integer self)
{
	ads_free(self->data);
	ads_free(self);
}

int integer_strip(char * tmp,int size,int sign)
{
	int i ;int cmp = (sign)?9:0;
	for(i=size-1;i>=0;i--)
	{
		if(RightNibble(tmp[i]) != cmp)
		{
			return i+1;
		}
		if(LeftNibble(tmp[i]) != cmp )
		{
			return i+1;
		}
	}
	return 1;
}

char PackedBCDAdd(char a,char b,char * carry)
{
	char ls = PackedBCDLSB(a) + PackedBCDLSB(b) + *carry;
	if(ls>9)ls += 6,*carry = LeftNibble(ls);
	else *carry =0;
	ls = RightNibble(ls);
	char ms = PackedBCDMSB(a) + PackedBCDMSB(b) + *carry;
	if(ms>9)ms += 6,*carry = LeftNibble(ms);
	else *carry =0;
	ms = RightNibble(ms);
	return PackedBCD(ms,ls);
}


void tens_compliment( char* self,int size)
{
	char t = PackedBCD(0,1);
    nines_compliment(self,size);
    char carry =0,a;int i;
    self[0] = PackedBCDAdd(self[0],t,&carry);
	for(i=1;i<size;i++)
	{
		a = self[i];
		self[i] = PackedBCDAdd(a,0,&carry);
	}
}

integer integer_TensCompliment(integer a)
{
	char tmp[a->blocksize] ;
	memset(tmp,0,a->blocksize);
	memmove(tmp,a->data,a->blocksize);
	tens_compliment(tmp,a->blocksize);
	/*if(RightNibble(a->data[a->blocksize-1]) == 0)*/
	/*tmp[a->blocksize-1] = getPackedBCD(0,LeftNibble(tmp[a->blocksize-1]));*/
	integer inte = newintegerobject(tmp,a->blocksize,0);
	return inte;
}

integer prepareSub(integer a)
{
	integer na = integer_TensCompliment(a);
	na->sign = 1^a->sign;
	return na;
}

integer newinteger(const char *  num)
{
	int sign = (num[0] == '-')? 1:0;
	int size = strlen(num),cbt=0,i,stop = sign;
	for(i=sign;i<size;i++)
	{
		if(__integer__altonum(num[i]) != 0 )
		{
			break;
		}
		stop = i;
	}
	int block = ceil((float)(size-stop)/2);
	block = (block == 0)?1:block;
	char tmp[block] ;
	memset(tmp,0,block);
	for(i=size-1;i>=stop;i-=2)
	{
		int msb,lsb;
		lsb = __integer__altonum(num[i]);
		msb = (i-1 >= 0+sign) ? __integer__altonum(num[i-1]):0;
		tmp[cbt++] = PackedBCD(msb,lsb);
	}
	if(sign == 1) tens_compliment(tmp,block);
	integer inte = newintegerobject(tmp,block,sign);
	return inte;
}

void printblockdata(char * tmp,int size)
{
	int i ;printf("\nBD = ");
	for(i=0;i<size;i++)
	{
		printf("%d%d",LeftNibble(tmp[i]),RightNibble(tmp[i]));
	}
}

void printinteger(integer self)
{
	int i ;char z;
	printf("\n");
	char tmp[self->blocksize];
	memset(tmp,0,self->blocksize);
	memmove(tmp,self->data,self->blocksize);
	if(self->sign == 1)printf("-"),tens_compliment(tmp,self->blocksize);
	i = self->blocksize-1;
	z = tmp[i];
	if(PackedBCDMSB(z)) printf("%d",PackedBCDMSB(z));
	printf("%d",PackedBCDLSB(z));
	for(i=i-1;i>=0;i--)
	{
		z = tmp[i];
		printf("%d",PackedBCDMSB(z));
		printf("%d",PackedBCDLSB(z));
	}
}

integer integer_abs(const integer self)
{
	char tmp[self->blocksize];
	memset(tmp,0,self->blocksize);
	memmove(tmp,self->data,self->blocksize);
	if(self->sign == 1) tens_compliment(tmp,self->blocksize);
	integer inte = newintegerobject(tmp,self->blocksize,0);
	return inte;
}

int integer_compare(const integer a , const integer b)
{
	if(a->sign < b->sign) return 1;
	if(a->sign > b->sign) return -1;
	if(a->blocksize>b->blocksize) return 1;
	if(a->blocksize<b->blocksize) return -1;
	int i;
	for(i=a->blocksize-1;i>=0;i--)
	{
		char ad = PackedBCDMSB(a->data[i]),bd = PackedBCDMSB(b->data[i]);
		if(ad>bd) return 1;
		if(ad<bd) return -1;
		ad = PackedBCDLSB(a->data[i]),bd = PackedBCDLSB(b->data[i]);
		if(ad>bd) return 1;
		if(ad<bd) return -1;
	}
	return 0;
}

integer integer_addition(const integer a ,const integer b)
{
	int sign = 0;
	int lena = a->blocksize,lenb = b->blocksize,i,maxbuf;
	maxbuf = (lena>=lenb)? lena : lenb ;
	char tmpc[maxbuf+1];
	memset(tmpc,0,maxbuf);char a1,b1;
	char carry =0;int pada = (a->sign)?0x99:0x00;int padb = (b->sign)?0x99:0x00;
	for(i=0;i<maxbuf;i++)
	{
		a1 = (i<a->blocksize)? a->data[i]:pada;
		b1 = (i<b->blocksize)? b->data[i]:padb;
		tmpc[i] = PackedBCDAdd(a1,b1,&carry);
	}
	if(a->sign == b->sign)
	{
		if(a->sign)
		{
			if(!carry)
			tmpc[i++] = PackedBCD(0,9-(!carry)),maxbuf++;
			sign = a->sign;
		}
		else
		{
			if(carry)
			tmpc[i++] = PackedBCD(0,carry),maxbuf++;
			sign = a->sign;
		}
	}
	else
	{
		sign = !carry;
	}
	maxbuf = integer_strip(tmpc,maxbuf,sign);
	integer inte = newintegerobject(tmpc,maxbuf,sign);
	return inte;
}

integer integer_subtraction(const integer a,const integer b)
{
	integer cb = prepareSub(b);
	integer sub = integer_addition(a,cb);
	freeinteger(cb);
	return sub;
}


#endif
/** @endcond  */
