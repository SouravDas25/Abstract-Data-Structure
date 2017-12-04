


#ifndef ADSError_H
#define ADSError_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>


#define ads_BUFSIZ 512

typedef enum ads_rt {
    ads_SUCCESS,
    ads_VAL_RETURN,
    ads_ERROR,
    ads_FATAL_ERROR
} ads_rt;

/** @cond ADS_INTERNAL */

const char* ads_tempBuffer(const char * format,...);

ads_rt ads_registerError(const char * msg,...);
ads_rt ads_registerFatalError(const char * msg,...);
ads_rt ads_checkMemory(void * n);
ads_rt __ads_assert(bool exp,const char*_exp,const char* file ,int line,const char * func,const char* emsg);

#define ads_assert(_exp,msg) __ads_assert( (!!(_exp)) , #_exp ,__FILE__,__LINE__,__FUNCTION__,msg)
/** @endcond */


/** @brief returns the c-string representation of the last error error occurred.
 *
 * @param void
 * @return const char* the error.
 *
 */
const char * ads_lastErrorMsg(void);

/** @brief reset the last error to no-error.
 *
 * @return void
 *
 */
void ads_resetLastError();

#endif

