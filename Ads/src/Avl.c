/** @cond ADS_INTERNAL */

#ifndef AVLTREE_C
#define AVLTREE_C

#include "../include/Avl.h"

ads_class(ads_avl)
{
	ads_init(ads_avl);
    int (*compare)(const void *,const void*) ;
	ads_avlNode * root;
	ads_td* dtd;
	ads_td* ktd;
	uint16_t height;
	uint16_t size;
};

ads_linkClass(ads_avl,ads_avlCopy,ads_avlDelete);

struct ads_avlNode
{
	void * key;
	void * data;
	struct ads_avlNode * left;
	struct ads_avlNode *right;
	struct ads_avlNode *parent;
};

static void ads_treeRotate(ads_avl self,ads_avlNode*tn,bool direction);
static void ads_treeDoubleRotate(ads_avl self,ads_avlNode*tn,bool direction);
static int ads_getLongestHeight(ads_avlNode*tn);

ads_avl ads_initAvl(int keytype, int (*compare)(const void * data1,const void*data2),int datatype)
{
	ads_avl t = ads_rawObject(ads_avl);
	if( ads_checkMemory(t) != ads_SUCCESS ) return t ;
	t->height = 0;
	t->size = 0;
	t->root = NULL;
	t->ktd  = ads_tdNew(NULL,NULL,keytype);
	t->dtd = ads_tdNew(NULL,NULL,datatype);
	t->compare = compare;
	if(!ADS_MethodVariableName(ads_avl).TypeCode) { /*adding the printtype and adstypes at runtime instead of compile time*/
		ADS_MethodVariableName(ads_avl).TypeCode = ads_genericIoInsert(ads_printAvlInternal,"avl(#:#)",4,5);
	}
	return t;
}

ads_rt ads_avlSetTD(ads_avl self, bool key_or_data ,const ads_td * td)
{
    if( key_or_data == ads_avlData )
    {
        if( ads_avlLen(self) > 0 ) return ads_registerError("Type definition For Data Can Only Be Set Before Any Element is added.");
        ads_tdCopy(self->dtd,td);
    }
    else
    {
        if( ads_avlLen(self) > 0 ) return ads_registerError("Type definition For Key Can Only Be Set Before Any Element is added.");
        ads_tdCopy(self->ktd,td);
    }
    return ads_SUCCESS;
}

void ads_treeRotate(ads_avl self,ads_avlNode*tn,bool direction)
{
	if(tn== NULL)
	{
		return;
	}
	ads_avlNode*parent = tn->parent,*child;
	if(direction == ads_left)
	{
		child = tn->left;
		tn->left = child->right;
		if(child->right != NULL)
		child->right->parent = tn;
		child->right = tn;
		child->parent = tn->parent;
		tn->parent = child;
	}
	else if(direction == ads_right)
	{
		child = tn->right;
		tn->right = child->left;
		if(child->left != NULL)
		child->left->parent = tn;
		child->left = tn;
		child->parent = tn->parent;
		tn->parent = child;
	}
	if(parent == NULL)
	{
		self->root = child;
		return;
	}
	if(parent->left == tn)
	{
		parent->left = child;
	}
	else if(parent->right == tn)
	{
		parent->right = child;
	}
}

void ads_treeDoubleRotate(ads_avl self,ads_avlNode*tn,bool direction)
{
	if(direction == ads_left)
	{
		ads_treeRotate(self,tn->left,ads_right);
		ads_treeRotate(self,tn,ads_left);
	}
	else if(direction == ads_right)
	{
		ads_treeRotate(self,tn->right,ads_left);
		ads_treeRotate(self,tn,ads_right);
	}
}

int ads_getLongestHeight(ads_avlNode*tn)
{
	int r=0,l=0;
	if(tn->right)
	{
		r = ads_getLongestHeight(tn->right)+1;
	}
	if(tn->left)
	{
		l = ads_getLongestHeight(tn->left)+1;
	}
	if(r>l) return r;
	else if (r == l) return r;
	else return l;
}

