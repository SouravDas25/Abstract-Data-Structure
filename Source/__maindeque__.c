
#include "Ads\Deque.h"

int main()
{
	ads_deque s = ads_new(ads_deque,int); /*create a new instance of the deque*/

	ads_dequeBuilt(s,int,1,2,3,4,5,6,7,8,9,10); /*initialize the deque with multiple elements*/

	int i = 56;
	ads_dequePush(s,i,int); /*push elements at back or end*/

	ads_dequePushFront(s,i,int); /*push elements at the front*/

	int h ;
	ads_dequeFront(s,int,h); /*get the front element without removing*/

	ads_dequeEnd(s,int,h); /*get the rear element without removing*/

	ads_output("\nlast element = %d\n",h); /*ads_output === printf but ads_output can output to file without any system argument*/

	ads_foreach(k,ads_dequeIterator(s),int) /*deque iterator*/
	{
		ads_output(",%d",k); /*print each element*/
	}ads_next(k); /*mandetory , without next foreach won't work*/

	ads_printf("\ndqeue = {deq(d)}",s);

	ads_delete(s); /*delete the deque*/

	return 0;
}
