/** @cond ADS_INTERNAL */

#ifndef ADSTIMEIT_C
#define ADSTIMEIT_C

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "../../include/utility/timeit.h"

clock_t ADS_timeit_start = 0;

void ads_setTimeCheckpoint()
{
	ADS_timeit_start = clock();
}

int32_t ads_getTimeMin(clock_t ct)
{
    clock_t diff = ct - ADS_timeit_start;
	int64_t msec = diff * 1000 / CLOCKS_PER_SEC;
	return msec/60000;
}

int32_t ads_getTimeSec(clock_t ct)
{
	clock_t diff = ct - ADS_timeit_start;
	int64_t msec = diff * 1000 / CLOCKS_PER_SEC;
	return msec/1000;
}

int32_t ads_getTimeMilliSec(clock_t ct)
{
	clock_t diff = ct - ADS_timeit_start;
	int64_t msec = diff * 1000 / CLOCKS_PER_SEC;
	return msec;
}

ads_time ads_getTimeInterval()
{
    clock_t ct = clock();
    ads_time t = {ads_getTimeMin(ct),ads_getTimeSec(ct)%60000,ads_getTimeMilliSec(ct)%1000};
    return t ;
}

void ads_printTimeChekpoint(ads_time * t)
{
	printf("\ntime-diff = %d min %d sec %d millisec ",t->min,t->sec,t->msec);
}

ads_time ads_clockFunction(void(*function)(void))
{
    ads_setTimeCheckpoint();
    function();
    return ads_getTimeInterval();
}

#endif

/** @endcond */
