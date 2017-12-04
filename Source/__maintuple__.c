#include <stdio.h>
#include <stdlib.h>

#include "../Ads/include/tuple.h"

int main()
{
    ads_tuple t = ads_tupleNew(7); /*create and initialize a tuple elements cannot be added into a tuple*/

    ads_tupleAppend(t,0,int);
//    ads_tupleAppend(t,985.356,double);
//    ads_tupleAppendTD(t,ads_cstrTD,"Sourav Das",const char *);
//    ads_tupleAppendCStr(t,"GOURAV Das");
//    ads_tupleAppendCStr(t,"NPTEL CERTIFICATE");
//    ads_tupleAppend(t,56,int);
    //ads_tupleSetItem(t,0,32,int);
    //ads_tupleSetItemCStr(t,0,"LOLLYPOP");
    //ads_tupleSetItem(t,1,32,int);

    ads_tuple tt = ads_tupleCopy(t);
    ads_tupleAppend(tt,"Sourav",char*);
    ads_tupleSetItem(t,0,"CSE",char*);
    ads_tuple t2 = ads_tupleConcat(t,tt);


//    char* k;
//    ads_tupleGetItem_NoCopy(t,2,char*,k);

    ads_printf("tuple = {('s',d,'s')} ",t2);

    ads_delete(t,tt,t2);
    ads_gcDump();
    return 0;
}
