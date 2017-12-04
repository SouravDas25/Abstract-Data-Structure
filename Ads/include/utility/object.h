/** @file object.h
 * @brief a ads object creation module.
 *
 */


#ifndef C_ObjectOriented_H
#define C_ObjectOriented_H

#include "GC.h"
#include "Types.h"


/**
 * @addtogroup Object
 * @{
 * @brief a ads object creation module.
 */



 /** @cond ADS_INTERNAL */

#define ADS_StructOf(name) ads_##name##_object_struct
#define ADS_MethodStructName(name) ads_##name##_method_struct
#define ADS_MethodVariableName(name) ads_##name##_method_variable
#define ads_classMethodRetrivingObj(name) ads_##name##_method_retrive


#define ads_class(name)  typedef struct ADS_StructOf(name) ADS_StructOf(name) ; \
						typedef struct ADS_MethodStructName(name) ADS_MethodStructName(name);\
						struct ADS_MethodStructName(name) { \
							void* (*copyconstructor)(void*) ; \
							void (*destructor)(void*) ; \
							const char * type; \
							int TypeCode;\
						};\
						struct ADS_StructOf(name)

#define ads_init(name) struct ADS_MethodStructName(name)* tomethod

#define ads_linkClass(name,cc,d) name cc(name); \
								void d(name); ADS_MethodStructName(name) ADS_MethodVariableName(name) = { (void*(*)(void*))cc,(void (*)(void*))d,#name,0}



#define ads_classProto(name) typedef struct ADS_StructOf(name)* name

#define ads_cstrNew(type) strdup(type)
#define ads_arrayNew(type,n) (type*)ads_calloc((n),sizeof(type))

#define ads_varNew(type) (type*)ads_calloc(1,sizeof(type))

#define ads_CheckAndCastPointer(ptr,type,result) { type __ADS_TEMP_VARIABLE = ((type)ptr);\
ads_assert(__ADS_TEMP_VARIABLE,"NullError : The Given Variable is Null.");\
ads_assert( ((ADS_MethodStructName(type)*)__ADS_TEMP_VARIABLE->tomethod) == &ADS_MethodVariableName(type) ,\
"ads_typeError : The Given Variable is Not Of CorrectType");result = __ADS_TEMP_VARIABLE;}

#define ads_CheckPointer(ptr,type,result) { type __ADS_TEMP_VARIABLE = ((type)ptr);\
ads_assert(__ADS_TEMP_VARIABLE,"NullError : The Given Variable is Null.");\
result = ads_assert( ((ADS_MethodStructName(type)*)__ADS_TEMP_VARIABLE->tomethod) == &ADS_MethodVariableName(type) ,\
"ads_typeError : The Given Variable is Not Of CorrectType");}

typedef struct ads_objectMethod ads_objectMethod;

typedef struct ads_object ads_object;

#define ads_rawObject(name) (name)ads_rawObject_ByMacro(sizeof(ADS_StructOf(name)),&ADS_MethodVariableName(name))
void* ads_rawObject_ByMacro(int objectsize,void*mp);

/** @endcond */




/** @brief returns a copy of a ads variable
 *
 * @param variable the variable to copy
 * @param type the data-type of the variable
 * @return
 */
#define ads_copy(type,variable) (type)ads_duplicate(variable)

/** @brief returns a typeless copy of a ads variable
 *
 * @param void*var the variable to copy
 * @return void*
 *
 */
void* ads_duplicate(void*var);

/** @brief call the destructor of the class with the current variable as argument.
 *
 * @param void*var the variable who's destructor will be called.
 * @return void
 *
 */
void ads_callDestructor(void*var);

/** @brief get the type name in ads_str.
 *
 * @param void*var the type name of the object
 * @return const char* the ads_str.
 *
 */
const char * ads_getObjectTypeName(void*var);

/** @brief delete any dynamically allocated memory
 *
 * @param void*var the variable to delete
 * @return void
 *
 */
void ads_deleteOne(void*var);

/** @cond ADS_INTERNAL */
#define __ads_COUNT_PARMS2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,_11,_12,_13,_14,_15,_16, _count, ...) _count
#define ads_countParameters(...)\
	__ads_COUNT_PARMS2(__VA_ARGS__, 16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
/** @endcond */

/** @brief allocate any object
 *
 * @param type the type of object to allocate.
 * @param ... the object constructor parameter.
 * @return #define ads_new(type,...)
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_vector v = ads_new(ads_vector,int,10);
 * int * dynamicInt = ads_new(ads_var,int);
 * int * intArray = ads_new(ads_array,int,10);
 * char * s = "This is a const";
 * char * cstr = ads_new(ads_cstr,s); //allocate and copies the c-string.
 * @endcode
 */
 #define ads_new(type,...) type##New(__VA_ARGS__)

/** @brief delete more than one dynamically allocated memory
 *
 * @param ... the variable to delete
 * @return void
 *
 * Every object allocated by the ads_new() can be deallocated by this function
 */
#define ads_delete(...) ads_deleteMany(ads_countParameters(__VA_ARGS__),__VA_ARGS__)

/** @brief delete more than one dynamically allocated variable
 *
 * @param n int no of variables to delete
 * @param ... the variables.
 * @return void
 *
 */
void ads_deleteMany(int n,...);


/** @} */

#endif
