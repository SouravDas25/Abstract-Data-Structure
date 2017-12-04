
/** @cond ADS_INTERNAL */

#ifndef ADSHEAPTREE_C
#define ADSHEAPTREE_C


#include "../include/heap.h"

ads_class(ads_heap)
{
 	ads_init(ads_heap);
    int (*compare)(const void *,const void*) ;
	struct heapnode* root;
	ads_td* dtd;
	int16_t type;
	uint32_t size;
};

ads_linkClass(ads_heap,ads_heapCopy,ads_heapDelete);

#define ads_minHeap -1
#define ads_maxHeap 1

typedef struct heapnode heapnode ;

struct heapnode
{
	void * data;
	struct heapnode * left;
	struct heapnode *right;
	struct heapnode *parent;
};

ads_heap ads_initHeap(int datatype, int (*compare)(const void * data1,const void*data2),int heaptype)
{
	ads_heap h = ads_rawObject(ads_heap);
    h->type = heaptype;
	h->compare = compare;
	h->dtd = ads_tdNew(NULL,NULL,datatype);
	h->root = NULL;
	h->size = 0;
	return h;
}

void ads_heapSetTD(ads_heap self,const ads_td * td)
{
    ads_tdCopy(self->dtd,td);
}

static int __heapgetnoofchilds(heapnode * tn)
{
	int i = 0;
	if(tn == NULL)
	{
		return 0;
	}
	if(tn->left)
	{
		i++;
		i+= __heapgetnoofchilds(tn->left);
	}
	if(tn->right)
	{
		i++;
		i+= __heapgetnoofchilds(tn->right);
	}
	return i;
}

static void __insertheapify(ads_heap self,heapnode * hn)
{
	if (self->type == ads_minHeap)
	{
		while(hn->parent != NULL)
		{
			if(ads_appropriateCmp(self->compare,hn->parent->data,hn->data,ads_tdGetSize(self->dtd)) > 0)
			{
				void * tmp = hn->parent->data;
				hn->parent->data = hn->data;
				hn->data = tmp;
			}
			hn = hn->parent ;
		}
	}
	else if (self->type == ads_maxHeap)
	{
		while(hn->parent != NULL)
		{
			if(ads_appropriateCmp(self->compare,hn->parent->data,hn->data,ads_tdGetSize(self->dtd)) < 0)
			{
				void * tmp = hn->parent->data;
				hn->parent->data = hn->data;
				hn->data = tmp;
			}
			hn = hn->parent ;
		}
	}
}

ads_rt ads_heapInsert_ByRef(ads_heap self,void* value)
{
	heapnode*tn = (heapnode*)ads_calloc(1,sizeof(heapnode));
	ads_checkMemory(tn);
	tn->data = ads_appropriteCopy(self->dtd,value);
	self->size++;
	if(self->root == NULL)
	{
		self->root = tn;
	}
	else
	{
		heapnode*trav = self->root;
		while(trav->left != NULL && trav->right != NULL )
		{
			if(__heapgetnoofchilds(trav->left) <= __heapgetnoofchilds(trav->right))
			{
				trav = trav->left;
			}
			else
			{
				trav = trav->right;
			}
		}
		if(!trav->left)
		{
			trav->left = tn;
			tn->parent = trav;
		}
		else if(!trav->right)
		{
			trav->right = tn;
			tn->parent = trav;
		}
		else
		{
			return ads_registerError("Heap Insert Function");
		}
		__insertheapify(self,tn);
	}
	return ads_SUCCESS;
}

void* ads_heapPop_RtnRef(ads_heap self)
{
	self->size--;
	heapnode * hn = self->root;
	if(hn == NULL)
	{
		ads_registerError("Cannot Pop an Empty Heap in Heap Pop Function");
		return NULL;
	}
	void * val = self->root->data;
	while(hn->left != NULL || hn->right != NULL)
	{
		if(hn->left != NULL && hn->right != NULL )
		{
			if (self->type == ads_minHeap)
			{
				if(ads_appropriateCmp(self->compare,hn->left->data,hn->right->data,ads_tdGetSize(self->dtd)) < 0)
				{
					hn->data = hn->left->data ;
					hn->left->data = NULL;
					hn = hn->left;
					continue;
				}
				else
				{
					hn->data = hn->right->data ;
					hn->right->data = NULL;
					hn = hn->right;
					continue;
				}
			}
			else if (self->type == ads_maxHeap)
			{
				if(ads_appropriateCmp(self->compare,hn->left->data,hn->right->data,ads_tdGetSize(self->dtd)) > 0)
				{
					hn->data = hn->left->data ;
					hn->left->data = NULL;
					hn = hn->left;
					continue;
				}
				else
				{
					hn->data = hn->right->data ;
					hn->right->data = NULL;
					hn = hn->right;
					continue;
				}
			}
		}
		else if(hn->left && hn->right == NULL)
		{
			hn->data = hn->left->data ;
			hn->left->data = NULL;
			hn = hn->left;
			continue;
		}
		else if(hn->right && hn->left == NULL)
		{
			hn->data = hn->right->data ;
			hn->right->data = NULL;
			hn = hn->right;
			continue;
		}
	}
	heapnode * parent = hn->parent;
	if (parent == NULL )
	{
		ads_free(hn);
		self->root = NULL;
	}
	else
	{
		if(parent->left == hn)
		{
			parent->left = NULL;
			ads_free(hn);
		}
		else if(parent->right == hn)
		{
			parent->right = NULL;
			ads_free(hn);
		}
		else
		{
			ads_registerError("Heap POP Function");
			return NULL;
		}
	}
	return val;
}

