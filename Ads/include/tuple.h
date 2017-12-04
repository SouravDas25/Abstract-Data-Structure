/** @file tuple.h
 * @brief a tuple data structure.
 *
 * a fixed collection of different types of elements.
 */


#ifndef ADSTUPLE_H
#define ADSTUPLE_H

#include "utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup tuple
 * @{
 * @brief An array of different elements.
 *
 * @details A tuple is an array of different elements
 * stored and accessed index-wise.
 *
 * <h4>Format Specifier</h4>
 *      - @b {tup(...)} - ... is an eclipse which can be any no of format specifier separated by comma that is contained in the tuple.
 *      - @b {(...)} - ... is an eclipse which can be any no of format specifier separated by comma that is contained in the tuple.
 *
 * <h4>Example</h4>
 * let's create a new tuple which contains 4 different elements.
 * @code {.c}
 * ads_tuple t = ads_tupleNew(4);  //creating a new tuple.
 * @endcode
 * the variable t is of type tuple and has a free space for holding any 4 different type of object.
 * Now lets insert some values.
 * @code {.c}
 * ads_tupleAppend(t,56,int);  //inserting an integer value into the tuple.
 * ads_tupleAppend(t,985.356,double); //inserting a double value into the tuple.
 * ads_tupleAppendTD(t,ads_cstrTD,"Cstr-TD",const char *); //inserting values using type-definition
 * ads_tupleAppendCStr(t,"Direct Cstr"); //simplified function to insert c-string.
 * @endcode
 *
 * Now let's fetch some values from the tuple.
 * @code {.c}
 * int j;
 * ads_tupleGetItem_NoCopy(t,0,int,k); //getting values from tuple.
 * @endcode
 * the ads_tupleGetItem_NoCopy() function fetches the value at index 0 and copies it to the j variable.
 *
 * we can also set values at particular index of the tuple.
 * @code {.c}
 * ads_tupleSetItem(t,1,32,int);
 * @endcode
 *
 * printing a tuple.
 * @code {.c}
 * ads_printf("tuple = {tup(d,lf,'s','s')}",t); // tuple = (56,985.356,'Cstr-TD','Direct Cstr')
 * @endcode
 *
 * all tuple variable must be deleted through ads_delete() or ads_deleteOne() explicitly.
 * @code {.c}
 * ads_delete(t); //deleting the tuple.
 * @endcode
 *
 */

/**
 * @struct  ads_tuple tuple.h <ads/tuple.h>
 * @brief An array of different elements.
 *
 * @details A tuple is an array of different elements
 * stored and accessed index-wise.
 *
 */
ads_classProto(ads_tuple);

/** @brief a tuple type definition , used to create tuple of tuple and many more.
 *
 */
extern const ads_td * const ads_tupleTD;


/**
 * @brief A macro Constructor of the tuple class.
 * @details Call this constructor to create a new instance of the class.
 * This constructor is called by the ads_new() to create a tuple.
 *
 * @param size the initial size of the tuple.
 * @return a tuple instance with size(size).
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_tuple t = ads_tupleNew(7);
 * @endcode
 */
#define ads_tupleNew ads_initTuple

/** @brief Constructor of the tuple class.
 *
 * @param size int the size of the tuple
 * @return ads_tuple
 *
 */
ads_tuple ads_initTuple(int size);

/**
 * @brief returns no of elements in the tuple.
 *
 * @param self takes a tuple instance.
 * @return No of item in the tuple.
 *
 */
int ads_tupleLen(ads_tuple self);

/**
 * @brief Insert a element at the end of the tuple.
 *
 * @param self takes a tuple instance
 * @param value any variable or constant
 * @param type type of the value variable or constant.
 *
 */
#define ads_tupleAppend(self,value,type) {type __ADS_TMPVAL__ = (value); ads_tupleAppend_ByRef(self,(void**)&__ADS_TMPVAL__,sizeof(type));}

/**< @cond ADS_INTERNAL */
ads_rt ads_tupleAppend_ByRef(ads_tuple self,void * value,int datatype);
/**< @endcond */


/** @brief return individual item from the tuple with out making a copy of it.
 *
 * @param self the tuple instance
 * @param index the item index
 * @param type the type of item in the index
 * @param result the where the value will be copied.
 *
 */
#define ads_tupleGetItem_Copy(self,index,type,result) ADS_GETITEM_FREE(ads_tupleGetItem_Copy_RtnRef,self,index,type,result)

/**< @cond ADS_INTERNAL */
void* ads_tupleGetItem_Copy_RtnRef(ads_tuple self,int index);
/**< @endcond */


/** @brief return a copy of the item present at the index
 *
 * @param self the tuple instance
 * @param index the item index
 * @param type the type of item in the index
 * @param result the where the value will be copied.
 *
 */
#define ads_tupleGetItem_NoCopy(self,index,type,result) ADS_GETITEM(ads_tupleGetItem_NoCopy_RtnRef,self,index,type,result)

/**< @cond ADS_INTERNAL */
void* ads_tupleGetItem_NoCopy_RtnRef(ads_tuple self,int index);
/**< @endcond */

/** @brief replace the item at the current index.
 *
 * @param self the tuple instance
 * @param index the item index
 * @param value the value to insert
 * @param type the type of item in the index
 *
 */
#define ads_tupleSetItem(self,index,value,type) {type __ADS_TMPVAL__ = (value); ads_tupleSetItem_ByRef(self,index,(void**)&__ADS_TMPVAL__,sizeof(type));}

/**< @cond ADS_INTERNAL */
ads_rt ads_tupleSetItem_ByRef(ads_tuple self,int index,void*value,int datatype);
/**< @endcond */

/**
 * @brief Insert a element at the end of the tuple.
 *
 * @param self takes a tuple instance
 * @param td the type-definition of the type
 * @param value any variable or constant
 * @param type type of the value variable or constant.
 *
 */
#define ads_tupleAppendTD(self,td,value,type) {type __ADS_TMPVAL__ = (value); ads_tupleAppendTD_ByRef(self,td,(void**)&__ADS_TMPVAL__,sizeof(type));}
/**< @cond ADS_INTERNAL */
ads_rt ads_tupleAppendTD_ByRef(ads_tuple self,const ads_td * td,void * value,int datatype);
/**< @endcond */


/** @brief replace the item at the current index.
 *
 * @param self the tuple instance
 * @param index the item index
 * @param td the type-definition of the type
 * @param value the value to insert
 * @param type the type of item in the index
 *
 */
#define ads_tupleSetItemTD(self,index,td,value,type) {type __ADS_TMPVAL__ = (value); ads_tupleSetItemTD_ByRef(self,index,td,(void**)&__ADS_TMPVAL__,sizeof(type));}
/**< @cond ADS_INTERNAL */
ads_rt ads_tupleSetItemTD_ByRef(ads_tuple self,int index,const ads_td * td,void*value,int datatype);
/**< @endcond */


/** @brief Insert a C-String at the end of the tuple.
 *
 * @param self ads_tuple the tuple instance
 * @param str const char* the c-string
 * @return void
 *
 */
void ads_tupleAppendCStr(ads_tuple self,const char * str);

/** @brief replace the item at the current index with a C-String item.
 *
 * @param self ads_tuple the tuple instance
 * @param index int tuple index
 * @param str const char* the C-String
 * @return void
 *
 */
void ads_tupleSetItemCStr(ads_tuple self,int index,const char * str);

/**< @cond ADS_INTERNAL */
void ads_printTuple(void*add,const char * cmd,int size);
/**< @endcond */


/** @brief Creates a new tuple by merging two tuples into one.
 *
 * @param t1 ads_tuple
 * @param t2 ads_tuple
 * @return ads_tuple
 *
 */
ads_tuple ads_tupleConcat(const ads_tuple t1,const ads_tuple t2);

/** @brief makes a deep copy of the tuple
 *
 * @param _self ads_tuple the tuple to be copied.
 * @return ads_tuple
 *
 */
ads_tuple ads_tupleCopy(const ads_tuple _self);

/** @brief deletes a tuple instance.
 *
 * @param _self ads_tuple
 * @return void
 *
 */
void ads_tupleDelete(const ads_tuple _self);

#ifdef __cplusplus
}
#endif


#endif
