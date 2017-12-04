

#include "Ads\Avl.h"

int main()
{
	ads_avlTree t = ads_new(ads_avlTree,int,ads_cmpInt,int); /*Create a Avltee instance*/
	
	ads_avlTreeInsert(t,1,int,30,int); /*Insert some key-Value pairs in avltree*/
	ads_avlTreeInsert(t,2,int,78,int);
	ads_avlTreeInsert(t,3,int,78,int);
	ads_avlTreeInsert(t,4,int,78,int);
	ads_avlTreeInsert(t,5,int,78,int);
	ads_avlTreeInsert(t,6,int,78,int);
	
	ads_printf("avl = {avl(d:d)}\n",t);
	//ads_printAvlTree(t,ads_printInt,ads_printInt); /*print the avltree*/
	
	ads_avlTreeDelete(t,4,int); /*delete a key from the avltree*/
	
	//ads_printAvlTree(t,ads_printInt,ads_printInt); /*print the avltree*/
	ads_printf("avl = {avl(d:d)}\n",t);
	
	int j;
	ads_avlTreeGetItem(t,6,int,int,j)

	ads_vector l = ads_avlTreeTraversal(t,ads_postOrder,ads_avlTreeKey); /*do a  post order traversal of the keys in the tree*/
	
	ads_printf("vec = {[d]} %d",l,j);
	
	ads_delete(l,t); /*delete the avl tree*/
	/*every data structure has to be deleted explicitly*/
	return 0;
}

