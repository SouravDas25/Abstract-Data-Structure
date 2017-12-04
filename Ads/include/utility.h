



#ifndef ADS_UTILITY_H
#define ADS_UTILITY_H

#include "utility\compiler_info.h"

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

#ifdef __cplusplus
extern "C" {
#endif

#include "utility\error.h"
#include "utility\datareplicator.h"
#include "utility\gc.h"
#include "utility\iterator.h"
#include "utility\defaultargs.h"
#include "utility\object.h"
#include "utility\Types.h"
#include "utility\c_str.h"
#include "utility\ads_stream.h"

#include "io\lnlist.h"
#include "io\stdout.h"

#define ads_left true
#define ads_right false

#define ads_endOfStr '\0'

typedef int64_t int64;
typedef uint64_t uint64;
typedef int (*ads_cmpFuncType) (const void*,const void*); /**< ads compare function signature. */


/**
 * @addtogroup Parsing
 * @{
 * @brief ads buffer parsing
 *
 */
void* ads_parseInt(const char * stream,int len);    /**< returns a dynamically allocated int containing the value of the buffer. */
void* ads_parseInt64(const char * stream,int len);  /**< returns a dynamically allocated int64 containing the value of the buffer. */
void* ads_parseDouble(const char * stream,int len); /**< returns a dynamically allocated double containing the value of the buffer. */
void* ads_parseFloat(const char * stream,int len);  /**< returns a dynamically allocated float containing the value of the buffer. */
void* ads_parseBool(const char * stream,int len);   /**< returns a dynamically allocated bool containing the value of the buffer. */
/** @} */



/**
 * @addtogroup Misc
 * @{
 * @brief ads utility functions.
 *
 */
bool ads_isEndBracket(int c);           /**< returns true if the char is a ending bracket( ')',']','}' ). */
bool ads_isStartBracket(int c);         /**< returns true if the char is a starting bracket( '(','[','{' ). */
char ads_opBracket(char c);             /**< returns the opposite bracket of the given bracket. */
int64_t ads_int64Abs(int64_t n);        /**< returns the absolute value of a int64 */
int64_t ads_roundOff(double number);    /**< returns the round off of a float */
bool ads_isPrime(int n) ;               /**< returns true if the number is a prime number */
int ads_nextPrime(int start);           /**< returns the prime number after the given number */
int ads_previousPrime(int start);       /**< returns the prime number before the given number */
/** @} */


/**
 * @addtogroup Repr-Printing
 * @{
 * @brief ads utility functions.
 *
 */
void ads_printChar(void*add);   /**< prints the char at the address */
void ads_printInt(void*add);    /**< prints the integer at the address */
void ads_printInt64(void*add);  /**< prints the int64 at the address */
void ads_printFloat(void*add);  /**< prints the float at the address */
void ads_printDouble(void*add); /**< prints the double at the address */
void ads_printBool(void* a);    /**< prints the bool at the address */
/** @} */

/**< @cond ADS_INTERNAL */
#define ads_printarray(arr,n,printfunction) { printf("\nArray( "); int __PRINTARRAYLOOPVARI__; for( __PRINTARRAYLOOPVARI__ = 0;\
__PRINTARRAYLOOPVARI__< n;__PRINTARRAYLOOPVARI__++){ printfunction(&arr[__PRINTARRAYLOOPVARI__]);\
if(__PRINTARRAYLOOPVARI__ < n-1)printf(" , "); } printf(" )\n");}
/**< @endcond */

/**
 * @addtogroup Compare-Functions
 * @{
 * @brief ads compare functions.
 *
 */
int ads_cmpChar(const void*a,const void*b);/**< compares two char's from their address */
int ads_cmpInt(const void*a,const void*b);/**< compares two int's from their address */
int ads_cmpInt64(const void*a,const void*b);/**< compares two int64's from their address */
int ads_cmpFloat(const void*a,const void*b);/**< compares two float's from their address */
int ads_cmpDouble(const void*a,const void*b);/**< compares two double's from their address */
/** @} */

/**< @cond ADS_INTERNAL */
int ads_appropriateCmp(int (*comparefunction)(const void*,const void*),const void*data1,const void*data2,int datatype);
/**< @endcond */

/**
 * @addtogroup Hash-Functions
 * @{
 * @brief ads hash functions.
 *
 */
uint64_t ads_hashInt(const void*add); /**< produces a hash value of a int */
uint64_t ads_hashInt64(const void*add);/**< produces a hash value of a int64 */
uint64_t ads_hashFloat(const void*add);/**< produces a hash value of a float */
uint64_t ads_hashDouble(const void*add);/**< produces a hash value of a double */
/** @} */

#ifdef __cplusplus
}
#endif

#endif


