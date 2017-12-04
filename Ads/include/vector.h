
/**
 * @file vector.h
 * @brief A vector Data Structure.
 *
 * A vector is a dynamic array which can grow to
 * fit more elements. It is same as the c++ Vector
 * Class
 *
 *
 */

#ifndef ADS_VECTOR_H
#define ADS_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utility.h"


/**
 * @addtogroup vector
 * @{
 * @brief A dynamic array.
 *
 * @details A vector is a dynamic array which can grow to
 * fit more elements. It is same as the c++ Vector
 * Class
 *
 * <h4>Format Specifier</h4>
 *      - @b @%vec(#) - # is a non-empty character or string specifying the format specifier of that type contained by the vector
 *      - @b {vec(#)} - prints the vector to the stdout.
 *      - @b @%[#] - # is a non-empty character or string specifying the format specifier of that type contained by the vector
 *      - @b {[#]} - prints the vector to the stdout.
 *
 * <h4>Example</h4>
 * let's create a new vector which contains integer in it.
 * @code {.c}
 * ads_vector v = ads_new(ads_vector,int,2);  //creating a new vector.
 * @endcode
 * the variable v is of type vector and has a free space for holding 2 integer
 * Now lets insert some values.
 * @code {.c}
 * ads_vectorAppend(v,25,int);  //inserting values to the vector.
 * ads_vectorAppend(v,30,int);
 * ads_vectorAppend(v,35,int); //reallocation occurred
 * @endcode
 * the value 25 is inserted at the end of the vector. as no element is present it is inserted at the first index.\n
 * and 30 was inserted after 25 at index 1.
 * but when 65 was inserted the vector has to reallocate to increase its size to accommodate more elements.
 *
 * we can insert a lot of values directly in the vector via array.
 * @code {.c}
 * int array[10] = {0,1,2,3,4,5,6,7,8,9};
 * ads_vectorBuilt_FrmArray(v,array,10);
 * //OR
 * ads_vectorBuilt(v,int,0,1,2,3,4,5,6,7,8,9); //This Macro is only available in C99 and above.
 * @endcode
 *
 * Now let's fetch some values from the vector.
 * @code {.c}
 * int j;
 * ads_vectorGetItem_NoCopy(v,0,int,j); //getting values from vector.
 * @endcode
 * the ads_vectorGetItem_NoCopy() function fetches the value at index 0 and copies it to the j variable.
 *
 * we can also sort the vector by using appropriate functions.
 * @code {.c}
 * ads_vectorSort(v,ads_cmpInt);
 * @endcode
 * ads_cmpInt() is a function which returns(-1,0,1)
 *
 * printing a vector of int's.
 * @code {.c}
 * ads_printf("vec = {[d]}",v);
 * @endcode
 *
 * for printing each elements in hex
 * @code {.c}
 * ads_printf("vec = {[x]}",v);
 * @endcode
 *
 * all vector variable must be deleted through ads_delete() or ads_deleteOne() explicitly.
 * @code {.c}
 * ads_delete(v); //deleting the vector.
 * @endcode
 *
 */

/**
 * @struct  ads_vector vector.h <ads/vector.h>
 * @brief A dynamic array.
 *
 * @details A vector is a dynamic array which can grow to
 * fit more elements. It is same as the c++ Vector
 * Class
 *
 */
ads_classProto(ads_vector);


/** @brief a vector type definition , used to create vector of vectors.
 *
 */
extern const ads_td * const ads_vectorTD;

/**
 * @brief A macro Constructor of the vector class.
 * @details Call this constructor to create a new instance of the class.
 * This constructor is called by the ads_new() to create a vector.
 *
 * @param type the type of variable to be stored inside the vector.
 * @param size the initial size of the vector.
 * @return a vector instance with datatype(type) and size(size).
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_vector v = ads_new(ads_vector,int,10);  //creating a new vector via new.
 * ads_vector v1 = ads_vectorNew(int,10); //creating a vector by directly calling the constructor.
 * ads_delete(v,v1); //deleting the vector.
 * @endcode
 */
#define ads_vectorNew(type,size)  ads_initVector(sizeof(type) , size  )

/**
 * @brief Constructor of the vector class.
 *
 * @param datatype takes the size of a datatype
 * @param size minimum size of the vector or vetor array
 * @param ads this parameter is set to 1 if the vector is storing a ads datatype like a vector,ads_str etc.
 * @return vector instance.
 *
 */
ads_vector ads_initVector(int typesize,int size);

/**
 * @brief returns no of elements in the vector.
 *
 * @param self takes a vector instance
 * @return No of item in the vector at present
 *
 */
int ads_vectorLen(const ads_vector self);

/** @brief set type definition (copy constructor, destructor) for the stored data type.
 *
 * @param self ads_vector the vector
 * @param td ads_td* the type definition variable.
 * @return void
 *
 * the above copy constructor and a destructor will be used to copy or destroy data variable instead
 * of a default shallow copy constructor and destructor.
 */
ads_rt ads_vectorSetTD(ads_vector self,const ads_td* td);

