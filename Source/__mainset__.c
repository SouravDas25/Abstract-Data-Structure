

#include "Ads\Set.h"

int main()
{
	ads_set s = ads_new(ads_set,int,ads_hashInt,7); /*create a set*/

	ads_setBuilt(s,int,1,2,3,4,5,6,7,8,9); /*initialize a set*/

	ads_printf("set = {<d>}",s);

	ads_setDiscardItem(s,7,int); /*delete 7 from  set*/

	int j;
	ads_setPop(s,int,j); /*any element from set*/

	ads_output("\nafter poping %d and discarding 7 ",j);

	ads_printf("set = {<d>}",s);

	ads_set s1 = ads_new(ads_set,int,ads_hashInt,7); /*create a new set*/

	ads_setAddItem(s1,12,int); /*add some elements*/
	ads_setAddItem(s1,2,int);
	ads_setAddItem(s1,4,int);

	ads_output("\nnew set ");
	ads_printf("set = {<d>}",s);

	ads_output("\nafter set union ");

	ads_set su = ads_setUnion(s,s1); /*union of the two set*/
	ads_printf("set = {<d>}",su); /*elements of both set only included once*/

	ads_output("\nafter set difference ");

	ads_set sd = ads_setDifference(s,s1);/*difference of both the set : */
	ads_printf("set = {<d>}",sd); /* elements of 's1' not in 's'. */

	ads_output("\nafter set intersection ");

	ads_set is = ads_setIntersection(s,s1);/*intersections of the two set*/
	ads_printf("set = {<d>}",is); /*elements common in both the sets*/


	ads_delete(s,s1,is,sd,su);
	return 0;
}