static int ads_getLeftHeight(ads_avlNode*tn)
{
	if(tn->left)
	{
		return ads_getLongestHeight(tn->left)+1;
	}
	return 0;
}

static int ads_getRightHeight(ads_avlNode*tn)
{
	if(tn->right)
	{
		return ads_getLongestHeight(tn->right)+1;
	}
	return 0;
}

static void ads_balanceAvlNode( ads_avl self,ads_avlNode*tn)
{
	int diff = ads_getLeftHeight(tn)-ads_getRightHeight(tn);
	if( diff > 1 )
	{
	    int ldiff = ads_getLeftHeight(tn->left) - ads_getRightHeight(tn->left);
		if( ldiff < 0 ) ads_treeDoubleRotate(self,tn,ads_left);
		else ads_treeRotate(self,tn,ads_left);
	}
	else if( diff < -1 )
	{
        int ldiff = ads_getLeftHeight(tn->right) - ads_getRightHeight(tn->right);
		if( ldiff > 0 ) ads_treeDoubleRotate(self,tn,ads_right);
		else ads_treeRotate(self,tn,ads_right);
	}
}

int ads_countTreeStack = 0;
static void ads_treeNode2Array(ads_avlNode * tn,ads_avlNode ** arr )
{
	if(tn)
	{
		arr[ads_countTreeStack++] = tn;
	}
	if(tn->left)
	{
		ads_treeNode2Array(tn->left,arr);
	}
	if(tn->right)
	{
		ads_treeNode2Array(tn->right,arr);
	}
}

ads_rt ads_balanceAvl( ads_avl self)
{
    if(self == NULL) return ads_registerError("Avl tree Not Initialized.");
	int i;
	ads_countTreeStack = 0;
	ads_avlNode * arr[self->size+1];
	ads_treeNode2Array(self->root,arr);
	ads_avlNode*trav;
	for(i =0;i<self->size;i++)
	{
		trav = arr[i];
		ads_balanceAvlNode(self,trav);
	}
	return ads_SUCCESS;
}



int ads_avlLen(ads_avl self)
{
    if(self == NULL)
    {
        ads_registerError("Avl tree Not Initialized.");
        return -1;
    }
	return self->size;
}


ads_rt ads_avlInsert_ByRef(ads_avl self,void*key,void* value)
{
    if(self == NULL) return ads_registerError("Avl tree Not Initialized.");
    if(value == NULL) return ads_registerError("Cannot copy NULL in ads_avl insert.");
    if(key == NULL) return ads_registerError("Cannot copy NULL in ads_avl insert.");
    ads_avlNode*trav = self->root;
	ads_avlNode*tn = (ads_avlNode*)ads_calloc(1,sizeof(ads_avlNode));
	if( ads_checkMemory(tn) != ads_SUCCESS ) return ads_ERROR;
	tn->data = ads_appropriteCopy(self->dtd,value);
	tn->key = ads_appropriteCopy(self->ktd,key);
	self->size++;
	if(self->root == NULL)
	{
		self->root = tn;
		self->height++;
	}
	else
	{
		while(trav != NULL)
		{
			if(ads_appropriateCmp(self->compare,tn->key,trav->key,ads_tdGetSize(self->ktd)) < 0)
			{
				if(trav->left)
				{
					trav = trav->left;
					continue;
				}
				else
				{
					tn->parent = trav;
					trav->left = tn;
					break;
				}
			}
			else if(ads_appropriateCmp(self->compare,tn->key,trav->key,ads_tdGetSize(self->ktd)) > 0)
			{
				if(trav->right)
				{
					trav = trav->right;
					continue;
				}
				else
				{
					tn->parent = trav;
					trav->right = tn;
					break ;
				}
			}
			else
            {
                ads_appropriateDelete(self->dtd,trav->data);
                ads_appropriateDelete(self->ktd,tn->key);
                trav->data = tn->data;
                ads_free(tn);
                break;
            }
		}
	}
	while(trav != NULL)
	{
		ads_balanceAvlNode(self,trav);
		trav = trav->parent;
	}
	return ads_SUCCESS;
}

