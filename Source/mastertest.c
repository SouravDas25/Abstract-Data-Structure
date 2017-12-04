
#include "Ads\std.h"


int vector_test()
{
	vector l = newvector(int,10); /*parse the string and create a vector of int */
	builtvector(l,int,1,2,4,7);
	ads_printf("builtvector with integers vector= {[d]}",l);
	int j ;
	vector_getitem(l,0,int,j);
	
	ads_printf("\n\nget element at index 0 and display it : {d} vector= {vec(d)}",j,l);

	vector_setitem(l,0,42,int);
	ads_printf("\n\nset element at index 0 with 42 vector = {[d]}",l);

	vector_insert(l,1,29,int);
	ads_printf("\n\ninsert {d} at index 1 vector= {[d]}",29,l);

	vector_remove(l,INBUILTCMP,42,int);
	ads_printf("\n\nremove all occurence of {d} from the vector using inbuiltcmp vector= {[d]}",42,l);
	
	vector_pop(l,int,j);
	ads_printf("\n\npop a element from the vector and return it : {d} vector= {[d]}",j,l);
	
	vector_index(l,INBUILTCMP,7,int,j);
	ads_printf("\n\nfind a element in the vector and return its index : {d} vector= {[d]}",j,l);

	vector_sort(l,INBUILTCMP);
	ads_printf("\n\nSort the vector using intcompare vector= {[d]}",l);

	vector_shrink(l);
	
	vector_clear(l);
	ads_printf("\n\nRemove all element from the vector = {[d]}",l);
	
	vector ll = newvector(vector,2); /*another instance but it is a vector of vectors*/
	ads_printf("\n\nDisplay the vector containing a vector of integers  vector= {[[d]]} ",ll);
	vector_append(ll,l,vector);
	vector_append(ll,l,vector);
	
	ads_printf("\n\nDisplay the vector containing a vector of integers vector= {[[d]]}  ",ll);
	
	delete(l,ll);
 	
	return 0;
}

int AvltreeTest()
{
	Avltree t = newAvltree(int,compareint,int); /*Create a Avltee instance*/
	
	Avltree_insert(t,1,30,int,int); /*Insert some key-Value pairs in avltree*/
	Avltree_insert(t,2,78,int,int);
	Avltree_insert(t,3,78,int,int);
	Avltree_insert(t,4,78,int,int);
	Avltree_insert(t,5,78,int,int);
	Avltree_insert(t,6,78,int,int);
	
	printAvltree(t,printint,printint); /*print the avltree*/
	
	Avltree_delete(t,4,int); /*delete a key from the avltree*/
	
	printAvltree(t,printint,printint); /*print the avltree*/

	vector l = Avltree_traversal(t,POSTORDER,AvltreeKEY); /*do a  post order traversal of the keys in the tree*/
	
	ads_printf("vec = {[d]} ",l,25);
	
	delete(l,t); /*delete the avl tree*/
	/*every data structure has to be deleted explicitly*/
	return 0;
}

int DequeTest()
{
	deque s = newdeque(int); /*create a new instance of the deque*/
	
	builtdeque(s,int,1,2,3,4,5,6,7,8,9,10); /*initialize the deque with multiple elements*/
	
	int i = 56;
	deque_push(s,i,int); /*push elements at back or end*/
	
	deque_pushfront(s,i,int); /*push elements at the front*/

	int h ;
	deque_front(s,int,h); /*get the front element without removing*/
	
	deque_end(s,int,h); /*get the rear element without removing*/
	
	ads_output("\nlast element = %d\n",h); /*ads_output === printf but ads_output can output to file without any system argument*/
	
	foreach(k,deque_iterator(s),int) /*deque iterator*/
	{
		ads_output(",%d",k); /*print each element*/
	}next(k); /*mandetory , without next foreach won't work*/
	
	LB; /*LineBreak === printf("\n") */
	
	ads_printf("dqeue = {deq(d)}",s);
	
	delete(s); /*delete the deque*/
	
	return 0;
}


