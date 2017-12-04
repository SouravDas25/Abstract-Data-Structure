
#include "../Ads/include/vector.h"


int main3()
{
    //char stream[] = "[ 2 , 6 , 4 , 8 , 3 , 6 , 8 , 9 , 6 , 3 ]";
    ads_vector l = ads_vectorNew(int,10);
    ads_vectorBuilt(l,int,10,9,8,7,6,5,4,3,2,1);

    ads_printf("parse string with integers to vector= {[d]}",l);
    int j ;
    ads_vectorGetItem_NoCopy(l,0,int,j);

    ads_printf("\n\nget element at index 0 and display it : {d} vector= {vec(d)}",j,l);

    ads_vectorSetItem(l,0,42,int);
    ads_printf("\n\nset element at index 0 with 42 vector = {[d]}",l);

    ads_vectorInsert(l,1,29,int);
    ads_printf("\n\ninsert {d} at index 1 vector= {[d]}",29,l);

    ads_vectorRemove(l,ads_cmpMem,42,int);
    ads_printf("\n\nremove all occurence of {d} from the vector using inbuilt cmp vector= {[d]}",42,l);

    ads_vectorPop(l,int,j);
    ads_printf("\n\npop a element from the vector and return it : {d} vector= {[d]}",j,l);

    ads_vectorSort(l,ads_cmpMem);
    ads_printf("\n\nSort the vector using MEMcompare vector= {[d]}",l);

    ads_vectorShrink(l);

    ads_vectorBinarySearch(l,ads_cmpInt,5,int,j);
    ads_printf("\n\nPerforming Bs on the vector using intcompare vector= {[d]} index = %d ",l,j);


    //ads_vectorClear(l);
    ads_printf("\n\nRemove all element from the vector = {[d]}",l);

    ads_vector los = ads_vectorNew(char*,2);
    ads_vectorSetTD(los,ads_cstrTD);

    ads_vectorAppend(los,"Sourav Das",char*);
    ads_vectorAppend(los,"GOURAV DAS",char*);
    ads_vectorAppend(los,"RITA DAS",char*);
    char * cs;
    ads_vectorGetItem_Copy(los,0,char*,cs);
    ads_printf("\n\nVector of cstring = {['s']} %s",los,cs);

    ads_vector lol =  ads_vectorNew(ads_vector,2);
    ads_vectorSetTD(lol,ads_vectorTD);

    ads_vectorAppend(lol,los,ads_vector);
    ads_vectorAppend(lol,los,ads_vector);
    ads_printf("\n\nVector of vector = %[['s']] ",lol);

    ads_delete(los,l,lol,cs);

    ads_gcDump();

    return 0;
}

#include "../Ads/include/Avl.h"

int main2()
{
    ads_avl t = ads_new(ads_avl,int,ads_cmpInt,int); /*Create a Avltee instance*/

    ads_avlInsert(t,1,int,30,int); /*Insert some key-Value pairs in avltree*/
    ads_avlInsert(t,2,int,78,int);
    ads_avlInsert(t,3,int,78,int);
    ads_avlInsert(t,4,int,78,int);
    ads_avlInsert(t,5,int,78,int);
    ads_avlInsert(t,6,int,78,int);

    //ads_printAvl(t,ads_printInt,ads_printInt);

    ads_printf("avl = {avl(d:d)} \n",t); /*print the avltree*/

    ads_avlRemove(t,4,int); /*delete a key from the avltree*/
    int ret;
    ads_avlPop(t,1,int,int,ret);
    ads_avlGet_Copy(t,2,int,int,ret);

    ads_printf("avl = {avl(d:d)} %d\n",t,ret); /*print the avltree*/

    ads_vector l = ads_avlTraversal(t,ads_postOrder,ads_avlKey); /*do a  post order traversal of the keys in the tree*/

    ads_printf("vec = {[d]} %d",l,25);

    ads_avl tos = ads_avlNew(int,ads_cmpInt,char*);
    ads_avlSetTD(tos,ads_avlData,ads_cstrTD);

    ads_avlInsert(tos,6,int,"SOURAV DAS",char*);
    ads_avlInsert(tos,2,int,"SOURAV DAS",char*);
    ads_avlInsert(tos,9,int,"SOURAV DAS",char*);

    ads_printf("\navl = {avl(d:'s')} \n",tos);

    //ads_gcDump();


    ads_delete(l,t,tos); /*delete the avl tree*/
    /*every data structure has to be deleted explicitly*/

    ads_gcDump();

    return 0;
}



