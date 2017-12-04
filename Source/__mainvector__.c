
#include "../Ads/include/vector.h"

void * testCopy(const void * v)
{
    int a = *(int*)v;
    int * p = (int*)ads_calloc(1,sizeof(int));
    *p = a;
    return p;
}

ads_td testtd = {testCopy,ads_free,sizeof(int)};

int main()
{

	ads_vector l = ads_vectorNew(int,10); /*parse the string and create a vector of int */

	//ads_vectorSetTD(l,&testtd);

	ads_vectorBuilt(l,int,2,6,4,8,3,6,8,9,6,3);

	ads_printf("parse string with integers to vector= {[d]}",l);
	int j ;
	ads_vectorGetItem_NoCopy(l,0,int,j);

	ads_printf("\n\nget element at index 0 and display it : {d} vector= {vec(d)}",j,l);

	ads_vectorSetItem(l,0,42,int);
	ads_printf("\n\nset element at index 0 with 42 vector = {[d]}",l);

	ads_vectorInsert(l,1,29,int);
	ads_printf("\n\ninsert {d} at index 1 vector= {[d]}",29,l);

	ads_vectorRemove(l,ads_cmpMem,42,int);
	ads_printf("\n\nremove all occurence of {d} from the vector using inbuiltcmp vector= {[d]}",42,l);

	ads_vectorPop(l,int,j);
	ads_printf("\n\npop a element from the vector and return it : {d} vector= {[d]}",j,l);

	ads_vectorSort(l,ads_cmpMem);
	ads_printf("\n\nSort the vector using intcompare vector= {[d]}",l);

	ads_vectorShrink(l);

	//ads_vectorClear(l);
	//ads_printf("\n\nRemove all element from the vector = {[d]}",l);

	ads_vector lol = ads_new(ads_vector,ads_vector,2);

	ads_vectorSetTD(lol,ads_vectorTD);

	ads_vectorAppend(lol,l,ads_vector);
	ads_vectorAppend(lol,l,ads_vector);

	ads_printf("\n\nDisplay the vector containing vector : {[[d]]}  ",lol);

    ads_delete(l,lol);




	ads_vector los = ads_new(ads_vector,char*,2); /*another instance but it is a vector of vectors*/
	//ads_printf("\n\nDisplay the vector containing char*  vector= {[s]} ",los);

	//ads_vectorSetTD(los,ads_cstrTD);

	ads_vectorAppend(los,"Sourav Das",char * );
	ads_vectorAppend(los,"Gourav Das",char * );

	ads_printf("\n\n\n\nDisplay the vector containing strs : {['s']}  ",los);

	ads_delete(los);

	ads_gcDump();

	return 0;
}
