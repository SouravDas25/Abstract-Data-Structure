
#include "Ads\linklist.h"

void printintlinklist(void*a)
{
	printlinklist(cast(a,linklist),printint);
}

int main ()
{
	printf("\n");
	linklist l = newlinklist(int);
	builtlinklist(l,int,1,2,3,4,5,6,7,8,9);
	linklist l2 = newlinklist(linklist);
	builtlinklist(l2,linklist,l,l,l);
	int y = 25;
	linklist_insert(l,8,y,int);
	printlinklist(l,printint);
	LB;
	printlinklist(l2,printintlinklist);
	delete(l,l2);
	//Garbagecheck();
	return 0;
}
