/** @cond ADS_INTERNAL */



#ifndef Graph_H
#define Graph_H

#include "../include/utility.h"
#include "../include/Dict.h"
#include "../include/list.h"


typedef struct graphnode
{
	void * data;
	linklist connections;
}graphnode;

ADSClassProto(graph);

ADSClass(graph)
{
	ADSinit(graph);
    int vertices;
    short datatype;
    short ads;
    int edges;
    dict raw;
};

ADSLinkMV(graph,copygraph,freegraph);

#define newgraph(n,s) init_graph(sizeof(n),s,__isADStype(#n))

graph init_graph(int datatype,int size, short ads)
{
	graph t = RawObject(graph);
	t->ads = ads;
	t->edges = 0;
	t->datatype = datatype;
	t->vertices = 0;
	t->raw = newdict(graphnode*,size);
	return t;
}

graphnode* __graphgetnode(graph self,const string key)
{
	graphnode* ptr1 = *(graphnode**)dictgetitem_NoCopy_RtnRef(self->raw,key);
	return ptr1;
}

void __graphaddvertex__(graph self,const string key,void*value)
{
	graphnode* gn = (graphnode*)mycalloc(1,sizeof(graphnode));
	gn->data = approprite_copy(self->ads,value,self->datatype);
	gn->connections = newlinklist(string);
	dict_setitem(self->raw,key,gn,graphnode*);
}

#define graph_addvertex(self,key,value) __graph_addvertex(self,key,(void**)&value)
#define cstring_graph_addvertex(self,key,value) __cstring_graph_addvertex(self,key,(void**)&value)

void __graph_addvertex(graph self,const string key,void ** value)
{
	__graphaddvertex__(self,key,evaluate_pointer(self->ads,value));
}

void __cstring_graph_addvertex(graph self,const char * key,void**value)
{
	string s = newstring(key);
	__graphaddvertex__(self,s,evaluate_pointer(self->ads,value));
	freestring(s);
}

void graph_deletevertex(graph self,const string key)
{
	if(dict_haskey(self->raw,key) == false)
	{
		ValueError("Datanode '%s' in Graph Not Found",__getcstring(key));
	}
	list l = dict_keys(self->raw);
	int i,len = list_len(l);
	for(i=0;i<len;i++)
	{
		graphnode* ptr2 = __graphgetnode(self,(string)listgetitem_NoCopy_RtnRef(l,i));
		int index = linklist_index(ptr2->connections,string_cmp_string,key,const string);
		if(index != -1)
		{
			linklist_delitem(ptr2->connections,index);
		}
	}
	freelist(l);
	graphnode** ptr1 = (graphnode**)dict_popitem_RtnRef(self->raw,key);
	graphnode* p = *ptr1;
	myfree(ptr1);
	freelinklist(p->connections);
	appropriate_delete(self->ads,p->data);
	myfree(p);
}

void cstring_graph_deletevertex(graph self,const char * key)
{
	string s = newstring(key);
	graph_deletevertex(self,s);
	freestring(s);
}

void graph_deleteconnection(graph self,const string key1,string key2)
{
	if(dict_haskey(self->raw,key1) == false)
	{
		ValueError("Datanode '%s' in Graph Not Found",__getcstring(key1));
	}
	graphnode* ptr1 = __graphgetnode(self,key1);
	if(dict_haskey(self->raw,key2) == true)
	{
		int i ,len= linklist_len(ptr1->connections);
		for(i=0;i<len;i++)
		{
			if(string_cmp_string(key2,(string)linklistgetitem_NoCopy_RtnRef(ptr1->connections,i))==0)
			{
				linklist_delitem(ptr1->connections,i);
			}
		}
	}
	else
	{
		ValueError("Datanode '%s' in Graph Not Found",__getcstring(key2));
	}
}

void cstring_graph_deleteconnection(graph self,const char * key1,const char * key2)
{
	string s = newstring(key1);
	string s1 = newstring(key2);
	graph_deleteconnection(self,s,s1);
	freestring(s);
	freestring(s1);
}

void graph_addconnection(graph self,string key1,string key2)
{
	if(dict_haskey(self->raw,key1) == false)
	{
		ValueError("Datanode '%s' in Graph Not Found",__getcstring(key1));
	}
	graphnode* ptr1 = __graphgetnode(self,key1);
	if(dict_haskey(self->raw,key2) == true)
	{
		linklistappend_ByRef(ptr1->connections,key2);
	}
	else
	{
		ValueError("Datanode '%s' in Graph Not Found",__getcstring(key2));
	}
}

void cstring_graph_addconnection(graph self,const char * key1,const char * key2)
{
	string s = newstring(key1);
	string s1 = newstring(key2);
	graph_addconnection(self,s,s1);
	freestring(s);
	freestring(s1);
}

void __graphsetitem__(graph self,const string key,void*value)
{
	void*val;
	graphnode* ptr1 = __graphgetnode(self,key);
	val = approprite_copy(self->ads,value,self->datatype);
	appropriate_delete(self->ads,ptr1->data);
	ptr1->data = val;
}

#define graph_setitem(self,key,value) __graph_setitem(self,key,(void**)&value)
#define cstring_graph_setitem(self,key,value) __cstring_graph_setitem(self,key,(void**)&value)

void __graph_setitem(graph self,const string key,void ** value)
{
	__graphsetitem__(self,key,evaluate_pointer(self->ads,value));
}

void __cstring_graph_setitem(graph self,const char * key,void**value)
{
	string s = newstring(key);
	__graphsetitem__(self,s,evaluate_pointer(self->ads,value));
	freestring(s);
}

void* graph_getitem(graph self,const string key)
{
	void*val;
	graphnode* ptr1 = __graphgetnode(self,key);
	val = approprite_copy(self->ads,ptr1->data,self->datatype);
	return val;
}

void* cstring_graph_getitem(graph self,const char * key)
{
	string s = newstring(key);
	void*val = graph_getitem(self,s);
	freestring(s);
	return val;
}

void printgraph(graph self,void (*printfunction) (void*))
{
	list l = dict_keys(self->raw);
	int i,len = list_len(l);
	printf("{ ");
	for(i=0;i<len;i++)
	{
		graphnode* ptr1 = __graphgetnode(self,(string)listgetitem_NoCopy_RtnRef(l,i));
		printfunction(ptr1->data);
		printf(" \x10 ");
		printstring(listgetitem_NoCopy_RtnRef(l,i));
		printf(" : ");
		printlinklist(ptr1->connections,printstring);
		if(i<len-1)printf(" ,\n  ");
	}
	printf(" }");
	freelist(l);
}

void* copygraph(void * _self)
{
	graph self = (graph)_self;
	assert(strcmp(self->type,"graph") == 0);
	return NULL;
}

void freegraph(void * _self)
{
	graph self = (graph)_self;
	assert(strcmp(self->type,"graph") == 0);
	list l = dict_keys(self->raw);
	int i,len = list_len(l);
	for(i=0;i<len;i++)
	{
		graphnode* p = *(graphnode**)dictgetitem_NoCopy_RtnRef(self->raw,(string)listgetitem_NoCopy_RtnRef(l,i));
		freelinklist(p->connections);
		appropriate_delete(self->ads,p->data);
		myfree(p);
	}
	freelist(l);
	freedict(self->raw);
	myfree(self);
}

#endif
/** @endcond */