#include "../Ads/include/list.h"


int main1()
{
    int s = 45;

    ads_list l = ads_new(ads_list,int); /*create a linked list*/

    ads_printf("\n list= {list(d)}\n",l);

    ads_listBuilt(l,int,1,2,3,4,5,6,7,8,9,10,11,12); /*initialize it with several values*/

    ads_foreach(k, ads_listIterator(l) , int )   /*iterate over them , it takes O(1) time complexity*/
    {
        ads_output("%d,",k); /*print each value*/
    }
    ads_next(k);


    ads_printf("\n list= {list(d)}",l); /*print the list*/
    int b ;
    ads_listGetItem_NoCopy(l,0,int,b); /* get element at index 0*/

    int s2 ;
    ads_listPop(l,int,s2); /*pop element at front*/

    ads_listAppend(l,s2,int); /*insert element at end*/

    ads_listSetItem(l,2,s,int); /*set element at index 2 */

    ads_listIndex(l,ads_cmpInt,b,int,s2);
    ads_output("\nindex = %d,%d\n",b,s2); /*get and print the index of b*/

    ads_listDelItem(l,0); /*delete value at 0*/
    ads_listDelItem(l,3); /*delete value at 3*/

    ads_printf("\n list= {list(d)}\n",l); /*print list*/


    ads_list l1 = ads_listNew(char*); /*copy the list*/

    ads_listSetTD(l1,ads_cstrTD);

    ads_listBuilt(l1,const char*,"Sourav Das","Gourav Das","it is a list of strings");

    ads_printf("\n los = {list('s')}",l1); /*print list*/
    ads_delete(l,l1); /*delete list*/
    ads_gcDump();
    return 0;
}

#include "../Ads/include/Deque.h"

int main4()
{
    ads_deque s = ads_new(ads_deque,int); /*create a new instance of the deque*/

    ads_dequeBuilt(s,int,1,2,3,4,5,6,7,8,9,10); /*initialize the deque with multiple elements*/

    int i = 56;
    ads_dequePush(s,i,int); /*push elements at back or end*/

    ads_dequePushFront(s,i,int); /*push elements at the front*/

    int h ;
    ads_dequeFront_Copy(s,int,h); /*get the front element without removing*/

    ads_dequeEnd_Copy(s,int,h); /*get the rear element without removing*/

    ads_output("\nlast element = %d\n",h); /*ads_output === printf but ads_output can output to file without any system argument*/

    ads_foreach(k,ads_dequeIterator(s),int) /*deque iterator*/
    {
        ads_output(",%d",k); /*print each element*/
    }
    ads_next(k); /*mandetory , without next foreach won't work*/

    ads_printf("\ndqeue = {deq(d)}",s);

    ads_delete(s); /*delete the deque*/

    ads_gcDump();

    return 0;
}


#include "../Ads/include/Dict.h"