int DictTest()
{
	dict d = newdict(int,hashint,int,32); /*create a dict instance*/
	
	int i;
	for(i=0;i<6;i++)
	{
		dict_setitem(d,i,i*10,int,int); /*insert some values into dict*/
	}
	
	ads_output("\nsizeofdict = %d bytes , load = %lf \n\n",dict_sizeof(d),dict_load(d)); /*print dict size and load*/
	
	ads_printf("dict = {[d:d]} ",d);
	
	int len = dict_len(d); /*get no of keys in dict*/
	
	dict_haskey(d,10,int); /*return true if the dict has the key 10 else false*/
	
	vector keys = dict_keys(d); /*get vector of all keys*/
	
	vector values = dict_values(d); /*get vector of all values*/
	
	int val;
	dict_getitem(d,5,int,int,val);/*returns the value corresponding to the key*/
	
	dict_popitem(d,4,int,int,val); /*returns the value and removes the key from the dict*/
	
	dict_delitem(d,3,int); /*removes a key*/
	
	delete(d,keys,values); /*delete the dict , key and value vector*/
	val = val;
	len = len;
	return 0;
}

int ExtrieTest()
{
	extrie t = newextrie(int); /*create a extrie*/
	extrie_insert(t,"boro",10,int); /*insert some strings*/
	extrie_insert(t,"bina",10,int);
	extrie_insert(t,"bino",10,int);
	extrie_insert(t,"bindas",10,int);
	extrie_insert(t,"before",10,int);
	extrie_insert(t,"beaware",10,int);
	extrie_insert(t,"belly",10,int);
	extrie_insert(t,"bee",10,int);
	extrie_insert(t,"beef",10,int);
	extrie_insert(t,"beech",10,int);
	extrie_insert(t,"beach",10,int);
	extrie_insert(t,"apple",10,int);
	
	extrie_delete(t,"bindas");/*delete some strings*/
	
	printextrie(t,printint); /*print the extrie*/
	
	list l = extrie_getallwith(t,"bi"); /*get all strings starting with "bi" */
	
	ads_printf("list = {list(str)} ",l);
	
	delete(l);
	LB;
	bool b = extrie_search(t,"belly"); /*returns true if string present*/
	printbool(&b);
	delete(t);
	
	return 0;
}

int HeapTest()
{
	int n = 12;
	srand(time(NULL));
	heap t = newheap(int,compareint,MINHEAP); /*create a heap*/
	
	int y = 10,i;
	builtheap(t,int,5,6,12); /*insert some elements*/
	for(i =0;i<n;i++)
	{
		y = rand() % 500 ;
		heap_insert(t,y,int);
	}
	printheap(t,printint); /*print heap*/
	ads_output("%d bytes,\n",heap_sizeof(t)); /*print heap size*/
	list l = heap2list(t); /* heap to list (heapsort)*/
	ads_printf("{list(d)}",l);
	
	delete(t,l);/*delete*/
	
	return 0;
}

int QueueTest()
{
	queue q = newqueue(int);
	int i=0;
	ads_output("Enqueueing to the tail.\n");
	for(i=0;i<10;i++)
	{
		EnQueue(q,i+10,int);
		ads_printf("{que(d)}",q);
		ads_output("\n");
	}
	ads_output("Dequeing from the head.\n");
	for(i=0;i<5;i++)
	{
		int k;
		DeQueue(q,int,k);
		k=k;
		ads_printf("{que(d)}",q);
		ads_output("\n");
	}
	freequeue(q);
	return 0;
}

int SetTest()
{
	FILE * fp = fopen("buffers/test.txt","w"); /*open a file*/
	/*set_output_stream(fp); redirect stdout to the file stream*/
	
	set s = newset(int,hashint,7); /*create a set*/
	
	builtset(s,int,1,2,3,4,5,6,7,8,9); /*initialize a set*/
	
	ads_printf("set = {<d>}",s);
	
	set_discarditem(s,7,int); /*delete 7 from  set*/
	
	int j ;
	set_pop(s,int,j); /*any element from set*/
	
	ads_output("\nafter poping %d and discarding 7 ",j);
	
	ads_printf("set = {<d>}",s);
	
	set s1 = newset(int,hashint,7); /*create a new set*/
	
	set_additem(s1,12,int); /*add some elements*/
	set_additem(s1,2,int);
	set_additem(s1,4,int);
	
	ads_output("\nnew set ");
	ads_printf("set = {<d>}",s);
	
	ads_output("\nafter set union ");

	set su = set_union(s,s1); /*union of the two set*/
	ads_printf("set = {<d>}",su); /*elements of both set only included once*/
	
	ads_output("\nafter set difference "); 

	set sd = set_difference(s,s1);/*difference of both the set : */
	ads_printf("set = {<d>}",sd); /* elements of 's1' not in 's'. */
	
	ads_output("\nafter set intersection "); 
	
	set is = set_intersection(s,s1);/*intersections of the two set*/
	ads_printf("set = {<d>}",is); /*elements common in both the sets*/
	
	
	delete(s,s1,is,sd,su);
		fclose(fp);
	return 0;
}

