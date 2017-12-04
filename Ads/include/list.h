
/** @file list.h
 * @brief a XOR linked list.
 *
 */

#ifndef ADSLinklist_H
#define ADSLinklist_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup list
 * @{
 * @brief a XOR linked list is a linear collection of data elements, called nodes, each pointing to the next node by means of a pointer.
 *
 * @details It is a data structure consisting of a group of nodes which together represent a sequence.
 * Under the simplest form, each node is composed of data and a reference (in other words, a link) to the next node in the sequence.
 * This structure allows for efficient insertion or removal of elements from any position in the sequence during iteration.
 * More complex variants add additional links, allowing efficient insertion or removal from arbitrary element references.
 * The principal benefit of a linked list over a conventional array is that the list elements can easily be
 * inserted or removed without reallocation or reorganization of the entire structure because
 * the data items need not be stored contiguously in memory or on disk,
 * while an array has to be declared in the source code, before compiling and running the program.
 * Linked lists allow insertion and removal of nodes at any point in the list,
 * and can do so with a constant number of operations if the link previous to the link being added or removed is maintained during list traversal.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%list(#) - # is a non-empty character or string specifying the format specifier of that type contained by the list
 *      - @b {list(#)} - prints the list to the stdout.
 *
 *
 * <h4>Example</h4>
 * list is a linked list let's create One.
 * @code {.c}
 * ads_list l = ads_new(ads_list,int);  //creating a new list.
 * @endcode
 *
 * insert some values.
 * @code {.c}
 * ads_listAppend(l,25,int);  //inserting values to the vector.
 * @endcode
 *
 * insert some multiple values.
 * @code {.c}
 * int array[] = {0,1,2,3,4,5};
 * ads_listBuilt_FrmArray(l,array,6);
 * ads_listBuilt(l,int,1,2,3,4,5,6,7,8,9,10,11,12); //only available in c99 and above.
 * @endcode
 *
 * traversing over list.
 * @code {.c}
 * ads_foreach(k , ads_listIterator(l) ,int ) //iterate over them , it takes O(1) time complexity
 * {
 *	 ads_printf("%d,",k); //print each value
 *	 ads_listSetItem(l,ads_enumerate(k),k*2,int); //double each value
 * }ads_next(k);
 * @endcode
 *
 * fetching values from a list.
 * @code {.c}
 * int j;
 * ads_listGetItem_NoCopy(l,0,int,j); //getting values from list.
 * @endcode
 *
 * delete a list.
 * @code {.c}
 * ads_delete(l); //deleting the list.
 * @endcode
 */




/** @cond ADS_INTERNAL */

typedef struct ads_listContainer ads_listContainer;
ads_listContainer* ads_listXor(ads_listContainer*arg1,ads_listContainer*arg2);
ads_listContainer *ads_listContainerNext(ads_listContainer * container);
void* ads_listContainerValue(ads_listContainer * container);

/** @endcond */



/**
 * @struct ads_list list.h <ads/list.h>
 * @brief a XOR linked list is a linear collection of data elements, called nodes, each pointing to the next node by means of a pointer.
 *
 * @details It is a data structure consisting of a group of nodes which together represent a sequence.
 * Under the simplest form, each node is composed of data and a reference (in other words, a link) to the next node in the sequence.
 * This structure allows for efficient insertion or removal of elements from any position in the sequence during iteration.
 * More complex variants add additional links, allowing efficient insertion or removal from arbitrary element references.
 * The principal benefit of a linked list over a conventional array is that the list elements can easily be
 * inserted or removed without reallocation or reorganization of the entire structure because
 * the data items need not be stored contiguously in memory or on disk,
 * while an array has to be declared in the source code, before compiling and running the program.
 * Linked lists allow insertion and removal of nodes at any point in the list,
 * and can do so with a constant number of operations if the link previous to the link being added or removed is maintained during list traversal.
 */
ads_classProto(ads_list);

extern const ads_td * const ads_listTD;

/** @brief ads_list constructor.
 *
 * @param type the data-type or domain of the containing data
 * @return a ads_list instance or NULL on Error check ads_lastErrorMsg()
 *
 */
#define ads_listNew(type) ads_initList(sizeof(type))

/** @brief set the type-definition of the type container by the ads_list
 *
 * @param self ads_list
 * @param td const ads_td*
 * @return ads_rt returns ads_SUCCESS if No Error Is Cause else returns ads_ERROR
 *
 */
ads_rt ads_listSetTD(ads_list self,const ads_td* td);

/** @cond ADS_INTERNAL */
ads_list ads_initList(size_t datatype);
ads_listContainer *ads_linklistHead(ads_list self);
ads_td* ads_listGetTD(ads_list self);

ads_rt ads_listAppend_NoCopy(ads_list self,void * value);
/** @endcond */

/** @brief insert a value at the end of the ads_list
 *
 * @param self a ads_list instance
 * @param value the value to be inserted
 = @param type the container type of the ads_list
 * @return void
 *
 */
#define ads_listAppend(self,value,type)  ADS_APPEND(ads_listAppend_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
ads_rt ads_listAppend_ByRef(ads_list self,void * value);
/** @endcond */

/** @brief replace a value at the index of the ads_list
 *
 * @param self a ads_list instance
 * @param index the position of the value to be replaced
 * @param value the value to be replaced with
 = @param type the container type of the ads_list
 * @return void
 *
 */
#define ads_listSetItem(self,index,value,type) ADS_SETITEM(ads_listSetItem_ByRef,self,index,value,type)

/** @cond ADS_INTERNAL */
ads_rt ads_listSetItem_ByRef(ads_list self,size_t index,void*value);
/** @endcond */

/** @brief insert a value before the index in the ads_list
 *
 * @param self a ads_list instance
 * @param index the position of the insertion
 * @param value the value to be inserted
 = @param type the container type of the ads_list
 * @return void
 *
 */
#define ads_listInsert(self,index,value,type) ADS_SETITEM(ads_listInsert_ByRef,self,index,value,type)

/** @cond ADS_INTERNAL */
ads_rt ads_listInsert_ByRef(ads_list self,size_t index,void*value);
/** @endcond */

/** @brief returns no of elements in the ads_list.
 *
 * @param self a ads_list instance
 * @return integer value
 *
 */
uint32_t ads_listLen(ads_list self);

/** @brief the amount of memory occupied by the ads_list.
 *
 * @param self a ads_list instance
 * @return memory in bytes
 *
 */
size_t ads_listSizeOf(ads_list self);

/** @brief return the item at index to the variable result.
 *
 * @param self takes a ads_list instance
 * @param index is the int index position just like the array starting from 0.
 * @param type of the data stored in the ads_list.
 * @return the item of type param type
 *
 */
#define ads_listGetItem_NoCopy(self,index,type,result) ADS_GETITEM(ads_listGetItem_NoCopy_RtnRef,self,index,type,result)

#define ads_listGetItem_Copy(self,index,type,result) ADS_GETITEM_FREE(ads_listGetItem_Copy_RtnRef,self,index,type,result)

/** @cond ADS_INTERNAL */
void* ads_listGetItem_Copy_RtnRef(ads_list  self,size_t index);
void* ads_listGetItem_NoCopy_RtnRef(ads_list  self,size_t index);
/** @endcond */

/**
 * @brief delete the element at index.
 *
 * @param self takes a ads_list instance
 * @param index is the int index position just like the array starting from 0.
 *
 */
ads_rt ads_listDelItem(ads_list  self,size_t index);

/** @brief verifies weather the ads_list is empty or not.
 *
 * @param self a ads_list instance
 * @return boolean value
 *
 */
bool ads_listEmpty(ads_list self);

/** @brief removes a element from the head of the ads_list and returns it.
 *
 * @param self a ads_list instance
 * @param type the container type of the ads_list
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_listPop(self,type,result) ADS_POP(ads_listPop_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_listPop_RtnRef(ads_list self);
/** @endcond */

/** @brief removes a element from the index of the ads_list and returns it.
 *
 * @param self a ads_list instance
 * @param index index is the int index position just like the array starting from 0.
 * @param type the container type of the ads_list
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_listPopItem(self,index,type,result) ADS_GETITEM_FREE(ads_listPopItem_RtnRef,self,index,type,result)

/** @cond ADS_INTERNAL */
void* ads_listPopItem_RtnRef(ads_list self,size_t index);
/** @endcond */

/**
 * @brief insert all elements from Other to self
 *
 * @param self takes a ads_list instance
 * @param other takes a ads_list instance
 *
 */
ads_rt ads_listExtend(ads_list self,ads_list other);

/**
 * @brief search the ads_list for the value and returns the index.
 *
 * @param self takes a ads_list instance
 * @param func takes a compare function.
 * @param value any variable or constant
 * @param type of the data stored in the ads_list.
 * @param result a variable where the index will be returned.
 * @return  void
 */
#define ads_listIndex(self,func,value,type,result) ADS_SENDFUNC_RETURN(ads_listIndex_ByRef,self,func,value,type,result)

/** @cond ADS_INTERNAL */
int ads_listIndex_ByRef(ads_list self,void * value,int (*comparefunction)(const void*,const void*));
/** @endcond */

/**
 * @brief removes all occurrence of the value from the ads_list.
 *
 * @param self takes a ads_list instance
 * @param function takes a compare function.
 * @param value any variable or constant
 * @param type of the data stored in the ads_list.
 * @return void
 */
#define ads_listRemove(self,func,value,type) ADS_SENDFUNC(ads_listRemove_ByRef,self,func,value,type)

/** @cond ADS_INTERNAL */
void ads_listRemove_ByRef(ads_list self,void * value,int (*comparefunction)(const void*,const void*));
/** @endcond */

/**
 * @brief reverse the ads_list.
 *
 * @param self takes a ads_list instance
 * @return void
 */
ads_rt ads_listReverse(ads_list self);

/**
 * @brief a ads_list iterator.
 *
 * @param self takes a ads_list instance
 * @return ads_list iterator
 */
ads_iterator ads_listIterator(ads_list self);

/** @cond ADS_INTERNAL */
void ads_printList(void*add,const char * cmd,int size);
/** @endcond */

/**
 * @brief return a  copy of the ads_list .
 * @details all elements are also copied recursively.
 *
 * @param _self takes a ads_list instance.
 * @return returns a ads_list instance.
 *
 */
ads_list ads_listCopy(ads_list _self);

/**
 * @brief remove all elements from the ads_list.
 *
 * @param self takes a ads_list instance.
 * @return void
 */
ads_rt ads_listClear(ads_list self);

/**
 * @brief ads_list destructor.
 * @details destroys a ads_list but dose not delete any elements.
 * @param _self takes a ads_list instance.
 * @return void
 *
 */
ads_rt ads_deleteList_NoDelete(ads_list _self);

/**
 * @brief ads_list destructor.
 * @details destroys a ads_list and deallocate all elements.
 * @param _self takes a ads_list instance.
 * @return void
 *
 */
void ads_listDelete(ads_list _self);

/**
 * @brief initialize a ads_list with the values.
 *
 * @param self takes a ads_list instance
 * @param type of the data stored in the ads_list.
 * @param ... n no of value to be appended into the ads_list.
 */
#define ads_listBuilt(self,type,...) {type aaa[] = {__VA_ARGS__};ads_listBuilt_FrmArray(self,aaa,sizeof(aaa)/sizeof(type));}

/** @brief initialize a list with array.
 *
 * @param self ads_list the list to initialize.
 * @param void*arr the starting address of the array
 * @param n int the size of the array.
 * @return void
 *
 */
ads_rt ads_listBuilt_FrmArray(ads_list self,void*arr,int n);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
