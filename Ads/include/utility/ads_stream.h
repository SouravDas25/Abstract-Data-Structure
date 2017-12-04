
#ifndef ADSSTREAM_C
#define ADSSTREAM_C

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * @addtogroup Input-Output
 * @{
 * @brief ads special io functions.
 *
 * @details standard output can be directed to any file buffer or stdout
 * or even to a char[] buffer.
 */
FILE* ads_getOutputStream();                        /**< returns the current output stream. */
char * ads_getOutBuffer();                          /**< returns the current output buffer if set else NULL. */
int ads_getBufferLen();                             /**< the output buffer maximum length. */
void ads_setOutputStream(FILE*ostream);             /**< redirect the output stream to the given file stream. */
void ads_setOutputBuffer(char * buffer,int size);   /**< redirect the output to the given ads_str buffer */
bool ads_getOutType();                              /**< returns true if output is to a buffer else false */
int ads_output(const char * format,...);            /**< output the formatted buffer to the out stream/buffer. */
void ads_putChar(int c);                            /**< put a single character to the out stream/buffer. */
/** @} */

#ifdef __cplusplus
}
#endif

#endif
