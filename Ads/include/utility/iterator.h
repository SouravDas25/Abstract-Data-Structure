

/** @file iterator.h
 * @brief a ads iterator module.
 *
 */

#ifndef ADSITERATOR_H
#define ADSITERATOR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#include<math.h>
#include<stdarg.h>
#include <time.h>
#include<stdbool.h>
#include<stdint.h>

#include "datareplicator.h"


/**
 * @addtogroup Iterator
 * @{
 * @brief a container iterating loop for ads.
 */



typedef struct ads_structiterator ads_structiterator;
typedef struct ads_structiterator ads_iterator;

typedef void*(*ads_iterator_function)(ads_iterator*) ;

struct ads_structiterator { /*iterator struct */
	void* object;
	ads_iterator_function startfunc;
	ads_iterator_function nextitem;
	void* extra;
	unsigned int max;
	unsigned int count;
};



/** @brief Creates a new iterator object.
 *
 * @param object object or variable on which the iteration is to be performed
 * @param next a function to call at each step when iterating through the object
 * @param start a function that is called when a iteration starts. It is called only once.
 * @param max the maximum number of time the iterator should loop before breaking loop
 * @param extra extra memory space for usage as per required.
 * @return a iterator instance.
 *
 */
ads_iterator ads_initIterator(void* object, ads_iterator_function next, ads_iterator_function start, int max, void* extra); /*iterator constructor */


/** @cond ADS_INTERNAL */

/*iterator start function.*/
void* ads_iteratorStart(ads_iterator* _i);

/*iterator next Function.*/
void* ads_iteratorNext(ads_iterator* _i);

#define ads_refItem(var,type) (*(type*)var)
#define ads_cast(var,type) ((type)var)

#define ADS_GETITEM(function,self,index_or_key,type,result) \
            do {\
                void * _ADSAUTOFREE_GETITEM_VOID_PTR_VAR_ = function(self,index_or_key);\
                if(_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_)\
                {\
                    result = *(type*)_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_;\
                }\
            }while(0) \

#define ADS_GETITEM_FREE(function,self,index_or_key,type,result) \
            do {\
                void * _ADSAUTOFREE_GETITEM_VOID_PTR_VAR_ = function(self,index_or_key);\
                if(_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_ != NULL) \
                {\
                    result = *(type*)_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_;\
                    ads_free(_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_);\
                }\
            } while(0) \

#define ADS_KEY_VALUE_TYPE(function,self,key,keytype,type,result) \
        do {\
            keytype  ADS_Check_KEYTYPEVAR_INTERMIDIATE_NOTNEED__ = (key);\
            void * _ADSAUTOFREE_GETITEM_VOID_PTR_VAR_ = function(self,(void*)&ADS_Check_KEYTYPEVAR_INTERMIDIATE_NOTNEED__);\
            if(_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_) \
            {\
                result = *(type*)_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_;\
            }\
        } while(0)

#define ADS_KEY_VALUE_TYPE_FREE(function,self,key,keytype,type,result) \
        do {\
            keytype  ADS_Check_KEYTYPEVAR_INTERMIDIATE_NOTNEED__ = (key);\
            void * _ADSAUTOFREE_GETITEM_VOID_PTR_VAR_ = function(self,(void*)&ADS_Check_KEYTYPEVAR_INTERMIDIATE_NOTNEED__);\
            if(_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_) \
            {\
                result = *(type*)_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_;\
                ads_free(_ADSAUTOFREE_GETITEM_VOID_PTR_VAR_);\
            }\
        }while(0)

#define ADS_KEYRETURN(function,self,key,type,result) \
        do {\
            type ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__ = (key); \
            result = function(self,(void*)&ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__);\
		}while(0)


#define ADS_POP_NO_FREE(function,self,type,result) \
        do {\
			void * _ADSAUTOFREE_POP_VOID_PTR_VAR_ = function(self);\
			if(_ADSAUTOFREE_POP_VOID_PTR_VAR_)\
			result =  *(type*)_ADSAUTOFREE_POP_VOID_PTR_VAR_; \
        }while(0)

#define ADS_POP(function,self,type,result) \
        do {\
			void * _ADSAUTOFREE_POP_VOID_PTR_VAR_ = function(self);\
			if(_ADSAUTOFREE_POP_VOID_PTR_VAR_)\
            {\
                result =  *(type*)_ADSAUTOFREE_POP_VOID_PTR_VAR_; \
                ads_free(_ADSAUTOFREE_POP_VOID_PTR_VAR_);\
            }\
        }while(0)

