#include<stdio.h>

#include "Ads\Stack.h"
#include "Ads\str.h"


int main()
{
	ads_stack  s = ads_stackNew(char*);

	ads_stackBuilt(s,char*,"apple","mapple");

	ads_printf("stk = {stk('s')}\n",s);

	ads_delete(s);

	ads_gcDump();

	return 0;
}
