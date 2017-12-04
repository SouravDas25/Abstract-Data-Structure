
#include "collection\Graph.c"

int main()
{
	graph g = newgraph(int,10);
	int i = 52;
	i+=2;
	cstring_graph_addvertex(g,"A",i);
	cstring_graph_addvertex(g,"B",i);
	cstring_graph_addvertex(g,"C",i);
	
	cstring_graph_addconnection(g,"A","B");
	cstring_graph_addconnection(g,"B","C");
	cstring_graph_addconnection(g,"B","A");
	cstring_graph_addconnection(g,"C","B");
	cstring_graph_addconnection(g,"C","A");
	cstring_graph_addconnection(g,"A","C");

	printgraph(g,printint);
	freegraph(g);
	return 0;
}
