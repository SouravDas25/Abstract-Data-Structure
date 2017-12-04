
#include "Ads\deque.h"

#define info "------------------------- \
\nchoose -1 to exit \
\nchoose 0 to print info again\
\nchoose 1 to push elements into deque\
\nchoose 2 to pop elements from deque\
\nchoose 3 to display deque\
\nchoose 4 to print no of item in deque \
\nchoose 5 to push elements in front of the deque\
\nchoose 6 to pop elements from the front of deque\
\n-------------------------\n\n"

int main ()
{
	deque d = newdeque(int);
	int c,val,pos;
	bool loop = true;
	printf("\nWelcome this is a deque(Double Ended Queue) program use the following option to do appropriate task on Deque\n");
	printf(info);
	while(loop)
	{
		printf("\nEnter a choice : ");scanf("%d",&c);
		switch(c)
		{
			case -1:
				loop = false;
				break;
			case 0 :
				printf(info);
				break;
			case 1 :
				printf("\nEnter value to insert into deque : ");scanf("%d",&val);
				deque_push(d,val,int);
				break ;
			case 2 :
				pos = deque_pop(d,int);
				printf("\nElement Poped from deque is : (%d)",pos);
				break;
			case 3 :
				break;
			case 4 :
				printf("\nNo of items in stack are %d \n",deque_len(d));
				break;
			case 5 :
				printf("\nEnter value to insert in front of deque : ");scanf("%d",&val);
				deque_pushfront(d,val,int);
				break;
			case 6 :
				pos = deque_popfront(d,int);
				printf("\nElement Poped from front of deque is : (%d)",pos);
				break;
			default :
				printf("\nInvalid choice : \n");
				break;
		}
		printf("\n\n");printdeque(d,printint);printf("\n\n");
	}
	freedeque(d);
	return 0;
}
