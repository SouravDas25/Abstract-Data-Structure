


#include "Ads\list.h"

#define info "------------------------- \
\nchoose -1 to exit \
\nchoose 0 to insert at end \
\nchoose 1 to delete a value from position\
\nchoose 2 to display list\
\nchoose 3 to insert at position(that position must be occupied)\
\nchoose 4 to set a value at position\
\nchoose 5 to print no of item in list \
\nchoose 6 to find the index of the value \
\nchoose 7 to remove a value from list \
\nchoose 8 to reverse the list \
\nchoose 9 to remove all element from list\
\nchoose 10 to sort the list \
\nchoose 11 to print info again\
\n-------------------------\n\n"

int main()
{
	int c,val,pos,index;
	bool loop = true;
	list l = newlist(int,10);
	printf("\nWelcome this is a list or vector program use the following option to do appropriate task on Vector\n");
	printf(info);
	while(loop)
	{
		printf("\nEnter a choice : ");scanf("%d",&c);
		switch(c)
		{
			case 0 :
				printf("Enter value to insert at last : ");scanf("%d",&val);
				list_append(l,val,int);
				break ;
			case 1 :
				printf("Enter index to delete from list : ");scanf("%d",&pos);
				list_delitem(l,pos);
				break;
			case 2 :
				break;
			case 3 :
				printf("Enter position to insert at : ");scanf("%d",&pos);
				printf("Enter value to insert at position %d : ",pos);scanf("%d",&val);
				list_insert(l,pos,val,int);
				break;
			case 4 :
				printf("Enter position to set item at : ");scanf("%d",&pos);
				printf("Enter value to set at position %d : ",pos);scanf("%d",&val);
				list_setitem(l,pos,val,int);
				break;
			case 5 :
				printf("\nNo of items in list are %d \n",list_len(l));
				break;
			case 6 :
				printf("\nEnter value to find in list : ");scanf("%d",&val);
				index = list_index(l,compareint,val,int);
				printf("\nValue %d found in position %d\n",val,index);
				break;
			case 7 :
				printf("\nEnter value to remove from list : ");scanf("%d",&val);
				list_remove(l,compareint,val,int);
				break;
			case 8 :
				list_reverse(l);
				break;
			case 9 :
				list_clear(l);
				break;
			case 10:
				list_sort(l,compareint);
				break;
			case -1:
				loop = false;
				break;
			case 11 :
				printf(info);
				break;
			default :
				printf("\nInvalid choice : \n");
				break;
		}
		printf("\n");printlist(l,printint);printf("\n");
	}
	delete(l);
	return 0;
}