static ads_avlNode * ads_getNode(ads_avl self,const void*key)
{
    if(!key) return NULL;
	ads_avlNode*trav = self->root;
	while(trav != NULL)
	{
	    int cmp = ads_appropriateCmp(self->compare,key,trav->key,ads_tdGetSize(self->ktd));
		if( cmp == 0)
		{
			return trav;
		}
		if(trav->left)
		{
			if( cmp < 0)
			{
				trav = trav->left;
				continue;
			}
		}
		if(trav->right)
		{
			if( cmp > 0)
			{
				trav = trav->right;
				continue;
			}
		}
		break;
	}
	return NULL;
}

bool ads_avlContains_ByRef(ads_avl self,void*key)
{
    if(self == NULL)
    {
        ads_registerError("Avl tree Not Initialized.");
        return false;
    }
    if(key == NULL) return ads_registerError("Value Error Key is Null");
	ads_avlNode * j = ads_getNode(self,key);
	if(j!=NULL) return true;
	return false;
}

const void* ads_avlGet_NoCopy_ByRef(ads_avl self,const void*key)
{
    if(!self)
    {
        ads_registerError("Avltree Not Initialized.");
        return NULL;
    }
    ads_avlNode * j = ads_getNode(self,key);
	if(j != NULL)
    {
        return j->data;
    }
    ads_registerError("Key Not Found.");
	return NULL;
}

void* ads_avlGet_Copy_ByRef(ads_avl self,const void*key)
{
    const void * val = ads_avlGet_NoCopy_ByRef(self,key);
    if(val)return ads_appropriteCopy(self->dtd,val);
	return NULL;
}

const void* ads_avlGetMax_NoCopy_RtnRef(ads_avl self)
{
    if(!self)
    {
        ads_registerError("Avltree Not Initialized.");
        return NULL;
    }
    ads_avlNode*trav = self->root;
	while(trav != NULL)
	{
		if(trav->right)
		{
			trav = trav->right;
		}
		else
        {
            return trav->key;
        }
	}
	ads_registerError("No key Present in the AVl Tree. ");
    return NULL;
}

void* ads_avlGetMax_Copy_RtnRef(ads_avl self)
{
    const void * val = ads_avlGetMax_NoCopy_RtnRef(self);
    if(val) return ads_appropriteCopy(self->ktd,val);
    return NULL;
}

const void* ads_avlGetRoot_NoCopy_RtnRef(ads_avl self)
{
    if(!self)
    {
        ads_registerError("Avltree Not Initialized.");
        return NULL;
    }
    ads_avlNode*trav = self->root;
	if(trav != NULL)
	{
		return trav->key;
	}
	ads_registerError("No key Present in the Avltree. ");
    return NULL;
}

void* ads_avlGetRoot_Copy_RtnRef(ads_avl self)
{
    const void * val = ads_avlGetRoot_NoCopy_RtnRef(self);
    if(val) return ads_appropriteCopy(self->ktd,val);
    return NULL;
}

const void* ads_avlGetMin_NoCopy_RtnRef(ads_avl self)
{
    if(!self)
    {
        ads_registerError("Avltree Not Initialized.");
        return NULL;
    }
    ads_avlNode*trav = self->root;
	while(trav != NULL)
	{
		if(trav->left)
		{
			trav = trav->left;
		}
		else
        {
            return trav->key;
        }
	}
	ads_registerError("No key Present in the AVl Tree.");
    return NULL;
}

void* ads_avlGetMin_Copy_RtnRef(ads_avl self)
{
    const void * val = ads_avlGetMin_NoCopy_RtnRef(self);
    if(val) return ads_appropriteCopy(self->ktd,val);
    return NULL;
}

static int ads_getNoOfChild(ads_avlNode*tn)
{
	int i = 0;
	if(tn->left != NULL)
	{
		i++;
	}
	if(tn->right != NULL)
	{
		i++;
	}
	return i;
}

