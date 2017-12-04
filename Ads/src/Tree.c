

/** @cond ADS_INTERNAL */

#ifndef ADSUNBALANCEDTREE_C
#define ADSUNBALANCEDTREE_C



#include "../include/tree.h"

ads_class(ads_treeLeaf)
{
	ads_init(ads_treeLeaf);
	ads_list siblings;
	ads_treeLeaf parent;
	void * data;
	ads_td * dtd;
};

ads_class(ads_tree)
{
	ads_init(ads_tree);
	ads_list root;
	ads_td * dtd;
	uint32_t noofleaves;
};

ads_linkClass(ads_treeLeaf,ads_treeLeafCopy,ads_treeLeafDelete);
ads_linkClass(ads_tree,ads_treeCopy,ads_treeDelete);

ads_treeLeaf ads_treeLeafNew(void*data,ads_treeLeaf parent,ads_td * pdtd)
{
	ads_treeLeaf tl = ads_rawObject(ads_treeLeaf);
	tl->data = (!data)?ads_appropriteCopy(pdtd,data):data;
	tl->dtd = ads_tdNew(NULL,NULL,0);
	ads_tdCopy(tl->dtd,pdtd);
	tl->siblings = NULL;
	tl->parent = parent;
	return tl;
}

ads_treeLeaf ads_treeLeafCopy(ads_treeLeaf _self)
{
	ads_treeLeaf self ;
	ads_CheckAndCastPointer(_self,ads_treeLeaf,self);
	ads_treeLeaf tl = ads_treeLeafNew( self->data, self->parent,self->dtd);
	if(self->siblings != NULL)
	{
		tl->siblings = ads_listCopy(self->siblings);
	}
	return tl;
}

void ads_treeLeafDelete(ads_treeLeaf _self)
{
	ads_treeLeaf self  ;
	ads_CheckAndCastPointer(_self,ads_treeLeaf,self);
	if(self->data != NULL ) ads_appropriateDelete(self->dtd,self->data);//*(int*)self->data
	if(self->siblings != NULL)
	{
		ads_listDelete(self->siblings);
	}
	ads_free(self->dtd);
	ads_free(self);
}

void ** ads_treeLeafCopy_ByRef(void ** self)
{
    ads_treeLeaf t = ads_treeLeafCopy(*(ads_treeLeaf*)self);
    return ads_toItem(t);
}

void ads_treeLeafDelete_ByRef(void ** self)
{
    ads_treeLeaf t = (ads_treeLeaf)ads_fromItem(self);
    ads_treeLeafDelete(t);
}

ads_tree ads_initTree(int datatype)
{
	ads_tree t = ads_rawObject(ads_tree);
	t->dtd = ads_tdNew(NULL,NULL,datatype);
	t->root = ads_listNew(ads_treeLeaf);
	ads_listSetTD(t->root,ads_treeLeafCopy_ByRef,ads_treeLeafDelete_ByRef);
	t->noofleaves = 0;
	return t;
}

void ads_treeSetCD(ads_tree self,void** (*copyConstructor) (void**),void (*destructor)(void**))
{
    ads_setCCD(self->dtd,copyConstructor,destructor);
}

ads_treeLeaf ads_treeGetToBranch(ads_tree self,const char * parent)
{
	int len = strlen(parent);
	if(len == 0) return NULL;
	ads_vector l = ads_cstringSplit(parent,".");
	ads_list pntsl = self->root;ads_treeLeaf pnt=NULL;
	ads_foreach(str,ads_vectorIterator(l),ads_string)
	{
		if(pntsl == NULL)
		{
			ads_valueError("key(%s) Provided For ads_tree is not valid Parent",parent);
		}
		int64_t p = ads_stringToInt64(str,10);
		if(pntsl == NULL || p > (int)ads_listLen(pntsl)-1 || p < 0)
		{
			ads_valueError("key(%s) Provided For ads_tree is not valid Parent",parent);
		}
		ads_listGetItem_NoCopy(pntsl,p,ads_treeLeaf,pnt);
		pntsl = pnt->siblings;
	}ads_next(str);
	ads_vectorDelete(l);
	return pnt;
}

static void treeleaf_addsibling(ads_treeLeaf parent,ads_treeLeaf child)
{
	if(parent->siblings == NULL)
	{
		parent->siblings = ads_listNew(ads_treeLeaf);
		ads_listSetCD(parent->siblings,ads_treeLeafCopy_ByRef,ads_treeLeafDelete_ByRef);
	}
	ads_listAppend(parent->siblings,child,ads_treeLeaf);
}

