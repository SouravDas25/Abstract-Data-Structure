/** @file Dict.h
 * @brief a hash table (hash map).
 *
 * a structure that can map keys to values.
 */



#ifndef ADSDict_H
#define ADSDict_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "utility.h"
#include "vector.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup dict
 * @{
 * @brief A hash table (hash map) is a data structure used to implement an associative array.
 *
 * @details a hash table (hash map) is a data structure used to implement an associative array,
 * a structure that can map keys to values. A hash table uses a hash function to compute an index into an array of buckets or slots,
 * from which the desired value can be found.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%dict(#:#) - # is a non-empty character or string specifying the format specifier of that type contained by the dict
 *      - @b {dict(#:#)} - prints the dict to the stdout.
 *      - @b @%[#:#] - # is a non-empty character or string specifying the format specifier of that type contained by the dict
 *      - @b {[#:#]} - prints the dict to the stdout.
 *
 * <h4>Example</h4>
 *
 * Creating a dict.
 * @code {.c}
 * ads_dict d = ads_new(ads_dict,int,ads_hashInt,int,32);
 * ads_dict d1 = ads_dictNew(ads_str,ads_strHash,int);
 * @endcode
 *
 * inserting some key-value pairs into the dict.
 * @code {.c}
 * ads_dictSetItem(d,5,int,5*10,int);
 * @endcode
 *
 * printing the dict.
 * @code {.c}
 * ads_printf("dict = {[d:d]} ",d);
 * @endcode
 *
 * checks weather a dict has a particular key.
 * @code {.c}
 * ads_dictHasKey(d,10,int); //return true if the dict has the key 10 else false
 * @endcode
 *
 * fetch a values through a key.
 * @code {.c}
 * ads_dictGetItem(d,5,int,int,val); //returns the value corresponding to the key
 * @endcode
 *
 * delete a dict.
 * @code {.c}
 * ads_delete(d,d1); //returns the value corresponding to the key
 * @endcode
 */

 /**
 * @struct dict Dict.h <ads/Dict.h>
 * @brief A hash table (hash map) is a data structure used to implement an associative array.
 *
 * @details a hash table (hash map) is a data structure used to implement an associative array,
 * a structure that can map keys to values. A hash table uses a hash function to compute an index into an array of buckets or slots,
 * from which the desired value can be found.
 */
ads_classProto(ads_dict);

extern const ads_td * const ads_dictTD;

/** @brief ads_dict Constructor.
 *
 * @param keytype the data-type or domain of the key values
 * @param hashfunction it is the hash function of the key data-type
 * @param datatype the data-type or domain of data values
 * @param size the initial size of the ads_dict
 * @return a ads_dict instance
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_dict d = ads_dictNew(int,hash_int,ads_str);
 * ads_dict d1 = ads_dictNew(ads_str,hash_string,int);
 * @endcode
 */
#define ads_dictNew(keytype,hashfunction,datatype,size) ads_initDict(sizeof(keytype),hashfunction,sizeof(datatype),size)

/** @cond ADS_INTERNAL */
ads_dict ads_initDict(int keytype,uint64_t (*hashfunction)(const void*),int datatype,int size);
void ads_dictSetDataTD(ads_dict self,const ads_td * td);
void ads_dictSetKeyTD(ads_dict self,const ads_td * td);
/** @endcond */

/** @brief returns true if the ads_dict contains the key
 *
 * @param self a ads_dict instance
 * @param key a key value of same type as keytype
 * @param type the data-type of key
 * @return boolean value
 *
 */
#define ads_dictHasKey(self,key,type) ADS_APPEND(ads_dictHasKey_ByRef,self,key,type)

/** @cond ADS_INTERNAL */
bool ads_dictHasKey_ByRef(ads_dict  self,void* key);
/** @endcond */

/** @brief returns the no of keys in the ads_dict.
 *
 * @param self a ads_dict instance
 * @return no of keys in the ads_dict
 *
 */
int ads_dictLen(ads_dict self);

/** @brief returns no of buckets full in the ads_dict
 *
 * @param self ads_dict
 * @return int
 *
 */
int ads_dictBuckets(ads_dict self);

/** @brief return the current no of buckets allocated to the ads_dict.
 *
 * @param self ads_dict
 * @return int
 *
 */
int ads_dictMemory(ads_dict self);

/** @brief returns a ads_vector of keys .
 *
 * @param self a ads_dict instance
 * @return ads_vector of keys
 *
 */
ads_vector ads_dictKeys(ads_dict self);

/** @brief returns a ads_vector of values.
 *
 * @param self a ads_dict instance
 * @return a ads_vector of values
 *
 */
ads_vector ads_dictValues(ads_dict self);

/** @brief returns the current load of the hash table.
 *
 * @param self a ads_dict instance
 * @return a double - value between (0.0 - 1.0)
 *
 */
double ads_dictLoad(ads_dict self);

/** @brief insert a key-value pair into the ads_dict if key not present
 * if key present replace the value
 *
 * @param self a ads_dict instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param value a data value of same type as datatype
 * @param datatype the data-type of value
 * @return void
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_dict d = ads_dictNew(int,hash_int,int);
 * ads_dictSetItem(d,2,int,53,int);
 * ads_dictSetItem(d,3,int,695,int);
 * @endcode
 */
#define ads_dictSetItem(self,key,keytype,value,valuetype) ADS_KEYVALUE(ads_dictSetitem_ByRef,self,key,value,keytype,valuetype)

/** @cond ADS_INTERNAL */
ads_rt ads_dictSetitem_ByRef(ads_dict self,void* key,void*value);
/** @endcond */

/** @brief returns the element at the respective key
 *
 * @param self a ads_dict instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param returntype the data-type of value
 * @param result the variable where the return value will be stored
 * @return void
 *
 */
#define ads_dictGetItem_NoCopy(self,key,keytype,returntype,result) ADS_KEYVALUETYPE_NOFREE(ads_dictGetItem_NoCopy_RtnRef,self,key,keytype,returntype,result)

/** @cond ADS_INTERNAL */
void* ads_dictGetItem_NoCopy_RtnRef(ads_dict self,void* key);
/** @endcond */

/** @brief returns the element at the respective key
 *
 * @param self a ads_dict instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param returntype the data-type of value
 * @param result the variable where the return value will be stored
 * @return void
 *
 */
#define ads_dictGetItem_Copy(self,key,keytype,returntype,result) ADS_KEY_VALUE_TYPE_FREE(ads_dictGetItem_Copy_RtnRef,self,key,keytype,returntype,result)

/** @cond ADS_INTERNAL */
void* ads_dictGetItem_Copy_RtnRef(ads_dict self,void* key);
/** @endcond */

/** @brief removes a key-value pair and return the value
 *
 * @param self a ads_dict instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param returntype the data-type of value
 * @param result the variable where the return value will be stored
 * @return void
 */
#define ads_dictPopItem(self,key,keytype,returntype,result) ADS_KEY_VALUE_TYPE_FREE(ads_dictPopItem_RtnRef,self,key,keytype,returntype,result)

/** @cond ADS_INTERNAL */
void* ads_dictPopItem_RtnRef(ads_dict self,void* key);
/** @endcond */

/** @brief removes a key-pair from the ads_dict
 *
 * @param self a ads_dict instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @return void
 *
 */
#define ads_dictDelItem(self,key,keytype) ADS_APPEND(ads_dictDelItem_ByRef,self,key,keytype)

/** @cond ADS_INTERNAL */
ads_rt ads_dictDelItem_ByRef(ads_dict self,void* key);
/** @endcond */

/** @brief returns the amount of memory occupied by the ads_dict
 *
 * @param self a ads_dict instance
 * @return returns memory in bytes
 *
 */
int ads_dictSizeOf(ads_dict self);

/** @brief add all the key-value pairs from other to self ads_dict
 *
 * @param self a ads_dict instance
 * @param other a ads_dict instance
 * @return void
 *
 */
ads_rt ads_dictExtend(ads_dict self,ads_dict other);

/** @cond ADS_INTERNAL */
void ads_printDict(void*add,const char * cmd,int size);
/** @endcond */

/** @brief returns a copy of the ads_dict
 *
 * @param self a ads_dict instance
 * @return a ads_dict instance
 *
 */
ads_dict ads_dictCopy(ads_dict _self);

/** @brief removes all key-value pair from the ads_dict
 *
 * @param self a ads_dict instance
 * @return void
 *
 */
void ads_dictClear(ads_dict self);

/** @brief deletes and deallocate the ads_dict
 *
 * @param self a ads_dict instance
 * @return void
 *
 */
void ads_dictDelete(ads_dict _self);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
