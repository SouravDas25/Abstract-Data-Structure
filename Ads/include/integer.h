/** @cond ADS_INTERNAL */

#ifndef ADSIMPROVEDINTEGER_H
#define ADSIMPROVEDINTEGER_H

#include "utility.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

struct structinteger
{
	int8_t * data;
	short int sign;
	short int blocksize;
};

typedef struct structinteger structinteger;
typedef structinteger* integer;

char RightNibble(char bcddigit);

char LeftNibble(char bcddigit);

#define PackedBCDMSB RightNibble
#define PackedBCDLSB LeftNibble

char PackedBCD(char msb,char lsb);

int __integer__altonum(char ch);

integer newintegerobject(const char *  num,int blocksize,int sign);

char PackedBCD9compliment(char a);

void nines_compliment( char* self,int size);

integer integer_NinesCompliment(integer a);

void freeinteger(integer self);

int integer_strip(char * tmp,int size,int sign);

char PackedBCDAdd(char a,char b,char * carry);

void tens_compliment( char* self,int size);

integer integer_TensCompliment(integer a);

integer prepareSub(integer a);

integer newinteger(const char *  num);

void printblockdata(char * tmp,int size);

void printinteger(integer self);

integer integer_abs(const integer self);

int integer_compare(const integer a , const integer b);

integer integer_addition(const integer a ,const integer b);

integer integer_subtraction(const integer a,const integer b);

#ifdef __cplusplus
}
#endif

#endif
/** @endcond  */
