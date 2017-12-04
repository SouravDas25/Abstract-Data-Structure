


#include "Ads\stack.h"

#define info "------------------------- \
\nchoose -1 to exit \
\nchoose 0 to print info again\
\nchoose 1 to push elements into stack\
\nchoose 2 to pop elements from stack\
\nchoose 3 to display stack\
\nchoose 4 to print no of item in stack \
\nchoose 5 to reverse the stack \
\n-------------------------\n\n"

int main ()
{
	stack s = newstack(int);
	int c,val,pos;
	bool loop = true;
	printf("\nWelcome this is a stack program use the following option to do appropriate task on Stack\n");
	printf(info);
	while(loop)
	{
		printf("\nEnter a choice : ");scanf("%d",&c);
		switch(c)
		{
			case 1 :
				printf("Enter value to insert into stack : ");scanf("%d",&val);
				stack_push(s,val,int);
				break ;
			case 2 :
				pos = stack_pop(s,int);
				printf("Element Poped from stack is : (%d)",pos);
				break;
			case 3 :
				break;
			case 4 :
				printf("No of items in stack are %d \n",stack_len(s));
				break;
			case 5 :
				stack_reverse(s);
				break;
			case -1:
				loop = false;
				break;
			case 0 :
				printf(info);
				break;
			default :
				printf("\nInvalid choice : \n");
				break;
		}
		printf("\n");printstack(s,printint);printf("\n");
	}
	freestack(s);
	return 0;
}
