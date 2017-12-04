/** @cond ADS_INTERNAL */

#ifndef garbagecollector_C
#define garbagecollector_C

#include "../../include/utility/gc.h"



typedef struct ads_gcpointer { /*ads_stack node struct declaration*/
    struct ads_gcpointer* prev ;
	struct ads_gcpointer* next ;
	short size;
	short ads;
	int NumberOfItems;
} ads_gcpointer ;

uint64_t ADS_CurrentMemoryActive = 0;
uint64_t ADS_GC_MaxMemoryActive = 0;
uint64_t ads_maxGCalocateinlist__ = 0; /*maximum number of dynamically alocated data active in occupied ads_stack at some point */
uint64_t ads_LenGCcalls__ = 0; /*current no of active dynamically allocated data in occupied ads_stack */
uint64_t ads_TotalGCcalls__ = 0; /*total no of call made to mycalloc*/
ads_gcpointer * ads_gcobjhead__ = NULL; /*ads_stack head for occupied ads_stack */


void * ads_getGcAllocatedBlock(ads_gcpointer* gcblock )
{
    return ((char*)gcblock) + sizeof(ads_gcpointer);
}

ads_gcpointer* ads_getGcBlock(void*allocated)
{
    return (ads_gcpointer*)(((char*)allocated) - sizeof(ads_gcpointer));
}

void* ads_calloc(int n,int size) /*allocate dynamic memory in a safe way maintaing a ads_stack of occupied element */
{
    if( n <= 0 || size <= 0 )
    {
        ads_registerError("Cannot Allocate Memory With 0 Size.");
        return NULL;
    }
	ads_gcpointer*p = (ads_gcpointer*)calloc(n,sizeof(ads_gcpointer)+size);
	if( !p )
    {
        perror("Fatal Error From Ads_Calloc ");
        return NULL;
    }
    void * allocated = ads_getGcAllocatedBlock(p);
	p->NumberOfItems = n;
	p->size = size;
	p->prev = NULL;
	p->next = ads_gcobjhead__;
	p->ads = ads_typeFalse;
	if(ads_gcobjhead__)ads_gcobjhead__->prev = p;
	ads_gcobjhead__ = p;
	ADS_CurrentMemoryActive += n*size;
	ads_TotalGCcalls__++;
	ads_LenGCcalls__++;
	if(ads_LenGCcalls__ > ads_maxGCalocateinlist__)
	ads_maxGCalocateinlist__ = ads_LenGCcalls__ ;
	if(ADS_CurrentMemoryActive > ADS_GC_MaxMemoryActive ) ADS_GC_MaxMemoryActive = ADS_CurrentMemoryActive;
	ads_flushVariable();
	return allocated;
}

void* ads_rawCalloc(int n,int size) /*allocate dynamic memory in a safe way maintaing a ads_stack of occupied element */
{
    if( n <= 0 || size <= 0 )
    {
        ads_registerError("Cannot Allocate Memory With 0 Size.");
        return NULL;
    }
	ads_gcpointer*p = (ads_gcpointer*)calloc(n,sizeof(ads_gcpointer)+size);
	if( !p )
    {
        perror("Fatal Error From Ads_Calloc ");
        return NULL;
    }
    void * allocated = ads_getGcAllocatedBlock(p);
	p->NumberOfItems = n;
	p->size = size;
	p->prev = NULL;
	p->next = ads_gcobjhead__;
	p->ads = ads_typeTrue;
	if(ads_gcobjhead__)ads_gcobjhead__->prev = p;
	ads_gcobjhead__ = p;
	ADS_CurrentMemoryActive += n*size;
	ads_TotalGCcalls__++;
	ads_LenGCcalls__++;
	if(ads_LenGCcalls__ > ads_maxGCalocateinlist__)
	ads_maxGCalocateinlist__ = ads_LenGCcalls__ ;
	if(ADS_CurrentMemoryActive > ADS_GC_MaxMemoryActive ) ADS_GC_MaxMemoryActive = ADS_CurrentMemoryActive;
	ads_flushVariable();
	return allocated;
}

void* ads_malloc(UINT size) /*allocate dynamic memory in a safe way maintaing a ads_stack of occupied element */
{
	return ads_calloc(1,size);
}

ads_rt ads_gcDump() /*prints the occupied stack */
{
	ads_gcpointer * trav ,*prev ;
	trav = ads_gcobjhead__;
	prev = NULL;
	printf("\n\n\n Allocated [ ");
	while(trav != NULL)
	{
		printf("\n%p, blocks = %d , size = %d ",ads_getGcAllocatedBlock(trav),trav->NumberOfItems,trav->size);
		if(trav->ads == ads_typeTrue)printf(" ObjectType : %s",ads_getObjectTypeName(ads_getGcAllocatedBlock(trav)));
		prev = trav;
		trav = trav->next;
	}
	trav = prev;
	printf(" ]\n\n MaxGC = %I64d ,ActiveAllocated = %I64d ,Totalcalls = %I64d\n",ads_maxGCalocateinlist__,ads_LenGCcalls__,ads_TotalGCcalls__);
	printf(" Current Dynamic Memory = %I64d MiB %I64d KiB %I64d Bytes.\n",ADS_CurrentMemoryActive/1048576,(ADS_CurrentMemoryActive/1024)%1048576,ADS_CurrentMemoryActive%1024);
	printf(" Max Dynamic Memory = %I64d MiB %I64d KiB %I64d Bytes.\n",ADS_GC_MaxMemoryActive/1048576,(ADS_GC_MaxMemoryActive/1024)%1048576,ADS_GC_MaxMemoryActive%1024);
	return ads_SUCCESS;
}