/**
 * @brief returns the current array size of the vector.
 * @details This may be than the no of elements in the vector.
 *  As the vector resizes to accommodate more elements.
 *
 * @param self takes a vector instance
 * @return return the no of Occupied + Free memory slots.
 *
 */
int ads_vectorMemory(const ads_vector self);

/** @brief grows the vector to a new size
 *
 * @param self ads_vector the vector
 * @param newsize int the new size to regrow
 * @return void
 *
 */
ads_rt ads_vectorRellocate(ads_vector self, int newsize);

/**< @cond ADS_INTERNAL */
ads_rt ads_vectorAppend_NoCopy(ads_vector self,void * value);
/**< @endcond  */

/**
 * @brief Insert a element at the end of the vector.
 *
 * @param self takes a vector instance
 * @param value any variable or constant
 * @param type type of the value variable or constant.
 *
 */
#define ads_vectorAppend(self,value,type) ADS_APPEND(ads_vectorAppend_ByRef,self,value,type)

/**< @cond ADS_INTERNAL */
ads_rt ads_vectorAppend_ByRef(ads_vector self,void * value);
ads_td* ads_vectorGetTD(const ads_vector self);
/**< @endcond  */

/**< @cond ADS_INTERNAL */
#define ads_vectorGetItem_Copy(self,index,type,result) ADS_GETITEM_FREE(ads_vectorGetItem_Copy_RtnRef,self,index,type,result)
void* ads_vectorGetItem_Copy_RtnRef(ads_vector self,int index);
/**< @endcond  */

/**
 * @brief return the item at index to the variable result.
 *
 * @param self takes a vector instance
 * @param index is the int index position just like the array starting from 0.
 * @param type of the data stored in the vector.
 * @param result a variable where the vector[index] will be put. ie result = vector[index].
 *
 */
#define ads_vectorGetItem_NoCopy(self,index,type,result) ADS_GETITEM(ads_vectorGetItem_NoCopy_RtnRef,self,index,type,result)

/**< @cond ADS_INTERNAL */
void* ads_vectorGetItem_NoCopy_RtnRef(ads_vector self,int index);
/**< @endcond  */

/**
 * @brief replace the element at index with value.
 *
 * @param self takes a vector instance
 * @param index is the int index position just like the array starting from 0.
 * @param value any variable or constant
 * @param type of the value.
 *
 */
#define ads_vectorSetItem(self,index,value,type) ADS_SETITEM(ads_vectorSetItem_ByRef,self,index,value,type)

/**< @cond ADS_INTERNAL */
ads_rt ads_vectorSetItem_ByRef(ads_vector self,int index,void*value);
/**< @endcond  */

/**
 * @brief insert the value before the element at index.
 *
 * @param self takes a ads_vector instance
 * @param index is the int index position just like the array starting from 0.
 * @param value any variable or constant
 * @param type of the value.
 *
 */
#define ads_vectorInsert(self,index,value,type) ADS_SETITEM(ads_vectorInsert_ByRef,self,index,value,type)

/**< @cond ADS_INTERNAL */
ads_rt ads_vectorInsert_ByRef(ads_vector self,int index,void*value);
/**< @endcond  */

/**
 * @brief insert all elements from Other to self
 *
 * @param self takes a ads_vector instance
 * @param other takes a ads_vector instance
 *
 */
ads_rt ads_vectorExtend(ads_vector self,ads_vector other);

/**
 * @brief reverse the ads_vector.
 *
 * @param self takes a ads_vector instance
 */
ads_rt ads_vectorReverse(ads_vector self);

/**
 * @brief delete the element at index.
 *
 * @param self takes a ads_vector instance
 * @param index is the int index position just like the array starting from 0.
 *
 */
ads_rt ads_vectorDelItem(ads_vector self,int index) ;

/**
 * @brief remove and return the value at index 0.
 *
 * @param self takes a ads_vector instance
 * @param type of the data stored in the ads_vector.
 * @param result a variable where the pop element will be returned to.
 */
#define ads_vectorPop(self,type,result) ADS_POP(ads_vectorPop_RtnRef,self,type,result)

/**< @cond ADS_INTERNAL */
void* ads_vectorPop_RtnRef(ads_vector self);
/**< @endcond  */

/**
 * @brief reduce the memory of the ads_vector to exactly the  no of elements.
 *
 * @param self takes a ads_vector instance
 *
 */
ads_rt ads_vectorShrink(ads_vector self);

/**
 * @brief iterate through the ads_vector calling the mapfunction with each element.
 *
 * @param self takes a ads_vector instance
 * @param function a mapping function for each element.
 *
 */
void ads_vectorMap(ads_vector self,void(*function)(void*value));

/**
 * @brief a ads_vector iterator.
 *
 * @param self takes a ads_vector instance
 *
 */
ads_iterator ads_vectorIterator(ads_vector self);

/**< @cond ADS_INTERNAL */
void ads_printVector(void*add,const char * cmd,int size);
/**< @endcond  */

/**
 * @brief remove all elements from the ads_vector.
 *
 * @param self takes a ads_vector instance.
 *
 */
