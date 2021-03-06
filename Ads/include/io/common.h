


#ifndef ADS_IO_COMMON_H
#define ADS_IO_COMMON_H

#include "../utility.h"
#include<stdbool.h>

/** @brief returns true if a comma is present in the buffer ads_str.
 *
 * @param cmd const char* the buffer start address
 * @param size int the length of the buffer
 * @return bool
 *
 * returns true if and only if comma is present outside of any kind of brackets.
 * - E.G.
 *      - "1,2,3,4" => true.
 *      - "1[2,3,6]" => false
 *      - "1,[2,3,4],5 => true
 */
bool ads_isCommanPresent(const char * cmd,int size);

/** @brief returns the non-zero int if it is a valid ads_str formatting.
 *
 * @param format const char*
 * @param adi int*
 * @return int
 *
 * recognize "%d" and "{d}" style formatting.
 */
int ads_isValidFormatting(const char* format,int * adi);

/** @cond ADS_INTERNAL */
const char* ads_getCMD(const char * format,int* i);
const char* ads_getOldCMD(const char * format,int* i);
/** @endcond */

#endif