ads_rt ads_fgcDump(FILE*fp) /*prints the occupied stack */
{
	ads_gcpointer * trav ,*prev ;
	trav = ads_gcobjhead__;
	prev = NULL;
	fprintf(fp,"\n\n Allocated [ ");
	while(trav != NULL)
	{
		fprintf(fp,"\n%p, blocks = %d , size = %d ",ads_getGcAllocatedBlock(trav),trav->NumberOfItems,trav->size);
		if(trav->ads == ads_typeTrue)fprintf(fp," ObjectType : %s",ads_getObjectTypeName(ads_getGcAllocatedBlock(trav)));
		prev = trav;
		trav = trav->next;
	}
	trav = prev;
	ads_memCheck(fp);
	return ads_SUCCESS;
}

ads_rt ads_memCheck(FILE*fp)
{
	fprintf(fp,"\n MaxGC = %I64d ,ActiveAllocated = %I64d ,Totalcalls = %I64d\n",ads_maxGCalocateinlist__,ads_LenGCcalls__,ads_TotalGCcalls__);
	fprintf(fp," Current Dynamic Memory = %I64d MiB %I64d KiB %I64d Bytes.\n",ADS_CurrentMemoryActive/1048576,(ADS_CurrentMemoryActive/1024)%1048576,ADS_CurrentMemoryActive%1024);
	fprintf(fp," Max Dynamic Memory = %I64d MiB %I64d KiB %I64d Bytes.\n",ADS_GC_MaxMemoryActive/1048576,(ADS_GC_MaxMemoryActive/1024)%1048576,ADS_GC_MaxMemoryActive%1024);
	return ads_SUCCESS;
}

ads_rt ads_rawFree(void* a) /*frees the memory from occupied stack */
{
    if(a == NULL)
    {
        return ads_registerError("Cannot Free A NULL Pointer.");
    }
	ads_gcpointer* gcblock = ads_getGcBlock(a);
	if(gcblock->ads == ads_typeTrue)
    {
        ads_callDestructor(a);
    }
    else
    {
        ADS_CurrentMemoryActive -= gcblock->NumberOfItems*gcblock->size;
        if(!gcblock->prev)
        {
            ads_gcobjhead__ = gcblock->next;
            if(ads_gcobjhead__) ads_gcobjhead__->prev =NULL;
        }
        else
        {
            gcblock->prev->next = gcblock->next;
            if(gcblock->next) gcblock->next->prev = gcblock->prev;
        }
        free(gcblock);
        ads_LenGCcalls__--;
    }
    return ads_SUCCESS;
}

ads_rt ads_free(void* a) /*frees the memory from occupied stack */
{
    if(a == NULL)
    {
        return ads_registerError("Cannot Free A NULL Pointer.");
    }
	ads_gcpointer* gcblock = ads_getGcBlock(a);
	ADS_CurrentMemoryActive -= gcblock->NumberOfItems*gcblock->size;
    if(!gcblock->prev)
    {
        ads_gcobjhead__ = gcblock->next;
        if(ads_gcobjhead__)ads_gcobjhead__->prev = NULL;
    }
    else
    {
        gcblock->prev->next = gcblock->next;
        if(gcblock->next) gcblock->next->prev = gcblock->prev;
    }
    free(gcblock);
    ads_LenGCcalls__--;
    return ads_SUCCESS;
}

ads_gcpointer* ads_getGcObjHead(void*allocated)
{
	return ads_getGcBlock(allocated);
}

typedef struct ads_Gc_variable {
	void ** address;
	void * value ;
	struct ads_Gc_variable * next;
	short ads;
} ads_Gc_variable ;

ads_Gc_variable * ads_gc_var_head = NULL;

ads_rt ads_setGcVariable(void ** add,short ads)
{
	ads_Gc_variable * p = (ads_Gc_variable*)calloc(1,sizeof(ads_Gc_variable));
	if( ads_checkMemory(p) != ads_SUCCESS ) return ads_registerError("Variable Cannot Be Set to GC Out Of Memory.");
	p->address = add;
	p->value = *add;
	p->next = ads_gc_var_head ;
	p->ads = ads;
	ads_gc_var_head = p;
	return ads_SUCCESS;
}

ads_rt ads_flushVariable()
{
	ads_Gc_variable * trav = ads_gc_var_head,*prev=NULL;
	void**add ;
	while( trav != NULL )
	{
		add = trav->address;
		if( *add != trav->value )
		{
			if( ads_getGcObjHead(trav->value) != NULL )
			{
				if( trav->ads == 1 ) ads_deleteOne(trav->value);
				else ads_free(trav->value);
			}
			if(prev == NULL) ads_gc_var_head = trav->next;
			else prev->next = trav->next;
			free(trav);
		}
		prev = trav;
		trav = trav->next;
	}
	return ads_SUCCESS;
}

#endif
/** @endcond */
