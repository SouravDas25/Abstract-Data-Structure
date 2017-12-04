

#include "Ads\Dict.h"

int main()
{
	ads_dict d = ads_new(ads_dict,int,ads_hashInt,int,32); /*create a dict instance*/

	int i;
	for(i=0;i<6;i++)
	{
		ads_dictSetItem(d,i,i*10,int,int); /*insert some values into dict*/
	}

	ads_output("\nsizeofdict = %d bytes , load = %lf \n\n",ads_dictSizeOf(d),ads_dictLoad(d)); /*print dict size and load*/

	ads_printf("dict = {[d:d]} ",d);

	int len = ads_dictLen(d); /*get no of keys in dict*/

	ads_dictHasKey(d,10,int); /*return true if the dict has the key 10 else false*/

	ads_vector keys = ads_dictKeys(d); /*get vector of all keys*/

	ads_vector values = ads_dictValues(d); /*get vector of all values*/

	int val ;
	ads_dictGetItem(d,5,int,int,val);/*returns the value corresponding to the key*/

	ads_dictPopItem(d,4,int,int,val); /*returns the value and removes the key from the dict*/

	ads_dictDelItem(d,3,int); /*removes a key*/

	ads_delete(d,keys,values); /*delete the dict , key and value vector*/

	return 0;
}



