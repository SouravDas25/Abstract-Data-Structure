

#include "Ads\Heap.h"

int main()
{
	int n = 12;
	srand(time(NULL));
	ads_heap t = ads_heapNew(int,ads_cmpInt,ads_minHeap); /*create a heap*/

	int y = 10,i;
	ads_heapBuilt(t,int,5,6,12); /*insert some elements*/
	for(i =0;i<n;i++)
	{
		y = rand() % 500 ;
		ads_heapInsert(t,y,int);
	}
	ads_printHeap(t,ads_printInt); /*print heap*/
	ads_output("%d bytes,\n",ads_heapSizeOf(t)); /*print heap size*/
	ads_list l = ads_heapToList(t); /* heap to list (heapsort)*/
	ads_printf("{list(d)}",l);

	ads_delete(t,l);/*delete*/

	return 0;
}
