

#include "Ads\Extrie.h"

int main()
{
	extrie t = newextrie(int); /*create a extrie*/
	extrie_insert(t,"boro",10,int); /*insert some strings*/
	extrie_insert(t,"bina",10,int);
	extrie_insert(t,"bino",10,int);
	extrie_insert(t,"bindas",10,int);
	extrie_insert(t,"before",10,int);
	extrie_insert(t,"beaware",10,int);
	extrie_insert(t,"belly",10,int);
	extrie_insert(t,"bee",10,int);
	extrie_insert(t,"beef",10,int);
	extrie_insert(t,"beech",10,int);
	extrie_insert(t,"beach",10,int);
	extrie_insert(t,"apple",10,int);
	
	extrie_delete(t,"bindas");/*delete some strings*/
	
	printextrie(t,printint); /*print the extrie*/
	
	list l = extrie_getallwith(t,"bi"); /*get all strings starting with "bi" */
	
	ads_printf("list = {list(str)} ",l);
	
	delete(l);
	LB;
	bool b = extrie_search(t,"belly"); /*returns true if string present*/
	printbool(&b);
	delete(t);
	
	return 0;
}

