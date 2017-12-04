


#include "Ads\Tree.h"

int main()
{
	ads_tree t = ads_treeNew(int); /*create a ads_tree*/
	int i = 20;

	ads_treeInsert(t,"",i,int); /* insert i at root an empty string implies root*/
	ads_treeInsert(t,"",i,int);
	ads_treeInsert(t,"0",i,int); /* insert i at 0th child of the root*/
	ads_treeInsert(t,"0",i,int);
	ads_treeInsert(t,"0",i,int);
	ads_treeInsert(t,"0",i,int);
	ads_treeInsert(t,"0",i,int);
	ads_treeInsert(t,"0.1",i,int); /* insert i at 1st child of 0th child of the root*/
	ads_treeInsert(t,"0.1",i,int);
	ads_treeInsert(t,"0.1",i,int);
	ads_treeInsert(t,"0.1.0",i,int); /*the string taken by the insert function is a parent key*/
	ads_treeInsert(t,"0.1.0",i,int); /*the ads_tree is travered w.r.t the key*/
	ads_treeInsert(t,"0.1.0",i,int); /*if we are reading the string from left to right*/
	ads_treeInsert(t,"0.1.0",i,int); /*an number represent parent index and a period(.) represent a increase in depth*/
	ads_treeInsert(t,"0.1.0.1",i,int); /*E.G. 0.1.3 */
	ads_treeInsert(t,"0.1.0.1",i,int); /* 0. - 0th child of root then enter the child and make child the new parent*/
	ads_treeInsert(t,"0.1.0.1.0",i,int); /*1. - 1th child of the new parent then enter the child and make child the new parent*/
	ads_treeInsert(t,"0.1.0.1.0",i,int); /*3 - 3th child of the new parent , No period(.) means we have reached the parent*/
	i = 53;
	ads_treeSetItem(t,"0",i,int);
	//ads_treeDelItem(t,"0.1.0");
	ads_printTree(t,ads_printInt);
	int h ;
	ads_treeGetItem(t,"0",int,h);
	ads_output("lol = %d",h);
	ads_delete(t);

	return 0;
}