int StackTest()
{
	stack  s = newstack(string); /*Create a stack*/
	string s0 = newstring("kilo"); /*create a new strings*/
	string s1 = newstring("Sourav Das"); 

	builtstack(s,string,s1,s0,s0); /*initialize the stack with the strings*/
	
	ln;
	string  s2 = newstring("pop");
	
	stack_push(s,s2,string);/*push new strings to stack*/
	
	ads_printf("{stk(str)}",s); /*print the string*/
	ln;
	stack st = (stack)copystack(s); /*copy stack*/
	
	ads_printf("{stk(str)}",st); /*print the stack*/
	ln;
	foreach(j , stack_iterator(st),string) /*iterator over the stack elements */
	{
		ln;
		ads_printf("{str}",j); /*print each elements*/
	}next(j);
	ln;
	ln;
	string s3 ;
	stack_pop(s,string,s3); /*pop a elements*/

	ads_printf("{stk(str)}",s);/*print the stack*/
	
	delete(s,st,s0,s1,s2,s3);
	
	return 0;
}


int StringTest()
{
	set_time_checkpoint();
	
	string s = double_tostring(10.20,3,10);
	string s1 = newstring("Sourav Das %d",10);
	string s5 = newstring("Go Baby");
	char tmp[string_len(s1)+1];
	cstring(s1,tmp);
	ads_output("cstring of string s1(%s) = %s\n\n",__getcstring(s1),tmp);
	
	int size = string_sizeof(s1);
	ads_output("sizeof string s1(%s) = %d bytes \n\n",__getcstring(s1),size);
	
	string_setslice(s1,7,9,"BAD");
	ads_output("string_setslice of s1(%s) at 7 to 9 with BAD  = %s\n\n",tmp,__getcstring(s1));
	
	int count = cstring_count("ac ab ab av ab ac ba bd ab","ab ac");
	int index = cstring_find("ac ab ab av ab ac ba bd ab","ab ac");
	ads_output("'ab ac' present in csting(%s) is %d time at index %d\n\n","ac ab ab av ab ac ba bd ab",count,index);
	
	
	bool b = cstring_isdouble("45.3");
	ads_output("cstring_isdouble works 45.5 isdouble ? = %s\n\n",bool2cstring(b));
	
	b = cstring_isinteger("453");
	ads_output("cstring_isinteger works 453 isinteger ? = %s\n\n",bool2cstring(b));
	
	cstring_replace(s1,"BAD","DAS");
	ads_output("Replacing 'BAD' with 'DAS' in %s = %s \n\n",tmp,__getcstring(s1));
	
	string_muliplier(s1,3);
	ads_output("muliplying s1(%s) with 3 = %s\n\n",tmp,__getcstring(s1));
	
	foreach(ch,string_iterator(s5),char)
	{
		ads_output("'%c' ",ch);
	}next(ch);

	ads_printf("{str}",s);
	LB;LB;LB;
	
	ads_output("time = %s\n","25/10/16 11:20:48");
	
	vector l = cstring_split_cstring("25/10/16 11:20:48","/: ");
	
	ads_output("\nsplit the string('25/10/16 11:20:48') into substring at point where it contain '/: ' characters \n\n");
	
	ads_printf("{[str]}",l);LB;
	
	foreach(i,vector_iterator(l),string)
	{
		int ff = string_tolong(i,10) * 2 ;
		string_set(i,"%d",ff);
	}next(i);

	
	ads_printf("{[str]}",l);LB;
	
	string s2 = cstring_join(l,":");
	LB;
	ads_output("\n\n Join the modified list into a complete strings = ");
	
	ads_printf("{str}",s2);

	delete(l,s2,s,s1,s5);
	
	return 0;
}
int TreeTest()
{
	tree t = newtree(int); /*create a tree*/
	int i = 20;
	
	tree_insert(t,"",i,int); /* insert i at root an empty string implies root*/
	tree_insert(t,"",i,int); 
	tree_insert(t,"0",i,int); /* insert i at 0th child of the root*/
	tree_insert(t,"0",i,int);
	tree_insert(t,"0",i,int);
	tree_insert(t,"0",i,int);
	tree_insert(t,"0",i,int);
	tree_insert(t,"0.1",i,int); /* insert i at 1st child of 0th child of the root*/
	tree_insert(t,"0.1",i,int);
	tree_insert(t,"0.1",i,int);
	tree_insert(t,"0.1.0",i,int); /*the string taken by the insert function is a parent key*/
	tree_insert(t,"0.1.0",i,int); /*the tree is travered w.r.t the key*/
	tree_insert(t,"0.1.0",i,int); /*if we are reading the string from left to right*/
	tree_insert(t,"0.1.0",i,int); /*an number represent parent index and a period(.) represent a increase in depth*/
	tree_insert(t,"0.1.0.1",i,int); /*E.G. 0.1.3 */
	tree_insert(t,"0.1.0.1",i,int); /* 0. - 0th child of root then enter the child and make child the new parent*/
	tree_insert(t,"0.1.0.1.0",i,int); /*1. - 1th child of the new parent then enter the child and make child the new parent*/
	tree_insert(t,"0.1.0.1.0",i,int); /*3 - 3th child of the new parent , No period(.) means we have reached the parent*/
	i = 53;
	tree_setitem(t,"0",i,int);
	tree_delitem(t,"0.1.0");
	printtree(t,printint);
	int h;
	tree_getitem(t,"0",int,h);
	ads_output("lol = %d",h);
	delete(t);
	
	return 0;
}

