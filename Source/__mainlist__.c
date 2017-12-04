
#include "Ads\list.h"


int main()
{
	int s = 45;

	ads_list l = ads_new(ads_list,int); /*create a linked list*/

	ads_printf("\n list= {list(d)}",l);

	ads_listBuilt(l,int,1,2,3,4,5,6,7,8,9,10,11,12); /*initialize it with several values*/

	ads_foreach(k , ads_listIterator(l) ,int ) /*iterate over them , it takes O(1) time complexity*/
	{
		ads_output("%d,",k); /*print each value*/
	}ads_next(k);


	ads_printf("\n list= {list(d)}",l); /*print the list*/
	int b ;
	ads_listGetItem(l,0,int,b); /* get element at index 0*/

	int s2 ;
	ads_listPop(l,int,s2); /*pop element at front*/

	ads_listAppend(l,s2,int); /*insert element at end*/

	ads_listSetItem(l,2,s,int); /*set element at index 2 */

    ads_listIndex(l,ads_cmpInt,b,int,s2);
	ads_output("\nindex = %d,%d\n",b,s2); /*get and print the index of b*/

	ads_listDelItem(l,0); /*delete value at 0*/
	ads_listDelItem(l,3); /*delete value at 3*/

	ads_printf("\n list= {list(d)}\n",l); /*print list*/


	ads_list l1 = ads_listNew(void*); /*copy the list*/
	
	void* v = NULL;
	
	ads_listAppend(l1,v,void*);
	ads_listAppend(l1,v,void*);
	ads_listAppend(l1,v,void*);
	
	ads_listGetItem(l1,0,void*,v);

	ads_printf("\n list= {list(d)}",l1); /*print list*/
	ads_delete(l,l1); /*delete list*/
	ads_gcDump();
	return 0;
}
