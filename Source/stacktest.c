

#include "Ads\stack.h"

int main ()
{
	ads_stack l= ads_new(ads_stack,int);
	int i=10;
	ads_stackPush(l,i,int);
	for(i=0;i<10;i++)
	{
		int y= (int) (rand()%100) ;
		ads_stackPush(l,y,int);
	}
	
	ads_printf("stck = {stk(d)}\n",l);
	
	ads_stack s2 = (ads_stack)ads_copyStack(l);
	int len=ads_stackLen(l);
	int arr[len];
	i=0;
	while(ads_stackIsEmpty(l) == false)
	{
		ads_stackPop(l,int,arr[i++]);
	}
	ads_printf("stck = {stk(d)}\n",l);
	
	ads_printf("stck = {stk(d)}\n",s2);
	
	ads_foreach(k,ads_arrayIterator(arr,len),int)
	{
		printf("%d,",k);
	}ads_next(k);
	
	ads_delete(s2,l);
	return 0;
}