void ads_treeInsert_ByRef(ads_tree self,const char * parents,void*data)
{
	ads_treeLeaf pnt = ads_treeGetToBranch(self,parents);
	ads_treeLeaf tl = ads_treeLeafNew(data,pnt,self->dtd);
	if(pnt == NULL)
	{
		ads_listAppend(self->root,tl,ads_treeLeaf);
	}
	else treeleaf_addsibling(pnt,tl);
	ads_treeLeafDelete(tl);
	self->noofleaves++;
}

void* ads_treeGetItem_RtnRef(ads_tree self,char * parents)
{
	ads_treeLeaf pnt = ads_treeGetToBranch(self,parents);
	if(pnt == NULL)
	{
		ads_keyError("Tree Root Dosn't Contain Any Value");
	}
	void * v = ads_appropriteCopy(self->dtd,pnt->data);
	return v;
}

void ads_treeSetItem_ByRef(ads_tree self,const char * parents,void*data)
{
	ads_treeLeaf pnt = ads_treeGetToBranch(self,parents);
	if(pnt == NULL)
	{
		ads_keyError("Tree Root Dosn't Contain Any Value");
	}
	void * v = ads_appropriteCopy(self->dtd,data);
	if(pnt->data != NULL ) ads_appropriateDelete(self->dtd,pnt->data);
	pnt->data = v;
}

void ads_treeDelItem(ads_tree self,const char * parents)
{
	ads_treeLeaf pnt = ads_treeGetToBranch(self,parents);
	if(pnt == NULL)
	{
		ads_listClear(self->root);
		return ;
	}
	ads_treeLeaf pntofleaf = pnt->parent;
	if(pntofleaf == NULL )
	{
		ads_listRemove(self->root,ads_cmpMem,pnt,ads_treeLeaf);
	}
	else
	{
		ads_listRemove(pntofleaf->siblings,ads_cmpMem,pnt,ads_treeLeaf);
	}
}

void __printtreeleaf(ads_treeLeaf tn,const char* buf,void(*printfunction)(void*))
{
	int n = strlen(buf);
	char b[n+5];
	ads_output(buf);
	strcpy(b,buf);
	if(tn->data == NULL) ads_output("\b\bRoot");else printfunction(tn->data);
	ads_output("\n");
	if(tn->siblings)
	{
		int ii=0,len = ads_listLen(tn->siblings);
		if(len == 1)
		{
			int l=-1;
			while(b[++l]!= ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
			strcat(b,"\xc0\xc4\xc4");
			__printtreeleaf((ads_treeLeaf)ads_listGetItem_NoCopy_RtnRef(tn->siblings,0),b,printfunction);
		}
		else if(len > 1)
		{
			int l=-1;
			while(b[++l] != ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
			strcat(b,"\xc3\xc4\xc4");
			ads_foreach(itm,ads_listIterator(tn->siblings),ads_treeLeaf)
			{
				if(ii==len-1)
				{
					b[n] = '\xc0';
					__printtreeleaf(itm,b,printfunction);
				}
				else
				{
					__printtreeleaf(itm,b,printfunction);
				}
				ii++;
			}ads_next(itm);
		}
	}
}

void ads_printTree(ads_tree self,void(*printfunction)(void*))
{
	ads_output("\nTree BEGIN:\n");
	ads_treeLeaf pnt1 = ads_treeLeafNew(NULL,NULL,self->dtd);
	pnt1->siblings = self->root;
	ads_treeLeaf pnt = pnt1;
	if(self->root != NULL ) __printtreeleaf(pnt,"  ",printfunction);
	ads_free(pnt1);
	ads_output("\nTree END:\n");
}

ads_tree ads_treeCopy(ads_tree _self)
{
	ads_tree self  ;
	ads_CheckAndCastPointer(_self,ads_tree,self);
	ads_tree t1 = ads_rawObject(ads_tree);
	ads_varCCDcopy(t1->dtd,self->dtd);
	t1->root = (ads_list)ads_listCopy(self->root);
	t1->noofleaves = self->noofleaves;
	return t1;
}

void ads_treeDelete(ads_tree _self)
{
	ads_tree self ;
	ads_CheckAndCastPointer(_self,ads_tree,self);
	ads_listDelete(self->root);
	ads_free(self->dtd);
	ads_free(self);
}

#endif

/** @endcond  */
