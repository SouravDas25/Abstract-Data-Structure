

#include <stdio.h>
#include "Ads\linklist.h"

#define info "------------------------- \
\nchoose -1 to exit \
\nchoose 0 to insert at end \
\nchoose 1 to delete a value from position\
\nchoose 2 to display linklist\
\nchoose 3 to insert at position\
\nchoose 4 to set a value at position\
\nchoose 5 to print no of item in linklist \
\nchoose 6 to find the index of the value \
\nchoose 7 to remove a value from linklist \
\nchoose 8 to reverse the linklist \
\nchoose 9 to remove all element from \
\nchoose 10 to print info again\
\nchoose 11 to add two random values to linklist\
\nchoose 12 print memory occupied by linklist\
\n-------------------------\n\n"

int main()
{
	int c,val,pos,index;bool loop = true;srand(time(NULL));
	linklist l = newlinklist(int);
	printf("\nWelcome this is a linklist program use the following option to do appropriate task on linklist\n");
	printf(info);
	while(loop)
	{
		printf("\nEnter a choice : ");scanf("%d",&c);
		switch(c)
		{
			case 0 :
				printf("Enter value to insert at last : ");scanf("%d",&val);
				linklist_append(l,val,int);
				break ;
			case 1 :
				printf("Enter index to delete from linklist : ");scanf("%d",&pos);
				linklist_delitem(l,pos);
				break;
			case 2 :
				break;
			case 3 :
				printf("Enter position to insert at : ");scanf("%d",&pos);
				printf("Enter value to insert at position %d : ",pos);scanf("%d",&val);
				linklist_insert(l,pos,val,int);
				break;
			case 4 :
				printf("Enter position to set item at : ");scanf("%d",&pos);
				printf("Enter value to set at position %d : ",pos);scanf("%d",&val);
				linklist_setitem(l,pos,val,int);
				break;
			case 5 :
				printf("\nNo of items in linklist are %d \n",linklist_len(l));
				break;
			case 6 :
				printf("\nEnter value to find in linklist : ");scanf("%d",&val);
				index = linklist_index(l,compareint,val,int);
				printf("\nValue %d found in position %d\n",val,index);
				break;
			case 7 :
				printf("\nEnter value to remove from linklist : ");scanf("%d",&val);
				linklist_remove(l,compareint,val,int);
				break;
			case 8 :
				linklist_reverse(l);
				break;
			case 9 :
				linklist_clear(l);
				break;
			case -1:
				loop = false;
				break;
			case 10 :
				printf(info);
				break;
			case 11 :
				builtlinklist(l,int,rand()%100,rand()%100);
				break;
			case 12 :
				printf("\nSize of linklist is %I64d bytes \n",linklist_sizeof(l));
				break;
			default :
				printf("\nInvalid choice : \n");
				break;
		}
		printf("\n");printlinklist(l,printint);printf("\n");
	}
	delete(l);
	//Garbagecheck();
	system("pause");
	return 0;
}