static ads_rt ads_avlRemoveNode(ads_avl self,ads_avlNode * node)
{
    if(self == NULL) return ads_registerError("Avl Tree Not initialized.");
    if(node == NULL)
	{
		return ads_registerError("Cannot Free A Null Node.");
	}
	ads_avlNode*parent = node->parent;
	int nc = ads_getNoOfChild(node);
	if(nc==0)
	{
		if(parent == NULL)
		{
			self->root = NULL;
		}
		else
		{
			if(parent->left == node)
			{
				parent->left = NULL;
			}
			else if(parent->right == node)
			{
				parent->right = NULL;
			}
			else
			{
				return ads_registerError("Parent mismatch in Remove function of avl Tree");
			}
		}
	}
	else if(nc==1)
	{
		if(node->left)
		{
			if(parent == NULL)
			{
				self->root = node->left;
				node->left->parent =parent;
			}
			else if(parent->left == node)
			{
				parent->left = node->left;
				node->left->parent =parent;
			}
			else if(parent->right == node)
			{
				parent->right = node->left;
				node->left->parent =parent;
			}
			else
			{
				return ads_registerError("Parent mismatch in Remove function of avl Tree");
			}
		}
		else if(node->right)
		{
			if(parent == NULL)
			{
				self->root = node->right;
				node->right->parent =parent;
			}
			else if(parent->left == node)
			{
				parent->left = node->right;
				node->right->parent =parent;
			}
			else if(parent->right == node)
			{
				parent->right = node->right;
				node->right->parent = parent;
			}
			else
			{
				return ads_registerError("Parent mismatch in Remove function of avl Tree");
			}
		}
		else
		{
			return ads_registerError("Nodecount dosn't match with the node");
		}
	}
	else if(nc == 2)
	{
		ads_avlNode *leastnode=node->left,*lparent ;
		while(leastnode->right != NULL)
		{
			leastnode = leastnode->right;
		}
		ads_appropriateDelete(self->dtd,node->data);
		ads_appropriateDelete(self->ktd,node->key);
		node->data = leastnode->data;
		node->key = leastnode->key;
		lparent = leastnode->parent;
		if(lparent->right == leastnode )
		{
			if(leastnode->left)
			{
				lparent->right = leastnode->left;
				leastnode->left->parent = lparent;
			}
			else
			{
				lparent->right = NULL;
			}
		}
		else if(lparent->left == leastnode)
		{
			if(leastnode->left)
			{
				lparent->left = leastnode->left;
				leastnode->left->parent = lparent;
			}
			else
			{
				lparent->left = NULL;
			}
		}
		self->size--;
		ads_avlNode*trav = leastnode->parent;
		ads_free(leastnode);
		while(trav != NULL)
		{
			ads_balanceAvlNode(self,trav);
			trav = trav->parent;
		}
		return ads_SUCCESS;
	}
	ads_appropriateDelete(self->dtd,node->data);
	ads_appropriateDelete(self->ktd,node->key);
	ads_avlNode*trav = node->parent;
	ads_free(node);
	self->size--;
	while(trav != NULL)
	{
		ads_balanceAvlNode(self,trav);
		trav = trav->parent;
	}
	return ads_SUCCESS;
}

void* ads_avlPop_ByRef(ads_avl self,const void*key)
{
    if(self == NULL)
    {
        ads_registerError("Avl Tree Not initialized.");
        return NULL;
    }
    if(!key)
    {
        ads_registerError("Avl Tree Cannot Pop Null key.");
        return NULL;
    }
	ads_avlNode*node = ads_getNode(self,key);
	if(node == NULL)
	{
		ads_registerError("Key Not Found Error.");
		return NULL;
	}
	void * val = ads_appropriteCopy(self->dtd,node->data);
    if( ads_avlRemoveNode(self,node) != ads_SUCCESS)
    {
        ads_appropriateDelete(self->dtd,val);
        return NULL ;
    }
    return val;
}

