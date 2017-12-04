
/** @file queue.h
 * @brief a queue data structure.
 *
 */


#ifndef ADSqueue_H
#define ADSqueue_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup queue
 * @{
 * @brief a queue is a particular kind of abstract data type or collection in which the entities in the collection are kept in order.
 *
 * @details a queue is a collection in which the entities in the collection are kept in order and
 * the principal (or only) operations on the collection are the addition of entities to the rear terminal position, known as enqueue,
 * and removal of entities from the front terminal position, known as dequeue. This makes the queue a First-In-First-Out (FIFO) data structure.
 * In a FIFO data structure, the first element added to the queue will be the first one to be removed.
 * This is equivalent to the requirement that once a new element is added,
 * all elements that were added before have to be removed before the new element can be removed.
 * Often a peek or front operation is also entered, returning the value of the front element without dequeuing it.
 * A queue is an example of a linear data structure, or more abstractly a sequential collection.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%que(#) - # is a non-empty character or string specifying the format specifier of that type contained by the queue
 *      - @b {que(#)} - prints the queue to the stdout.
 */

/** @cond ADS_INTERNAL */
typedef struct ads_queueContainer ads_queueContainer;
/** @endcond */

/**
 * @struct queue queue.h <ads/queue.h>
 * @brief a queue is a particular kind of abstract data type or collection in which the entities in the collection are kept in order.
 *
 * @details a queue is a collection in which the entities in the collection are kept in order and
 * the principal (or only) operations on the collection are the addition of entities to the rear terminal position, known as enqueue,
 * and removal of entities from the front terminal position, known as dequeue. This makes the queue a First-In-First-Out (FIFO) data structure.
 * In a FIFO data structure, the first element added to the queue will be the first one to be removed.
 * This is equivalent to the requirement that once a new element is added,
 * all elements that were added before have to be removed before the new element can be removed.
 * Often a peek or front operation is also entered, returning the value of the front element without dequeuing it.
 * A queue is an example of a linear data structure, or more abstractly a sequential collection.
 */
ads_classProto(ads_queue);

extern const ads_td * const ads_queueTD;

/** @brief ads_queue constructor.
 *
 * @param type the data-type or domain of the containing data
 * @return a ads_queue instance
 *
 */
#define ads_queueNew(type) ads_initQueue(sizeof(type))

/** @cond ADS_INTERNAL */
ads_queue ads_initQueue(int datatype);
void ads_queueSetTD(ads_queue self,const ads_td * td);
/** @endcond */

/** @brief insert a value at the end of the ads_queue
 *
 * @param self a ads_queue instance
 * @param value the value to be inserted
 * @param type the container type of the ads_queue
 * @return void
 *
 */
#define ads_queueInsert(self,value,type) ADS_APPEND(ads_queueInsert_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
void ads_queueInsert_ByRef(ads_queue self,void * value);
/** @endcond */

/** @brief removes a element from the head of the ads_queue and returns it.
 *
 * @param self a ads_queue instance
 * @param type the container type of the ads_queue
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_queuePop(self,type,result) ADS_POP(ads_queuePop_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_queuePop_RtnRef(ads_queue self);
/** @endcond */

/** @brief copies the front element of the ads_queue into the result variable.
 *
 * @param self a ads_queue instance
 * @param type the container type of the ads_queue
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_queueFront_Copy(self,type,result) ADS_POP(ads_queueFront_Copy_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_queueFront_Copy_RtnRef(ads_queue self);
/** @endcond */

/** @brief verifies weather the ads_queue is empty or not.
 *
 * @param self a ads_queue instance
 * @return boolean value
 *
 */
bool ads_queueIsEmpty(ads_queue self);

/** @brief returns no of elements in the ads_queue.
 *
 * @param self a ads_queue instance
 * @return integer value
 *
 */
int ads_queueLen(ads_queue self);

/** @brief iterator for foreach loop.
 *
 * @param self a ads_queue instance
 * @return ads_queue iterator
 *
 */
ads_iterator ads_queueIterator(ads_queue self);

/** @cond ADS_INTERNAL */
void ads_printQueue(void*add,const char * cmd,int size);
/** @endcond */

/** @brief copy constructor.
 *
 * @param self a ads_queue instance
 * @return ads_queue instance
 *
 */
ads_queue ads_queueCopy(ads_queue _self);

/** @brief destructor.
 *
 * @param self a ads_queue instance
 * @return void
 *
 */
void ads_queueDelete(ads_queue _self);

/** @brief initialize the ads_queue with variables.
 *
 * @param self a ads_queue instance
 * @param type the container type of the ads_queue
 * @param ... the variables to initialize the ads_queue with.
 * @return void
 *
 */
#define ads_queueBuilt(self,type,...) {type aDSqueueaaa[] = {__VA_ARGS__};ads_queueBuilt_FrmArray(self,aDSqueueaaa,sizeof(aDSqueueaaa)/sizeof(type));}

/** @cond ADS_INTERNAL */
void ads_queueBuilt_FrmArray(ads_queue self,void*arr,int n);
/** @endcond */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
