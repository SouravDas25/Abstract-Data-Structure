


#include "Ads\extrie.h"

#define info "------------------------- \
\nchoose -1 to exit \
\nchoose 0 to print info again\
\nchoose 1 to insert Words into extrie\
\nchoose 2 to search for a partial word in extrie\
\nchoose 3 to search if the exact word is present in the extrie or not\
\nchoose 4 to print extrie \
\n-------------------------\n\n"


int main()
{
	extrie et = newextrie();
	int c;
	bool loop = true;char str[50];
	printf("\nWelcome this is a extrie(Exotic Trie) program use the following option to do appropriate task on Extrie\n");
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
				printf("\nEnter a Word to insert : ");gets(str);gets(str);
				extrie_insert(et,str);
				break;
			case 2 :
				printf("\nEnter a Word or Partial Word to Search : ");gets(str);gets(str);
				list l = extrie_getallwith(et,str);ln;ln;
				printlist(l,printstring);ln;
				freelist(l);
				break;
			case 3 :
				printf("\nEnter a Word to know if it is there or not : ");gets(str);gets(str);LB;
				printbool(extrie_search(et,str));ln;
				break;
			case 4 :
				printextrie(et);
				break;
			default :
				printf("\nInvalid Choice : ");
				break;
		}
	}
	freeextrie(et);
	Garbagecheck();
	return 0;
}