int main5()
{
    ads_dict d = ads_new(ads_dict,int,ads_hashInt,int,32); /*create a dict instance*/

    int i;
    for(i=0; i<6; i++)
    {
        ads_dictSetItem(d,i,int,i*10,int); /*insert some values into dict*/
    }

    ads_output("\nsizeofdict = %d bytes , load = %lf \n\n",ads_dictSizeOf(d),ads_dictLoad(d)); /*print dict size and load*/

    ads_printf("dict = {[d:d]} ",d);

    ads_dictLen(d); /*get no of keys in dict*/

    ads_dictHasKey(d,10,int); /*return true if the dict has the key 10 else false*/

    int val ;
    ads_dictGetItem_Copy(d,5,int,int,val);/*returns the value corresponding to the key*/
    val = val;

    ads_dictPopItem(d,4,int,int,val); /*returns the value and removes the key from the dict*/

    ads_dictDelItem(d,3,int); /*removes a key*/

    ads_dict d2 = ads_dictNew(char*,ads_cstrHash_ByRef,int,10);

    ads_dictSetKeyTD(d2,ads_cstrTD);

    ads_dictSetItem(d2,"Sourav Das",char*,1,int);
    ads_dictSetItem(d2,"Gourav Das",char*,2,int);
    ads_dictSetItem(d2,"Rita Das",char*,3,int);
    ads_dictSetItem(d2,"Subhas Das",char*,4,int);
    ads_dictSetItem(d2,"Arati Das",char*,5,int);

    ads_vector keys = ads_dictKeys(d2); /*get vector of all keys*/
    ads_vectorSort(keys,ads_cmpCStr_ByRef);
    ads_vector values = ads_dictValues(d2); /*get vector of all values*/

    ads_printf("\ndict = {['s':d]} \nkeys = {['s']} \nvalues = {[d]}",d2,keys,values);

    ads_delete(d,keys,values,d2); /*delete the dict , key and value vector*/

    ads_gcDump();

    return 0;
}


#include "../Ads/include/Heap.h"

int main6() //6
{

    srand(time(NULL));
    ads_heap t = ads_heapNew(char*,ads_cmpCStr_ByRef,ads_minHeap); /*create a heap*/
    ads_heapSetTD(t,ads_cstrTD);
    ads_heapBuilt(t,char*,"Apple", "Ball", "Cat","Dog", "Egg", "Fish","Aeroplane" );  /*insert some elements*/

    ads_printHeap(t,ads_printCStr_ByRef); /*print heap*/
    ads_output("%d bytes,\n",ads_heapSizeOf(t)); /*print heap size*/
    ads_list l = ads_heapToList(t); /* heap to list (heapsort)*/
    ads_printf("{list('s')}",l);

    ads_delete(t,l);/*delete*/
    ads_gcDump();

    return 0;
}

#include "../Ads/include/Queue.h"

int main7()//7
{
    ads_queue q = ads_new(ads_queue,int);
    int i=0,l;
    ads_output("Enqueueing to the tail.\n");
    for(i=0; i<10; i++)
    {
        ads_queueInsert(q,i+10,int);
        ads_printf("{que(d)}",q);
        ads_output("\n");
    }
    ads_output("Dequeing from the head.\n");
    for(i=0; i<5; i++)
    {
        ads_queuePop(q,int,l);
        ads_printf("{que(d)} %d",q,l);
        ads_output("\n");
    }


    ads_delete(q);
    ads_gcDump();
    return 0;
}

#include "../Ads/include/Stack.h"

int main8()//8
{
    ads_stack s = ads_stackNew(char*);

    ads_stackSetTD(s,ads_cstrTD);

    ads_stackBuilt(s,char*,"apple","mapple");

    ads_printf("stk = {stk('s')}\n",s);

    ads_delete(s);

    ads_gcDump();

    return 0;
}



#include "../Ads/include/Set.h"

int main9()//9
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

    ads_gcDump();

    return 0;
}

#include "../Ads/include/str.h"
#include "../Ads/include/utility/timeit.h"


