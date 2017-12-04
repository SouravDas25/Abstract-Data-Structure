/** @file deque.h
 * @brief a double-ended queue data structure.
 *
 */


#ifndef ADSDequeue_H
#define ADSDequeue_H

#include "utility.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup deque
 * @{
 * @brief a double-ended queue data structure.
 *
 * @details a double-ended queue
 * is an abstract data type that generalizes a queue,
 * for which elements can be added to or removed from either the front (head) or back (tail).
 * It is also often called a head-tail linked list, though properly this refers to a specific data structure implementation of a deque.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%deq(#) - # is a non-empty character or string specifying the format specifier of that type contained by the deque
 *      - @b {deq(#)} - prints the deque to the stdout.
 */


/**
 * @struct deque deque.h <ads/deque.h>
 * @brief a double-ended queue data structure.
 *
 * @details a double-ended queue
 * is an abstract data type that generalizes a queue,
 * for which elements can be added to or removed from either the front (head) or back (tail).
 * It is also often called a head-tail linked list, though properly this refers to a specific data structure implementation of a deque.
 */
ads_classProto(ads_deque);


extern const ads_td * const ads_dequeTD;

/** @brief ads_deque constructor.
 *
 * @param type the data-type or domain of the containing data
 * @return a ads_deque instance
 *
 */
#define ads_dequeNew(type) ads_initDeque(sizeof(type))

/** @cond ADS_INTERNAL */
ads_deque ads_initDeque(int datatype);
/** @endcond */

/** @brief set type-definition of the container type in the queue.
 *
 * @param self ads_deque
 * @param td const ads_td*
 * @return void
 *
 */
void ads_dequeSetTD(ads_deque self,const ads_td * td);

/** @brief insert a value at the end of the ads_deque
 *
 * @param self a ads_deque instance
 * @param value the value to be inserted
 * @param type the container type of the ads_deque
 * @return void
 *
 */
#define ads_dequePush(self,value,type)  ADS_APPEND(ads_dequePush_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
void ads_dequePush_ByRef(ads_deque self,void*value);
/** @endcond */

/** @brief insert a element in front of the ads_deque
 *
 * @param self a ads_deque instance
 * @param value the value to be inserted
 * @param type the container type of the ads_deque
 * @return void
 *
 */
#define ads_dequePushFront(self,value,type)  ADS_APPEND(ads_dequePushFront_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
void ads_dequePushFront_ByRef(ads_deque self,void*value);
/** @endcond */

/** @brief removes a element from the head of the ads_deque and returns it.
 *
 * @param self a ads_deque instance
 * @param type the container type of the ads_deque
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define  ads_dequePopFront(self,type,result) ADS_POP(ads_dequePopFront_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_dequePopFront_RtnRef(ads_deque self);
/** @endcond */

/** @brief removes a element from the tail of the ads_deque and returns it.
 *
 * @param self a ads_deque instance
 * @param type the container type of the ads_deque
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_dequePop(self,type,result) ADS_POP(ads_dequePop_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_dequePop_RtnRef(ads_deque self);
/** @endcond */

/** @brief copies the front element of the ads_deque into the result variable.
 *
 * @param self a ads_deque instance
 * @param type the container type of the ads_deque
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_dequeFront_Copy(self,type,result) ADS_POP(ads_dequeFront_Copy_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_dequeFront_Copy_RtnRef(ads_deque self);
/** @endcond */

/** @brief copies the last element of the ads_deque into the result variable.
 *
 * @param self a ads_deque instance
 * @param type the container type of the ads_deque
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_dequeEnd_Copy(self,type,result) ADS_POP(ads_dequeEnd_Copy_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_dequeEnd_Copy_RtnRef(ads_deque self);
/** @endcond */

/** @brief verifies weather the ads_deque is empty or not.
 *
 * @param self a ads_deque instance
 * @return boolean value
 *
 */
bool ads_dequeIsEmpty(ads_deque self);

/** @brief returns no of elements in the ads_deque.
 *
 * @param self a ads_deque instance
 * @return integer value
 *
 */
int ads_dequeLen(ads_deque self);

/** @brief iterator for foreach loop.
 *
 * @param self a ads_deque instance
 * @return ads_deque iterator
 *
 */
ads_iterator ads_dequeIterator(ads_deque self);

/** @cond ADS_INTERNAL */
void ads_printDeque(void*add,const char * cmd,int size);
/** @endcond */

/** @brief copy constructor.
 *
 * @param self a ads_deque instance
 * @return ads_deque instance
 *
 */
ads_deque ads_dequeCopy(ads_deque _self);

/** @brief destructor.
 *
 * @param self a ads_deque instance
 * @return void
 *
 */
void ads_dequeDelete(ads_deque _self);

/** @cond ADS_INTERNAL */
void ads_dequeBuilt_FrmArray(ads_deque self,void*arr,int n);
/** @endcond */

/** @brief initialize the ads_deque with variables.
 *
 * @param self a ads_deque instance
 * @param type the container type of the ads_deque
 * @param ... the variables to initialize the ads_deque with.
 * @return void
 *
 */
#define ads_dequeBuilt(self,type,...) {type aaa[] = {__VA_ARGS__}; ads_dequeBuilt_FrmArray(self,aaa,sizeof(aaa)/sizeof(type));}

/** @} */

#ifdef __cplusplus
}
#endif

#endif
