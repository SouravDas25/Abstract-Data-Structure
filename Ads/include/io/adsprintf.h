
/** @file adsprintf.h
 * @brief a data structure printing functions.
 *
 */


#ifndef ADS_PRINTF_H
#define ADS_PRINTF_H

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>


#include "lnlist.h"
#include "common.h"
#include "gpf.h"

/** @cond ADS_INTERNAL */
ads_iList ads_getTypeCode(const char * cmd,int *size ,int * cutoff);
ads_iList ads_getPrintFunction(const char * cmd,int * size,int * cutoff);
/** @endcond */

/** @brief the standard printing function for ads data types.
 *
 * @param format const char* the format of data
 * @param ... the variables to format.
 * @return void
 *
 */
void ads_printf(const char * format,...);

/** @brief ads printf with valist argument.
 *
 * @param format const char*
 * @param arg va_list*
 * @return void
 *
 */
void ads_vprintf(const char * format,va_list *arg);

/** @brief formating print to buffer with va_list
 *
 * @param des char* the buffer start address
 * @param size int size of the buffer
 * @param char*format const formated ads_str
 * @param arg va_list* variables
 * @return void
 *
 */
void ads_vsprintf(char * des,int size,const char*format,va_list *arg);

/** @brief formating print to buffer
 *
 * @param des char* the buffer start address
 * @param size int size of the buffer
 * @param char*format const formated ads_str
 * @param ... variables
 * @return void
 *
 */
void ads_sprintf(char * des,int size,const char*format,...);

#endif
