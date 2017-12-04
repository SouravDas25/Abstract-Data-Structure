
#include "Ads\tree.h"

#define info "------------------------- \
\nchoose -1 to exit \
\nchoose 0 to print info again\
\nchoose 1 to insert value into tree\
\nchoose 2 to set value of a perticular treeleaf \
\nchoose 3 to delete a perticular parent and all its decendant \
\nchoose 4 to print tree \
\nchoose 5 to to insert two random element into the tree \
\n-------------------------\n\n"


int main()
{
	tree et = newtree(int);
	int c,val;srand(time(NULL));
	bool loop = true;char str[50];
	printf("\nWelcome this is a unordered tree program use the following option to do appropriate task on Tree\n");
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
				printf("\nEnter value : ");scanf("%d",&val);
				printf("\nEnter a parent key : ");gets(str);gets(str);
				tree_insert(et,str,val,int);
				break;
			case 2 :
				printf("\nEnter value : ");scanf("%d",&val);
				printf("\nEnter a parent key : ");gets(str);gets(str);
				tree_setitem(et,str,val,int);
				break;
			case 3 :
				printf("\nEnter a parent key to delete : ");gets(str);gets(str);
				tree_delitem(et,str);
				break;
			case 4 :
				break;
			case 5 :
				printf("\nEnter a parent key : ");gets(str);gets(str);
				tree_insert(et,str,rand()%100,int);
				tree_insert(et,str,rand()%100,int);
			default :
				printf("\nInvalid Choice : ");
				break;
		}
		printtree(et,printint);
	}
	freetree(et);
	Garbagecheck();
	return 0;
}
