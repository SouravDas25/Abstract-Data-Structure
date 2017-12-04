

#include "Ads\vector.h"

int main()
{
	vector v = newvector(int,10);
	builtvector(v,int,1,2,3,4,5,6);
	ads_printf("vector = {[d]}.\n",v);
	
	foreach(item,vector_iterator(v),int) //foreach(loop_variable,iterator,loop_variable type)
	{
		ads_printf("item = {d}.\n",item); //loop variable will be declared internally

	}next(item); //This is mandetory or else nested foreach loop will not work
	
	delete(v);
	
	return 0;
}


