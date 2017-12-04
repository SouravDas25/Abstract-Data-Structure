/** @file set.h
 * @brief a set data structure.
 *
 */


#ifndef ADSSET_H
#define ADSSET_H

#include "utility.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup set
 * @{
 * @brief a set is an abstract data type that can store certain values, without any particular order, and no repeated values.
 *
 * @details It is a computer implementation of the mathematical concept of a finite set.
 * Unlike most other collection types, rather than retrieving a specific element from a set,
 * one typically tests a value for membership in a set.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%set(#) - # is a non-empty character or string specifying the format specifier of that type contained by the set
 *      - @b {set(#)} - prints the set to the stdout.
 *      - @b @%<#> - # is a non-empty character or string specifying the format specifier of that type contained by the set
 *      - @b {<#>} - prints the set to the stdout.
 */

 /**
 * @struct set set.h <ads/set.h>
 * @brief a set is an abstract data type that can store certain values, without any particular order, and no repeated values.
 *
 * @details It is a computer implementation of the mathematical concept of a finite set.
 * Unlike most other collection types, rather than retrieving a specific element from a set,
 * one typically tests a value for membership in a set.
 */
ads_classProto(ads_set);

extern const ads_td * const ads_setTD;

/** @brief set constructor.
 *
 * @param type the data-type or domain of the containing data
 * @param hashfunction hash function of the type
 * @param size the initial size of the set
 * @return a set instance
 *
 */
#define ads_setNew(type,hashfunction,size) ads_initSet(sizeof(type),hashfunction,size)

/** @cond ADS_INTERNAL */
ads_set ads_initSet(int datatype,uint64 (*reprfunction) (const void*),int size);
void ads_setSetTD(ads_set self,const ads_td*td);
/** @endcond */

/** @brief returns no of elements in the set.
 *
 * @param self a set instance
 * @return integer value
 *
 */
int ads_setLen(ads_set self);

/** @cond ADS_INTERNAL */
ads_rt ads_setAddItem_NoCopy(ads_set self,void * value);
/** @endcond */

/** @brief insert a value into ads_set
 *
 * @param self a set instance
 * @param value the value to be inserted
 = @param type the container type of the set
 * @return void
 *
 */
#define ads_setAddItem(self,value,type) ADS_APPEND(ads_setAddItem_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
ads_rt ads_setAddItem_ByRef(ads_set self,void * value);
/** @endcond */

/** @brief delete the value from the set
 *
 * @param self a set instance
 * @param value the value to be deleted
 = @param type the container type of the set
 * @return void
 *
 */
#define ads_setDiscardItem(self,value,type) ADS_APPEND(ads_setDiscardItem_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
ads_rt ads_setDiscardItem_ByRef(ads_set self,void * value);
/** @endcond */

/** @brief check weather the set contains the value
 *
 * @param self a set instance
 * @param value the value to be verified
 = @param type the container type of the set
 * @return void
 *
 */
#define ads_setContains(self,value,type) ADS_APPEND(ads_setContains_ByRef,self,value,type)

/** @cond ADS_INTERNAL */
bool ads_setContains_ByRef(ads_set self,void * value);
/** @endcond */

/** @brief removes a element from set and returns it.
 *
 * @param self a set instance
 * @param type the container type of the set
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_setPop(self,type,result) ADS_POP(ads_setPop_RtnRef,self,type,result)

/** @cond ADS_INTERNAL */
void* ads_setPop_RtnRef(ads_set self);
/** @endcond */

/** @brief copy constructor.
 *
 * @param self a set instance
 * @return set instance
 */
ads_set ads_setCopy(ads_set _self);

/** @cond ADS_INTERNAL */
void ads_printSet(void*add,const char * cmd,int size);
/** @endcond */

/**
 * @brief remove all elements from the set.
 *
 * @param self takes a set instance.
 * @return void
 */
void ads_setClear(ads_set self);

/** @brief destructor.
 *
 * @param self a set instance
 * @return void
 *
 */
void ads_setDelete(ads_set _self);

/**
 * @brief returns a intersection of two set
 *
 * @param self takes a set instance
 * @param other takes a set instance
 * @return a set
 */
ads_set ads_setIntersection(ads_set self,ads_set other);

/**
 * @brief returns a true is other is subset of self
 *
 * @param self a set instance
 * @param other a set instance
 * @return a boolean
 */
bool ads_setIsSubset(ads_set self,ads_set other);

/**
 * @brief returns a true is other is not subset of self
 *
 * @param self a set instance
 * @param other a set instance
 * @return a boolean
 */
bool ads_setIsDisjoint(ads_set self,ads_set other);

/**
 * @brief returns a difference of two set
 *
 * @param self takes a set instance
 * @param other takes a set instance
 * @return a set
 */
ads_set ads_setDifference(ads_set self,ads_set other);

/**
 * @brief returns a union of two set
 *
 * @param self takes a set instance
 * @param other takes a set instance
 * @return a set
 */
ads_set ads_setUnion(ads_set self,ads_set other);

/** @brief initialize the set with variables.
 *
 * @param self a set instance
 * @param type the container type of the set
 * @param ... the variables to initialize the set with.
 * @return void
 *
 */
#define ads_setBuilt(self,type,...) {type aaa[] = {__VA_ARGS__};ads_setBuilt_FrmArray(self,aaa,sizeof(aaa)/sizeof(type));}

/** @cond ADS_INTERNAL */
void ads_setBuilt_FrmArray(ads_set self,void*arr,int n);
/** @endcond */

/** @} */

#ifdef __cplusplus
}
#endif


#endif