int TrieTest()
{
	trie t = newtrie(int);
	trie_insert(t,"bina",52,int);
	trie_insert(t,"binod",85,int);
	trie_insert(t,"bipin",32,int);
	trie_insert(t,"binaly",20,int);
	
	printtrie(t,printint);
	
	int v ;
	trie_getitem(t,"binaly",int,v);
	ads_output("Trie_Getitem = %d \n",v);
	trie_delete(t,"binaly");

	printtrie(t,printint);
	delete(t);
	
	Garbagecheck();
	return 0;
}

int TupleTest()
{
	tuple t = newtuple(4); /*create and initialize a tuple elements cannot be added into a tuple*/
	
	tuple_setitem(t,0,32,int);
	tuple_setitem(t,1,3652,int);
	tuple_setitem(t,2,895,int);
	tuple_setitem(t,3,963,int);
	
	ads_printf("tuple = {(d,d,d,d)}",t);
	
	delete(t);
	return 0;
}

int main()
{
	printf("\n\nRUNNING VECTOR TEST.\n\n");
	vector_test();
	printf("\n\nCOMPLETED VECTOR TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING AVLTREE TEST.\n\n\n\n");
	AvltreeTest();
	printf("\n\nCOMPLETED AVLTREE TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING DEQUE TEST.\n\n\n\n");
	DequeTest();
	printf("\n\nCOMPLETED DEQUE TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING DICT TEST.\n\n\n\n");
	DictTest();
	printf("\n\nCOMPLETED DICT TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING EXTRIE TEST.\n\n\n\n");
	ExtrieTest();
	printf("\n\nCOMPLETED EXTRIE TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Heap TEST.\n\n\n\n");
	HeapTest();
	printf("\n\nCOMPLETED Heap TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Queue TEST.\n\n\n\n");
	QueueTest();
	printf("\n\nCOMPLETED Queue TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Set TEST.\n\n\n\n");
	SetTest();
	printf("\n\nCOMPLETED Set TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Stack TEST.\n\n\n\n");
	StackTest();
	printf("\n\nCOMPLETED Stack TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING String TEST.\n\n\n\n");
	StringTest();
	printf("\n\nCOMPLETED String TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Tree TEST.\n\n\n\n");
	TreeTest();
	printf("\n\nCOMPLETED Tree TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Trie TEST.\n\n\n\n");
	TrieTest();
	printf("\n\nCOMPLETED Trie TEST SUCCESSFULLY.\n\n");
	
	printf("\n\nRUNNING Tuple TEST.\n\n\n\n");
	TupleTest();
	printf("\n\nCOMPLETED Tuple TEST SUCCESSFULLY.\n\n");
	
	printf("\n\n ALL TEST SUCCESSFULLY Completed.\n\n");
	
	Garbagecheck();
	return 0;
}
