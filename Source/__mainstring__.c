

#include "Ads\str.h"
#include "Ads\utility\timeit.h"


int main()
{
	ads_str s = ads_strFrmDouble(10.20,10,2);
	ads_str s1 = ads_new(ads_str,"Sourav Das %d",10);
	ads_str s5 = ads_new(ads_str,"Go Baby");
	char tmp[ads_strLen(s1)+1];
	ads_strToCstr(s1,tmp);
	ads_printf("cstring of ads_str s1 %str = %s\n\n",s1,tmp);

	int size = ads_strSizeOf(s1);
	ads_printf("sizeof ads_str s1 %str = %d bytes \n\n",s1,size);

	ads_strSetSlice(s1,7,9,"BAD");
	ads_printf("string_setslice of s1=%s at 7 to 9 with BAD  = %str\n\n",tmp,s1);

	int count = ads_cstrCount("ac ab ab av ab ac ba bd ab","ab ac");
	int index = ads_cstrFind("ac ab ab av ab ac ba bd ab","ab ac");
	ads_printf("'ab ac' present in csting=%s is %d time at index %d\n\n","ac ab ab av ab ac ba bd ab",count,index);

	ads_cstrReplace(s1,"BAD","DAS");
	ads_printf("Replacing 'BAD' with 'DAS' in %s = %str \n\n",tmp,s1);

	ads_strMul(s1,3);
	ads_printf("muliplying s1 %s with 3 = %str\n\n",tmp,s1);

	ads_foreach(ch,ads_strIterator(s5),char)
	{
		ads_output("'%c' ",ch);
	}ads_next(ch);

	ads_printf("{str}",s);

	ads_printf("time = %s\n","25/10/16 11:20:48");

	ads_vector l = ads_cstrSplit("creat key lol : {1,2,3,4,5,6}",":,");

	ads_printf("\nsplit the ads_str('25/10/16 11:20:48') into substring at point where it contain '/: ' characters \n\n");

	ads_printf("{[str]}\n",l);

	ads_foreach(i,ads_vectorIterator(l),ads_str)
	{
		int ff = ads_strToInt64(i,10) * 2 ;
		ads_strSet(i,"%d",ff);
	}ads_next(i);


	ads_printf("{[str]}\n",l);

	ads_str s2 = ads_cstrJoin(l,":");
	ads_output("\n\n Join the modified list into a complete strings = ");

	ads_printf("{str}",s2);

	ads_delete(l,s2,s,s1,s5);

	return 0;
}