int main10()//10
{
    ads_str s = ads_strFrmDouble(10.20,10,3);
    ads_str s1 = ads_new(ads_str,"Sourav Das %d",10);
    ads_str s5 = ads_new(ads_str,"Go Baby");
    char tmp[ads_strLen(s1)+1];
    ads_strToCstr(s1,tmp);
    ads_printf("cstring of ads_str s1 %str = %s\n\n",s1,tmp);

    ads_strCat(s1," is a Good Boy.");

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
        ads_putChar(ch);
    }
    ads_next(ch);

    ads_printf("{str}",s);

    ads_printf("time = %s\n","25/10/16 11:20:48");

    ads_vector l = ads_cstrSplit("25/10/16 11:20:48","/: ");

    ads_printf("\nsplit the ads_str('25/10/16 11:20:48') into substring at point where it contain '/: ' characters \n\n");

    ads_printf("{[str]}\n",l);

    ads_foreach(i,ads_vectorIterator(l),ads_str)
    {
        int ff = ads_strToInt64(i,10) * 2 ;
        ads_strSet(i,"%d",ff);
    }
    ads_next(i);


    ads_printf("{[str]}\n",l);

    ads_str s2 = ads_cstrJoin(l,":");
    ads_output("\n\n Join the modified list into a complete strings = ");

    ads_printf("{str}",s2);

    ads_delete(l,s2,s,s1,s5);

    ads_gcDump();


    return 0;
}


#include "../Ads/include/trie.h"

int main12() // 12
{
    ads_trie t = ads_trieNew(int);
    ads_trieInsert(t,"bina",52,int);
    ads_trieInsert(t,"binod",85,int);
    ads_trieInsert(t,"bipin",32,int);
    ads_trieInsert(t,"binaly",20,int);

    ads_printTrie(t,ads_printInt);

    int v ;
    ads_trieGetItem_NoCopy(t,"binaly",int,v);
    ads_output("Trie_Getitem = %d \n",v);
    ads_trieDelItem(t,"binaly");

    ads_printTrie(t,ads_printInt);
    ads_delete(t);

    ads_gcDump();
    return 0;
}


#include "../Ads/include/Extrie.h"

int main13()//13
{
    ads_extrie t = ads_extrieNew(int); /*create a extrie*/
    ads_extrieInsert(t,"boro",10,int); /*insert some strings*/
    ads_extrieInsert(t,"bina",10,int);
    ads_extrieInsert(t,"bino",10,int);
    ads_extrieInsert(t,"bindas",10,int);
    ads_extrieInsert(t,"before",10,int);
    ads_extrieInsert(t,"beaware",10,int);
    ads_extrieInsert(t,"belly",10,int);
    ads_extrieInsert(t,"bee",10,int);
    ads_extrieInsert(t,"beef",10,int);
    ads_extrieInsert(t,"beech",10,int);
    ads_extrieInsert(t,"beach",10,int);
    ads_extrieInsert(t,"apple",10,int);

    ads_extrieRemove(t,"bindas");/*delete some strings*/

    ads_printExtrie(t,ads_printInt); /*print the extrie*/

    ads_list l = ads_extrieGetAllWith(t,"bi"); /*get all strings starting with "bi" */

    ads_printf("\nlist = {list(str)} \n",l);

    ads_delete(l);

    ads_extrieSearch(t,"belly"); /*returns true if string present*/
    //printbool(&b);
    ads_delete(t);

    ads_gcDump();

    return 0;
}

#include "../Ads/include/Tuple.h"


int main14()//14
{
    ads_tuple t = ads_tupleNew(3); /*create and initialize a tuple elements cannot be added into a tuple*/

    ads_tupleAppend(t,56,int);
    ads_tupleAppend(t,985.356,double);
    ads_tupleAppendTD(t,ads_cstrTD,"Sourav Das",const char*);
    //ads_tupleSetItem(t,0,32,int);
    //ads_tupleSetItem(t,1,32,int);

    char* k;
    ads_tupleGetItem_Copy(t,2,char*,k);


    ads_printf("tuple = {(d,lf,'s')} %s ",t,k);

    ads_delete(t,k);
    ads_gcDump();
    return 0;
}

#define breakline printf("\n\n\t\tENDING TEST.\n\n");//getchar(multiple definition of );

int main()
{
    int i;
    for(i=0;i<1;i++)
    {


    main1();
    breakline;
    main2();
    breakline;
    main3();
    breakline;
    main4();
    breakline;
    main5();
    breakline;
    main6();
    breakline;
    main7();
    breakline;
    main8();
    breakline;
    main9();
    breakline;
    main10();
    breakline;
    main12();
    breakline;
    main13();
    breakline;
    main14();
    breakline;
    }
    return 0;
}

