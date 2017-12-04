
#include "Ads\trie.h"

int main()
{
	ads_trie t = ads_trieNew(int);
	ads_trieInsert(t,"bina",52,int);
	ads_trieInsert(t,"binod",85,int);
	ads_trieInsert(t,"bipin",32,int);
	ads_trieInsert(t,"binaly",20,int);

	ads_printTrie(t,ads_printInt);

	int v ;
	ads_trieGetItem(t,"binaly",int,v);
	ads_output("Trie_Getitem = %d \n",v);
	ads_trieDelItem(t,"binaly");

	ads_printTrie(t,ads_printInt);
	ads_delete(t);

	ads_gcDump();
	return 0;
}
