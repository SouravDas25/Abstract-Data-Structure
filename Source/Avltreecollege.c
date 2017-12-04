

#include "Ads\Avl.h"

#define info "\n \
choose -1 to exit. \
\n choose 0 to print info again. \
\n choose 1 to insert into tree \
\n choose 2 to delete from tree.\
\n choose 3 to insert 2 random number into tree. \
"

int main()
{
	srand(time(NULL));
	Avltree t = newAvltree(int,compareint,int);
	int c,val,pos;
	bool loop = true;
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
			case 1 : {
				printf("Enter key : ");scanf("%d",&pos);
				printf("Enter value : ");scanf("%d",&val);
				Avltree_insert(t,pos,val,int,int);
				break;
			}
			case 2 : {
				printf("Enter key : ");scanf("%d",&pos);
				Avltree_delete(t,pos,int);
				break;
			}
			case 3 : {
				Avltree_insert(t,rand()%100,rand()%100,int,int);
				Avltree_insert(t,rand()%100,rand()%100,int,int);
				break;
			}
			default :
				printf("\nInvalid choice : \n");
				break;
		}
		printAvltree(t,printint,printint);
	}
	delete(t);
	Garbagecheck();
	return 0;
}