ads_rt ads_vectorClear(ads_vector self );

/**
 * @brief return a  copy of the ads_vector .
 * @details all elements are also copied recursively.
 *
 * @param l takes a ads_vector instance.
 * @return returns a ads_vector instance.
 *
 */
ads_vector ads_vectorCopy( ads_vector self);

/** @brief copy a vector from its address and returns a address to the copied vector.
 *
 * @param void*v the address of the vector to copy
 * @return void* the address of the copied vector.
 *
 */
void* ads_vectorCopy_ByRef_RtnRef(const void*v);

/**
 * @brief ads_vector destructor.
 * @details destroys a ads_vector and deallocate all memory.
 * @param l takes a ads_vector instance.
 *
 */
void ads_vectorDelete(ads_vector self );

/** @brief deallocate the from its address.
 * should only be used to deallocate vector allocated by ads_vectorCopy_ByRef_RtnRef()
 *
 * @param void*v the address of the vector.
 * @return void
 *
 */
void ads_vectorDelete_ByRef(void*v);

/**
 * @brief search the ads_vector for the value and returns  the index from the start of the vector.
 *
 * @param self takes a ads_vector instance
 * @param func takes a compare function.
 * @param value any variable or constant
 * @param type of the data stored in the ads_vector.
 * @param result a variable where the index will be returned.
 */
#define ads_vectorIndex(self,func,value,type,result) ADS_SENDFUNC_RETURN(ads_vectorIndex_ByRef,self,func,value,type,result)

/**< @cond ADS_INTERNAL */
int ads_vectorIndex_ByRef(ads_vector self,void *value,int (*compare)(const void*,const void*)) ;
/**< @endcond */

/**
 * @brief return the number of times a value occurs in the ads_vector.
 *
 * @param self takes a ads_vector instance
 * @param func takes a compare function.
 * @param value any variable or constant
 * @param type of the data stored in the ads_vector.
 * @param result a variable where the index will be returned.
 */
#define ads_vectorCount(self,func,value,type,result) ADS_SENDFUNC_RETURN(ads_vectorCount_ByRef,self,func,value,type,result)

/**< @cond ADS_INTERNAL */
int ads_vectorCount_ByRef(ads_vector self,void * value,int (*compare)(const void*,const void*));
/**< @endcond */

/**
 * @brief removes all occurrence of the value from the ads_vector.
 *
 * @param self takes a ads_vector instance
 * @param function takes a compare function.
 * @param value any variable or constant
 * @param type of the data stored in the ads_vector.
 */
#define ads_vectorRemove(self,function,value,type) ADS_SENDFUNC(ads_vectorRemove_ByRef,self,function,value,type)

/**< @cond ADS_INTERNAL */
ads_rt ads_vectorRemove_ByRef(ads_vector self,void * value,int (*compare)(const void*,const void*));
/**< @endcond */

/**
 * @brief sorts the ads_vector.
 *
 * @param self takes a ads_vector instance
 * @param compare takes a compare function.
 *
 */
ads_rt ads_vectorSort(ads_vector self,int (*compare)(const void*,const void*));

/**
 * @brief binary search the ads_vector for the value and returns the index.
 *
 * @param self takes a ads_vector instance
 * @param function takes a compare function.
 * @param value any variable or constant
 * @param type of the data stored in the ads_vector.
 * @param result a variable where the index will be returned.
 */
#define ads_vectorBinarySearch(self,function,value,type,result) ADS_SENDFUNC_RETURN(ads_vectorBinarySearch_ByRef,self,function,value,type,result)

/**< @cond ADS_INTERNAL */
int ads_vectorBinarySearch_ByRef(ads_vector self,void * value ,int (*compare)(const void*,const void*) ) ;
/**< @endcond */

/**< @cond ADS_INTERNAL */
/*
#define ads_vectorParse(stream,len,parsefunc,type) ads_vectorParse_ByRef(stream,len,parsefunc,sizeof(type))
ads_vector ads_vectorParse_ByRef(const char * stream,int len,void*parsefunction(const char * ,int),int datatype);
*/
/**< @endcond */

/**
 * @brief initialize a ads_vector with the values.
 *
 * @param self takes a ads_vector instance
 * @param type of the data stored in the ads_vector.
 * @param ... n no of value to be appended into the ads_vector.
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_vector v = ads_new(ads_vector,int,10);  //creating a new vector via new.
 * ads_vectorBuilt(v,int,1,2,3,4,5,6,7,8,9,6,3,5);
 * @endcode
 */
#define ads_vectorBuilt(self,type,...) {type ADS__aaa[] = {__VA_ARGS__};ads_vectorBuilt_FrmArray(self,ADS__aaa,sizeof(ADS__aaa)/sizeof(type));}

/** @brief create a vector from an array.
 * the given array must be of same type.
 * @param self ads_vector the vector.
 * @param void*arr the starting address of the array
 * @param n int no of element in the array.
 * @return void
 *
 */
void ads_vectorBuilt_FrmArray(ads_vector self,void*arr,int n);



#ifdef __cplusplus
}
#endif


/** @} */

#endif
