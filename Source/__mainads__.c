#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>

#include "Ads\vector.h"

int main()
{
	ads_vector v = ads_new(ads_vector,int,10);

 	ads_printf(" list = {[d]} \n",v);

	ads_delete(v);
	ads_memoryList();
	return 0;
}