ads_rt ads_avlRemove_ByRef(ads_avl self,const void*key)
{
    if(self == NULL) return ads_registerError("Avl Tree Not initialized.");
    if(!key) return ads_registerError("Avl Tree Cannot Remove Null key.");
	ads_avlNode*node = ads_getNode(self,key);
	if(node == NULL)
	{
		return ads_registerError("Key Not Found Error.");
	}
    return ads_avlRemoveNode(self,node);
}

static void ads_freeNode(ads_avlNode*tn,ads_avl self)
{
	ads_appropriateDelete(self->dtd,tn->data) ;
	ads_appropriateDelete(self->ktd,tn->key);
	if(tn->left)
	{
		ads_freeNode(tn->left,self);
	}
	if(tn->right)
	{
		ads_freeNode(tn->right,self);
	}
	ads_free(tn);
}

void ads_avlDelete(ads_avl _self)
{
	ads_avl self ;
	ads_CheckAndCastPointer(_self,ads_avl,self);
	if(self->root != NULL)
	ads_freeNode(self->root,self);
	ads_free(self->dtd);
	ads_free(self->ktd);
	ads_free(self);
}

static void ads_avlPrintNode(ads_avlNode* tn,const char* buf,void (*keyads_outputunction)(void*),void (*dataads_outputunction)(void*))
{
	int n = strlen(buf);
	char * b = ads_new(ads_array,char,n+5);
	ads_output(buf);
	strcpy(b,buf);
	keyads_outputunction(tn->key);
	ads_output(": ");
	dataads_outputunction(tn->data);
	ads_output("\n");
	if(tn->left && tn->right)
	{
		int l=-1;
		while(b[++l] != ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
		strcat(b,"\xc3\xc4\xc4");
		ads_avlPrintNode(tn->left,b,keyads_outputunction,dataads_outputunction);
		b[n] = '\xc0';
		ads_avlPrintNode(tn->right,b,keyads_outputunction,dataads_outputunction);
	}
	else if(tn->left || tn->right)
	{
		if(tn->left)
		{
			int l=-1;
			while(b[++l]!= ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
			strcat(b,"\xc0\xc4\xc4");
			ads_avlPrintNode(tn->left,b,keyads_outputunction,dataads_outputunction);
		}
		if(tn->right)
		{
			int l=-1;
			while(b[++l]!= ads_endOfStr) if(b[l] != '\xb3')(b[l] == '\xc3') ? b[l] = '\xb3' : (b[l] = ' ') ;
			strcat(b,"\xc0\xc4\xc4");
			ads_avlPrintNode(tn->right,b,keyads_outputunction,dataads_outputunction);
		}
	}
	ads_free(b);
}

void ads_printAvl(ads_avl self,void (*keyads_outputunction)(void*),void (*dataads_outputunction)(void*))
{
	ads_output("AVL Tree : \n");
	if(self->root != NULL) ads_avlPrintNode(self->root,"  ",keyads_outputunction,dataads_outputunction);
}

static void ads_avlPrintNodeInternal(ads_avlNode* tn,ads_genericPrint keyprint,ads_genericPrint dataprint,const char * cmd ,int size,int cutoff)
{
    ads_putChar(' ');
    keyprint(tn->key,&cmd[cutoff],size);
    ads_putChar(':');
    dataprint(tn->data,&cmd[cutoff],size);
    ads_putChar(' ');
    ads_putChar(',');
    if(tn->left) ads_avlPrintNodeInternal(tn->left,keyprint,dataprint,cmd,size,cutoff);
    if(tn->right) ads_avlPrintNodeInternal(tn->right,keyprint,dataprint,cmd,size,cutoff);
}

void ads_printAvlInternal(void*add,const char*cmd,int size)
{
    ads_avl self ;
    add = *(void**)add;
	ads_CheckAndCastPointer(add,ads_avl,self);
    int cutoff;
    ads_iList l = ads_getPrintFunction(cmd,&size,&cutoff);
	ads_genericPrint keyprint,dataprint;

	if(ads_iListLen(l)>1)
	{
		ads_iListGetItem(l,0,ads_genericPrint,keyprint);
		ads_iListGetItem(l,1,ads_genericPrint,dataprint);
	}
	else
	{
		ads_output("ads_avl Print Error");
		return ;
	}
	ads_deleteIList(l);

    ads_output("ads_avl{");
    ads_avlPrintNodeInternal(self->root,keyprint,dataprint,cmd,size,cutoff);
    ads_output("\b}");

}

static void ads_copyAvlNode(ads_avl other,ads_avlNode* tn )
{
	ads_avlInsert_ByRef(other,tn->key,tn->data);
	if(tn->left)
	{
		ads_copyAvlNode(other,tn->left);
	}
	if(tn->right)
	{
		ads_copyAvlNode(other,tn->right);
	}
}

ads_avl ads_avlCopy(ads_avl _self)
{
	ads_avl self,t ;
	ads_CheckAndCastPointer(_self,ads_avl,self);

	t = ads_initAvl(ads_tdGetSize(self->ktd) ,self->compare, ads_tdGetSize(self->dtd));
	ads_tdCopy(t->dtd,self->dtd);
	ads_tdCopy(t->ktd,self->ktd);
	if(self->root != NULL) ads_copyAvlNode(t,self->root);
	return t;
}

static void* ads_avlCopy_ByRef_RtnRef(const void*v)
{
    ads_avl t ;
    ads_CheckAndCastPointer(*(ads_avl*)v,ads_avl,t);
    ads_avl cv = ads_avlCopy(t);
    return ads_toItem(cv);
}

static void ads_avlDelete_ByRef(void*v)
{
    void * vl = ads_fromItem(v);
    ads_CheckAndCastPointer(vl,ads_avl,vl);
    ads_avlDelete(vl);
}

ads_td ads_avlTDStruct = {ads_avlCopy_ByRef_RtnRef,ads_avlDelete_ByRef,sizeof(ads_avl)};

const ads_td * const ads_avlTD = &ads_avlTDStruct;

static void ads_avlNodeTraversal(ads_vector l , ads_avlNode * tn,int order,bool keyordata)
{
    if(tn && l)
    {
        if(order == ads_inOrder)
        {
            if(tn->left)ads_avlNodeTraversal(l,tn->left,order,keyordata);
            if(keyordata) ads_vectorAppend_ByRef(l,tn->key);
            else ads_vectorAppend_ByRef(l,tn->data);
            if(tn->right)ads_avlNodeTraversal(l,tn->right,order,keyordata);
        }
        else if(order == ads_preOrder)
        {
            if(keyordata) ads_vectorAppend_ByRef(l,tn->key);
            else ads_vectorAppend_ByRef(l,tn->data);
            if(tn->left)ads_avlNodeTraversal(l,tn->left,order,keyordata);
            if(tn->right)ads_avlNodeTraversal(l,tn->right,order,keyordata);
        }
        else
        {
            if(tn->left)ads_avlNodeTraversal(l,tn->left,order,keyordata);
            if(tn->right)ads_avlNodeTraversal(l,tn->right,order,keyordata);
            if(keyordata) ads_vectorAppend_ByRef(l,tn->key);
            else ads_vectorAppend_ByRef(l,tn->data);
        }
    }

}

ads_vector ads_avlTraversal(ads_avl self,int order,bool key_or_data)
{
    ads_rt rt;
    ads_CheckPointer(self,ads_avl,rt);
    if(rt != ads_SUCCESS)
    {
        return NULL;
    }
	ads_vector l;
	if(key_or_data)
    {
        l = ads_initVector(ads_tdGetSize(self->ktd),self->size);
        ads_vectorSetTD(l,self->ktd);
    }
    else
    {
        l = ads_initVector(ads_tdGetSize(self->dtd) ,self->size);
        ads_vectorSetTD(l,self->dtd);
    }
	ads_avlNodeTraversal(l,self->root,order,key_or_data);
	return l;
}
#endif

/** @endcond */