#define ADS_APPEND(function,self,value,type) \
        do { \
            type ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__ = (value);\
            function(self,(void*)&ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__);\
		}while(0)

#define ADS_KEYVALUE(function,self,key,value,keytype,datatype) \
        do { \
            keytype  ADS_Check_KEYTYPEVAR_INTERMIDIATE_NOTNEED__ = (key);\
            datatype ADS_Check_DATATYPEVAR_INTERMIDIATE_NOTNEED__ = (value);\
            function(self,(void*)&ADS_Check_KEYTYPEVAR_INTERMIDIATE_NOTNEED__,(void*)&ADS_Check_DATATYPEVAR_INTERMIDIATE_NOTNEED__);\
        }while(0)

#define ADS_SETITEM(function,self,index,value,type) \
        do { \
            type ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__ = (value);\
            function(self,index,(void*)&ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__);\
		}while(0)

#define ADS_SENDFUNC(function,self,sendfunc,value,type) { \
		type ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__ = (value);\
		function(self,(void*)&ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__,sendfunc);	}

#define ADS_SENDFUNC_RETURN(function,self,sendfunc,value,type,result)\
        do { \
            type ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__ = (value);\
            result=function(self,(void*)&ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__,sendfunc);\
		}while(0)

#define ADS_KEYVALUE_RETURN(function,self,key,type,result) \
        do { \
            type ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__ = (key);\
            result=function(self,(void*)&ADS_Check_TYPEVAR_INTERMIDIATE_NOTNEED__,sendfunc);\
		}while(0)

/** @endcond */

#define ADS_loopItr(name) __ads_loopitr_##name##__
#define ADS_loopVar(name) __ads_loopvar_##name##__
#define ADS_loopIsAdsVar(name) __ads_loopisadsvar_##name##__


/** @brief a foreach loop it takes a iterator a calls the iterator start and next function till a stop is encountered.
 *
 * @param loopvar the variable that contains the the value of each iteration
 * @param itrtor the iterator instance
 * @param type the type of variable that is returned at each iteration
 *
 * <h4>Example</h4>
 * @code {.c}
 *
 * ads_foreach(item , ads_vectorIterator(v1) , int)
 * {
 *      sum += item;
 * }ads_next(item);
 *
 * @endcode
 *
 * @code {.c}
 *
 * ads_foreach(item , ads_vectorIterator(v1) , int)
 * {
 *      ads_vectorSetItem(v1, ads_enumerate(item) , item* 20 ,int);
 * }ads_next(item);
 *
 * @endcode
 */
#define ads_foreach(loopvar,itrtor,type)   \
do { \
    ads_iterator ADS_loopItr(loopvar) = (itrtor);\
    void * ADS_loopVar(loopvar) = NULL; \
    for ( ADS_loopVar(loopvar) = ads_iteratorStart(&ADS_loopItr(loopvar)); \
          ADS_loopVar(loopvar) != NULL ; \
          ADS_loopVar(loopvar) = ads_iteratorNext( &ADS_loopItr(loopvar) ) \
        )\
        {\
            type loopvar = *(type*)ADS_loopVar(loopvar);


/** @brief End part of the foreach loop.
 * @param loopvar the loop variable passed at foreach loop.
 * This is mandatory or else the foreach loop won't work.
 */
#define ads_next(loopvar) \
        }\
    } while(0);

/** @brief return current no of time the foreach loop is looping
 *
 * @param loopvar the looping variable
 * @return int the count index
 *
 */
#define ads_enumerate(loopvar) ads_enumerate_ByRef( & ADS_loopItr(loopvar) )
/** @cond ADS_INTERNAL */
int ads_enumerate_ByRef(ads_iterator*i);
/** @endcond */

/** @brief A iterator over an array.
 *
 * @param array the array to iterate over
 * @param size the size of the array
 *
 */
#define ads_arrayIterator(array,size) __ads_arrayIterator(array,sizeof(*array),size)

/** @cond ADS_INTERNAL */
ads_structiterator __ads_arrayIterator(void * arr,int64_t datatype,int size);
/** @endcond */

/** @} */

#endif

