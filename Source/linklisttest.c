
#include "Ads\linklist.h"

void printintlinklist(void*a)
{
	printlinklist(cast(a,linklist),printint);
}

int main ()
{
	printf("\n");
	//lolasdda;
	linklist l = newlinklist(linklist);
	int i,h;
	for(i = 0 ;i<4;i++)
	{
		linklist lsub = newlinklist(int);
		for(h=0;h<4;h++)
		{
			int f = 3* h;
			linklist_append(lsub,f,int);
		}
		linklist_append(l,lsub,linklist);
		freelinklist(lsub);
	}
	printlinklist(l,printintlinklist);
	printf("\n\n");
	foreach(ll,linklist_iterator(l),linklist)
	{
		foreach(z,linklist_iterator(ll),int)
		{
			printf("%d,",z);
		}next(z)
	}next(ll)
	freelinklist(l);
	Garbagecheck();
	return 0;
}
