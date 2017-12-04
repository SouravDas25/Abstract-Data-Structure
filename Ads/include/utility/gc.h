
/** @file gc.h
 * @brief a semi-automatic garbage collector for c.
 *
 */



#ifndef garbagecollector_H
#define garbagecollector_H

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

#include "Error.h"
#include "object.h"


/**
 * @addtogroup Garbage-Collector
 * @{
 * @brief automatically deallocate memory when the pointer to the memory is gone.
 *
 * @details this GC keeps track of the variable which contains the dynamically allocated memory
 * and frees the memory when the variable is destroyed or out of scope
 */
typedef unsigned int UINT;

/** @brief allocate dynamic memory in a safe way maintaining a stack of occupied element .
 *
 * @param n no of element to allocate
 * @param size size of each element
 * @return void* pointer to such memory block
 *
 */
void* ads_calloc(int no_of_blocks,int size_of_blocks);

/** @brief allocate dynamic ADS Object memory in a safe way. It Keeps track of the object.
 *
 * @param n no of element to allocate
 * @param size size of each element
 * @return void* pointer to such memory block
 *
 * @warning should not be called directly use specific object constructor instead.
 */
void* ads_rawCalloc(int n,int size);

/** @brief allocate dynamic memory in a safe way maintaining a stack of occupied element .
 *
 * @param size size of the element
 * @return void* pointer to such memory block
 *
 */
void* ads_malloc(UINT size);

/** @brief prints the occupied stack to the stdout .
 * @return void
 */
ads_rt ads_gcDump();

/** @brief prints the current amount of dynamic memory used till now.
 *
 * @return void
 *
 */
ads_rt ads_memCheck(FILE*fp);

ads_rt ads_fgcDump(FILE*fp);

/** @brief frees the memory occupied by the ads_calloc() and ads_malloc() functions.
 *
 * @param address address of the memory to deallocate
 * @return void
 *
 */
ads_rt ads_free(void* address);

/** @brief frees the memory occupied by the ads_raw_calloc() functions.
 *
 * @param address address of the memory to deallocate
 * @return void
 * @warning should not be called directly use ads_delete_many() or ads_delete_one() function instead.
 */
ads_rt ads_rawFree(void* a);

/** @brief sends a variable to the GC. if the GC sees that the variable is deleted then it deallocate memory with it.
 *
 * @param var variable containing the address of the dynamically allocated block
 * @param type the type of the variable.
 * @return void
 *
 */
#define ads_registerVar(var,type) ads_setGcVariable((void**)&var,ads_isAdsType(#type))

/** @cond ADS_INTERNAL */
ads_rt ads_setGcVariable(void ** add,short ads);
/** @endcond */

/** @brief deallocate all pending freed memory
 *
 * @return void
 *
 */
ads_rt ads_flushVariable();

/** @} */

#endif
