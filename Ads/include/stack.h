
/** @file stack.h
 * @brief a stack data structure.
 *
 */



#ifndef ADS_Stck_H
#define ADS_Stck_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup stack
 * @{
 * @brief a stack is a particular kind of abstract data type or collection in which the oder of removal is reverse of the order of insertion.
 *
 * @details  a stack is an abstract data type that serves as a collection of elements, with two principal operations:
 * push, which adds an element to the collection, and
 * pop, which removes the most recently added element that was not yet removed.
 * The order in which elements come off a stack gives rise to its alternative name, LIFO (for last in, first out).
 * Additionally, a peek operation may give access to the top without modifying the stack.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%stk(#) - # is a non-empty character or string specifying the format specifier of that type contained by the stack
 *      - @b {stk(#)} - prints the stack to the stdout.
 */

/** @cond ADS_INTERNAL */
typedef struct ads_stackContainer ads_stackContainer;
/** @endcond */

/**
 * @struct stack stack.h <ads/stack.h>
 * @brief a stack is a particular kind of abstract data type or collection in which the oder of removal is reverse of the order of insertion.
 *
 * @details  a stack is an abstract data type that serves as a collection of elements, with two principal operations:
 * push, which adds an element to the collection, and
 * pop, which removes the most recently added element that was not yet removed.
 * The order in which elements come off a stack gives rise to its alternative name, LIFO (for last in, first out).
 * Additionally, a peek operation may give access to the top without modifying the stack.
 */
ads_classProto(ads_stack);

extern const ads_td * const ads_stackTD;

/** @brief ads_stack constructor.
 *
 * @param type the data-type or domain of the containing data
 * @return a ads_stack instance
 *
 */
#define ads_stackNew(type) ads_initStack(sizeof(type))

/** @cond ADS_INTERNAL */
ads_stack ads_initStack(int datatype );
void ads_stackSetTD(ads_stack self,const ads_td * td);
/** @endcond */

/** @brief insert a value at the top of the ads_stack
 *
 * @param self a ads_stack instance
 * @param value the value to be inserted
 = @param type the container type of the ads_stack
 * @return void
 *
 */
#define ads_stackPush(self,value,type) ADS_APPEND(ads_stackPush_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
void ads_stackPush_ByRef(ads_stack self,void * value);
/** @endcond */

/** @brief removes a element from the top of the ads_stack and returns it.
 *
 * @param self a ads_stack instance
 * @param type the container type of the ads_stack
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_stackPop(self,type,result) ADS_POP(ads_stackPop_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_stackPop_RtnRef(ads_stack self);
/** @endcond */

/** @brief copies the top element of the ads_stack into the result variable.
 *
 * @param self a ads_stack instance
 * @param type the container type of the ads_stack
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_stackFront_Copy(self,type,result) ADS_POP(ads_stackFront_Copy_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_stackFront_Copy_RtnRef(ads_stack self);
/** @endcond */

/** @brief returns no of elements in the ads_stack.
 *
 * @param self a ads_stack instance
 * @return integer value
 *
 */
int ads_stackLen(ads_stack  self);

/** @cond ADS_INTERNAL */
void ads_printStack(void*add,const char * cmd,int size);
/** @endcond */

/** @brief verifies weather the ads_stack is empty or not.
 *
 * @param self a ads_stack instance
 * @return boolean value
 *
 */
bool ads_stackIsEmpty(ads_stack self);

/** @brief reverses the given ads_stack.
 *
 * @param self a ads_stack instance
 * @return void
 *
 */
void ads_stackReverse(ads_stack self);

/** @brief iterator for foreach loop.
 *
 * @param self a ads_stack instance
 * @return ads_stack iterator
 *
 */
ads_iterator ads_stackIterator(ads_stack self);

/** @brief copy constructor.
 *
 * @param self a ads_stack instance
 * @return ads_stack instance
 *
 */
ads_stack ads_stackCopy(ads_stack _self);

/** @brief destructor.
 *
 * @param self a ads_stack instance
 * @return void
 *
 */
void ads_stackDelete(ads_stack _self);

/** @brief initialize the ads_stack with variables.
 *
 * @param self a ads_stack instance
 * @param type the container type of the ads_stack
 * @param ... the variables to initialize the ads_stack with.
 * @return void
 *
 */
#define ads_stackBuilt(self,type,...) {type aaa[] = {__VA_ARGS__};ads_stackBuilt_FrmArray(self,aaa,sizeof(aaa)/sizeof(type));}

/** @cond ADS_INTERNAL */
void ads_stackBuilt_FrmArray(ads_stack self,void*arr,int n);
/** @endcond */

/** @} */

#ifdef __cplusplus
}
#endif

#endif


