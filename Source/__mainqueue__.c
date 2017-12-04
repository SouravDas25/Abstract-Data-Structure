#include<stdio.h>

#include "Ads\Queue.h"

int main()
{
	ads_queue q = ads_new(ads_queue,int);
	int i=0,l;
	ads_output("Enqueueing to the tail.\n");
	for(i=0;i<10;i++)
	{
		ads_queueInsert(q,i+10,int);
		ads_printf("{que(d)}",q);
		ads_output("\n");
	}
	ads_output("Dequeing from the head.\n");
	for(i=0;i<5;i++)
	{
		ads_queuePop(q,int,l);
		ads_printf("{que(d)}",q);
		ads_output("\n");
	}


	ads_delete(q);
	return 0;
}