static void __copyheapnode(ads_heap other,heapnode* tn )
{
	ads_heapInsert_ByRef(other,tn->data);
	if(tn->left)
	{
		__copyheapnode(other,tn->left);
	}
	if(tn->right)
	{
		__copyheapnode(other,tn->right);
	}
}

ads_heap ads_heapCopy(ads_heap _self)
{
	ads_heap self ,t;
	ads_CheckAndCastPointer(_self,ads_heap,self);
	t = ads_initHeap( ads_tdGetSize(self->dtd) ,self->compare, self->type);
    ads_tdCopy(t->dtd,self->dtd);
	if(self->root != NULL ) __copyheapnode(t,self->root);
	return t;
}

static void __printheapnode(heapnode* tn,const char* buf,void (*printfunction)(const void*))
{
	int n = strlen(buf);
	char b[n+5];
	printf(buf);
	strcpy(b,buf);
	printfunction(tn->data);
	printf("\n");
	if(tn->left && tn->right)
	{
		int l=-1;
		while(b[++l] != ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
		strcat(b,"\xc3\xc4\xc4");
		__printheapnode(tn->left,b,printfunction);
		b[n] = '\xc0';
		__printheapnode(tn->right,b,printfunction);
	}
	else if(tn->left || tn->right)
	{
		if(tn->left)
		{
			int l=-1;
			while(b[++l]!=ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
			strcat(b,"\xc0\xc4\xc4");
			__printheapnode(tn->left,b,printfunction);
		}
		if(tn->right)
		{
			int l=-1;
			while(b[++l]!= ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
			strcat(b,"\xc0\xc4\xc4");
			__printheapnode(tn->right,b,printfunction);
		}
	}
}

void ads_printHeap(ads_heap self,void (*printfunction)(const void*))
{
	printf("\nHeap BEGIN:\n\n");
	if(self->root != NULL ) __printheapnode(self->root,"  ",printfunction);
	printf("\nHeap END:\n");
}

static void __freeheapnode(heapnode*tn,ads_heap self)
{
	ads_appropriateDelete(self->dtd,tn->data);
	if(tn->left)
	{
		__freeheapnode(tn->left,self);
	}
	if(tn->right)
	{
		__freeheapnode(tn->right,self);
	}
	ads_free(tn);
}

void ads_heapDelete(ads_heap _self)
{
	ads_heap self ;
	ads_CheckAndCastPointer(_self,ads_heap,self);
	if(self->root != NULL )  __freeheapnode(self->root,self);
	ads_free(self->dtd);
	ads_free(self);
}

void* ads_heapCopy_ByRef_RtnRef(const void*v)
{
    ads_heap t ;
    ads_CheckAndCastPointer(*(ads_heap*)v,ads_heap,t);
    ads_heap cv = ads_heapCopy(t);
    return ads_toItem(cv);
}

void ads_heapDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_heap,vl);
    ads_heapDelete(vl);
}

ads_td ads_heapTDStruct = {ads_heapCopy_ByRef_RtnRef,ads_heapDelete_ByRef,sizeof(ads_heap)};

const ads_td * const ads_heapTD = &ads_heapTDStruct;

int ads_heapSizeOf(ads_heap self)
{
	return self->size*sizeof(heapnode)+sizeof(ADS_StructOf(ads_heap));
}

int ads_heapLen(ads_heap self)
{
	return self->size;
}

bool ads_heapIsEmpty(ads_heap self)
{
	if(self->root == NULL) return true;
	return false;
}

ads_list ads_heapToList(ads_heap self)
{
	ads_heap cself = (ads_heap)ads_heapCopy(self);
	ads_CheckAndCastPointer(cself,ads_heap,cself);
	ads_list l = ads_initList( ads_tdGetSize(self->dtd) );
    ads_listSetTD(l,self->dtd);
	while ( ! ads_heapIsEmpty(cself))
	{
		void *v = ads_heapPop_RtnRef(cself);
		ads_listAppend_NoCopy(l,v);
	}
	ads_heapDelete(cself);
	return l;
}

void ads_heapBuilt_FrmArray(ads_heap self,void*arr,int n)
{
	int i;char * c = (char*)arr;int datatype = ads_tdGetSize(self->dtd);
	for(i = 0;i<n;i++)
	{
		ads_heapInsert_ByRef(self,c);
		c = (c+datatype);
	}
}



#endif
/** @endcond  */
