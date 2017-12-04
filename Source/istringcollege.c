

#include "Ads\str.h"



int main()
{
	char * b = ADS_AllocateArray(char,23);
	string s = newstring("%s","sourav");
	vector v = newvector(int,10);
	builtvector(v,int,1,2,3,4,5,6,9);
	ads_printf("s = %ld %s %str %[d] ",cstring_tolong("ff",16),"Sourav Das",s,v);
	ads_delete(s,b,v);
	return 0;
}
