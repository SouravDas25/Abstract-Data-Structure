

#include <stdio.h>
#include "Ads\stack.h"

void intprint(void*add)
{
	printf("%d",*(int*)add);
}


int main()
{
	srand(time(0));
	stack l = newstack(int);
	int i=10;int arr[10];int* g = arr;
	stack_push(l,i,int);
	for(i=0;i<10;i++)
	{
		int y= (rand()%100) ;
		arr[i] = y;
		stack_push(l,y,int);
	}
	printstack(l,intprint);
	LB;
	foreach(j , array_iterator(g,10),int)
	{
		printf("%d,",j);
	}next(j);
	printarray(arr,10,printint);
	printstack(l,intprint);
	freestack(l);
	ln;ln;
	system("pause");
	return 0;
}
