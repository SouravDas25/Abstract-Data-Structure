

/** @file timeit.h
 * @brief a performance clocking module.
 *
 */

#ifndef ADSTIMEIT_H
#define ADSTIMEIT_H

#include <stdint.h>
#include <time.h>

/**
 * @addtogroup time-it
 * @{
 * @brief containing function and variable where execution time can be computed.
 *
 */

typedef struct _ads_time ads_time;


struct _ads_time {
    int32_t min;
    int32_t sec;
    int32_t msec;
};

/** @cond ADS_INTERNAL */

void ads_setTimeCheckpoint();

int32_t ads_getTimeMin(clock_t ct);

int32_t ads_getTimeSec(clock_t ct);

int32_t ads_getTimeMilliSec(clock_t ct);

ads_time ads_getTimeInterval();

void ads_printTimeChekpoint(ads_time * t);

/** @endcond */


/** @brief computes the execution time of the function
 *
 * @param function a function whose execution time should be computed.
 * @return ads_time(minutes,second,millisecond)
 */
ads_time ads_clockFunction(void(*function)(void));

/** @} */

#endif
