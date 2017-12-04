
/** @file heap.h
 * @brief a heap is a specialized tree-based data structure that satisfies the heap property.
 *
 * The heap is one maximally efficient implementation of an abstract data type called a priority queue,
 * and in fact priority queues are often referred to as "heaps".
 */




#ifndef ADSHEAPTREE_H
#define ADSHEAPTREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utility.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup heap
 * @{
 * @brief a heap is a specialized tree-based data structure that satisfies the heap property.
 *
 * @details If A is a parent node of B,
 * then the key (the value) of node A is ordered with respect to the key of node B with the same ordering applying across the heap.
 * A heap can be classified further as either a "max heap" or a "min heap". In a max heap,
 * the keys of parent nodes are always greater than or equal to those of the children and the highest key is in the root node.
 * In a min heap, the keys of parent nodes are less than or equal to those of the children and the lowest key is in the root node.
 * The heap is one maximally efficient implementation of an abstract data type called a priority queue,
 * and in fact priority queues are often referred to as "heaps".
 */

/**
 * @struct heap heap.h <ads/heap.h>
 * @brief a heap is a specialized tree-based data structure that satisfies the heap property.
 *
 * @details If A is a parent node of B,
 * then the key (the value) of node A is ordered with respect to the key of node B with the same ordering applying across the heap.
 * A heap can be classified further as either a "max heap" or a "min heap". In a max heap,
 * the keys of parent nodes are always greater than or equal to those of the children and the highest key is in the root node.
 * In a min heap, the keys of parent nodes are less than or equal to those of the children and the lowest key is in the root node.
 * The heap is one maximally efficient implementation of an abstract data type called a priority queue,
 * and in fact priority queues are often referred to as "heaps".
 */
ads_classProto(ads_heap);

extern const ads_td * const ads_heapTD;

#define ads_minHeap -1
#define ads_maxHeap 1

/** @brief heap constructor
 *
 * @param datatype the data-type or domain of data values
 * @param cmpfunction the compare function for data values
 * @param heaptype specify weather the heap is MAX or MIN ads_heap
 * @return a ads_heap instance
 *
 */
#define ads_heapNew(datatype,cmpfunction,heaptype) ads_initHeap(sizeof(datatype),cmpfunction,heaptype)

/** @cond ADS_INTERNAL */
ads_heap ads_initHeap(int datatype, int (*compare)(const void * data1,const void*data2),int heaptype);
void ads_heapSetTD(ads_heap self,const ads_td * td);
/** @endcond */

/** @brief insert a value into the ads_heap
 *
 * @param self a ads_heap instance
 * @param value the value to be inserted
 = @param type the container type of the ads_heap
 * @return void
 *
 */
#define ads_heapInsert(self,value,type) ADS_APPEND(ads_heapInsert_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
ads_rt ads_heapInsert_ByRef(ads_heap self,void* value);
/** @endcond */

/** @brief removes a element from the ads_heap and returns it.
 *
 * @param self a ads_heap instance
 * @param type the container type of the ads_heap
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_heapPop(self,type,result)  ADS_POP(ads_heapPop_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_heapPop_RtnRef(ads_heap self);
/** @endcond */

/** @brief copy constructor.
 *
 * @param self a ads_heap instance
 * @return ads_heap instance
 *
 */
ads_heap ads_heapCopy(ads_heap _self);

/** @cond ADS_INTERNAL */
void ads_printHeap(ads_heap self,void (*printfunction)(const void*));
/** @endcond */

/** @brief destructor.
 *
 * @param self a ads_heap instance
 * @return void
 *
 */
void ads_heapDelete(ads_heap _self);

/** @brief return the amount of memory occupied by the ads_heap.
 *
 * @param self a ads_heap instance
 * @return memory in bytes
 *
 */
int ads_heapSizeOf(ads_heap self);

/** @brief return no of element in ads_heap.
 *
 * @param self a ads_heap instance
 * @return no of element
 *
 */
int ads_heapLen(ads_heap self);

/** @brief verifies weather the ads_heap is empty or not.
 *
 * @param self a ads_heap instance
 * @return boolean value
 *
 */
bool ads_heapIsEmpty(ads_heap self);

/** @brief return a ads_list of sorted values.
 *
 * @param self a ads_heap instance
 * @return ads_list
 *
 */
ads_list ads_heapToList(ads_heap self);

/** @brief initialize the ads_heap with variables.
 *
 * @param self a ads_heap instance
 * @param type the container type of the ads_heap
 * @param ... the variables to initialize the ads_heap with.
 * @return void
 *
 */
#define ads_heapBuilt(self,type,...) {type aaa[] = {__VA_ARGS__};ads_heapBuilt_FrmArray(self,aaa,sizeof(aaa)/sizeof(type));}

/** @cond ADS_INTERNAL */
void ads_heapBuilt_FrmArray(ads_heap self,void*arr,int n);
/** @endcond */

/** @} */

#ifdef __cplusplus
}
#endif

#endif




