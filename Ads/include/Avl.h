
/** @file Avl.h
 * @brief Adelson Velsky and Landis Tree data structure.
 *
 * an AVL tree is a self-balancing binary search tree.
 */



#ifndef ADSAVLTREE_H
#define ADSAVLTREE_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utility.h"
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup Avltree
 * @{
 * @brief A self-balancing binary search tree.
 * @details In an AVL tree, the heights of the two child subtrees of
 * any node differ by at most one; if at any time they differ by more than one, rebalancing is done
 * to restore this property. Lookup, insertion, and deletion all take O(log n) time in both the average and worst cases,
 * where n is the number of nodes in the tree prior to the operation. Insertions and deletions may require the tree to be
 * rebalanced by one or more tree rotations.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%avl(#:#) - # is a non-empty character or string specifying the format specifier of that type contained by the avl
 *      - @b {avl(#:#)} - prints the avl to the stdout.
 *
 * <h4>Example</h4>
 * creating a avl tree.
 * @code {.c}
 * ads_avl t = ads_new(ads_avl,int,ads_cmpInt,int);
 * @endcode
 *
 * insert values into avltree.
 * @code {.c}
 * ads_avlInsert(t,1,int,30,int); //Insert some key-Value pairs in avltree
 * ads_avlInsert(t,2,int,78,int);
 * @endcode
 *
 * printing an avltree.
 * @code {.c}
 * ads_printf("printing Avl = {avl(d:d)}",t);
 * @endcode
 *
 * Traversing an avltree.
 * @code {.c}
 * ads_vector l = ads_avlTraversal(t,ads_postOrder,ads_avlKey); //do a  post order traversal of the keys in the tree
 * ads_printf("postOrder = {[d]} ",l);
 * @endcode
 *
 * delete an avltree.
 * @code {.c}
 * ads_delete(l,t);
 * @endcode
 */


 /** @cond ADS_INTERNAL */
typedef struct ads_avlNode ads_avlNode;
/** @endcond */

/**
 * @struct ads_avl Avl.h <ads/Avl.h>
 * @brief A self-balancing binary search tree.
 *
 * @details In an AVL tree, the heights of the two child subtrees of
 * any node differ by at most one; if at any time they differ by more than one, rebalancing is done
 * to restore this property. Lookup, insertion, and deletion all take O(log n) time in both the average and worst cases,
 * where n is the number of nodes in the tree prior to the operation. Insertions and deletions may require the tree to be
 * rebalanced by one or more tree rotations.
 */

ads_classProto(ads_avl);

extern const ads_td * const ads_avlTD ;/**< ads_avlTD type-definition of the Avltree tree.*/

/** @name Avltree Traversal
 * @brief These Macro should be used with traversal functions
 */

/** @{ */

#define ads_inOrder 0       /**< Specify to do a in-order traversal */
#define ads_preOrder 1      /**< Specify to do a pre-order traversal */
#define ads_postOrder 2     /**< Specify to do a post-order traversal */

#define ads_avlKey 1    /**< Specify to do a traversal retrieving keys  */
#define ads_avlData 0   /**< Specify to do a traversal retrieving data  */

/** @} */


/** @brief Avltree Constructor.
 *
 * @param keytype the data-type or domain of the key values
 * @param cmpfunction it is the compare function of the key data-type
 * @param datatype the data-type or domain of data values
 * @return a avltree instance
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_avl intree = ads_avlNew(int,ads_cmpInt,int);
 * ads_avl stree = ads_avlNew(ads_str,ads_cmpString,int);
 * @endcode
 */
#define ads_avlNew(keytype,cmpfunction,datatype)  ads_initAvl(sizeof(keytype),cmpfunction,sizeof(datatype))

/** @cond ADS_INTERNAL */
ads_avl ads_initAvl(int keytype, int (*compare)(const void * data1,const void*data2),int datatype);
/** @endcond */

/** @brief Balances the binary tree if not balanced properly.
 *
 * @param self a avltree instance
 * @return Void
 *
 */
ads_rt ads_balanceAvl( ads_avl self);

/** @brief insert a key-value pair into the avltree
 *
 * @param self a avltree instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param value a data value of same type as datatype
 * @param datatype the data-type of value
 * @return void
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_avl intree = ads_avlNew(int,intcompare,int);
 * ads_avlInsert(intree,2,53,int,int);
 * ads_avlInsert(intree,3,695,int,int);
 * ads_avlDelete(intree);
 * @endcode
 */
#define ads_avlInsert(self,key,keytype,value,datatype) ADS_KEYVALUE(ads_avlInsert_ByRef,self,key,value,keytype,datatype)

/** @cond ADS_INTERNAL */
ads_rt ads_avlInsert_ByRef(ads_avl self,void*key,void* value);
/** @endcond */

/** @brief returns the no of keys in the Avltree.
 *
 * @param self a Avltree instance
 * @return no of keys in the Avltree
 *
 */
int ads_avlLen(ads_avl self);

/** @brief returns true if the avltree contains the key
 *
 * @param self a avltree instance
 * @param key a key value of same type as keytype
 * @param type the data-type of key
 * @return boolean value
 *
 */
#define ads_avlContains(self,key,type,result)  ADS_KEYRETURN(ads_avlContains_ByRef,self,key,type,result)

/** @cond ADS_INTERNAL */
bool ads_avlContains_ByRef(ads_avl self,void*key);
/** @endcond */

/** @brief returns the element at the respective key
 *
 * @param self a Avltree instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param returntype the data-type of value
 * @param result the variable where the return value will be stored
 * @return void
 *
 */
#define ads_avlGet_Copy(self,key,keytype,returntype,result) ADS_KEY_VALUE_TYPE_FREE(ads_avlGet_Copy_ByRef,self,key,keytype,returntype,result)

#define ads_avlGet_NoCopy(self,key,keytype,returntype,result) ADS_KEY_VALUE_TYPE(ads_avlGet_NoCopy_ByRef,self,key,keytype,returntype,result)
/** @cond ADS_INTERNAL */
const void* ads_avlGet_NoCopy_ByRef(ads_avl self,const void*key);
void* ads_avlGet_Copy_ByRef(ads_avl self,const void*key);
void* ads_avlGetMax_Copy_RtnRef(ads_avl self);
const void* ads_avlGetMax_NoCopy_RtnRef(ads_avl self);
const void* ads_avlGetRoot_NoCopy_RtnRef(ads_avl self);
const void* ads_avlGetMin_NoCopy_RtnRef(ads_avl self);
void* ads_avlGetRoot_Copy_RtnRef(ads_avl self);
void* ads_avlGetMin_Copy_RtnRef(ads_avl self);
/** @endcond */


/** @brief the function returns the key with the maximum value.
 *
 * @param ads_avlGetMax_Copy_RtnRef
 * @param self
 * @param type
 * @param result
 * @return maximum valued key
 *
 */
#define ads_avlGetMax_NoCopy(self,type,result) ADS_POP_NO_FREE(ads_avlGetMax_NoCopy_RtnRef,self,type,result)

/** @brief the function returns the key at root of the tree.
 *
 * @param ads_avlGetMax_Copy_RtnRef
 * @param self
 * @param type
 * @param result
 * @return mean valued key
 *
 */
#define ads_avlGetRoot_NoCopy(self,type,result) ADS_POP_NO_FREE(ads_avlGetRoot_NoCopy_RtnRef,self,type,result)

/** @brief the function returns the key with the minimum value.
 *
 * @param ads_avlGetMin_Copy_RtnRef
 * @param self
 * @param type
 * @param result
 * @return minimum valued key
 *
 */
#define ads_avlGetMin_NoCopy(self,type,result) ADS_POP_NO_FREE(ads_avlGetMin_NoCopy_RtnRef,self,type,result)

/** @brief deletes a key-value pair from the avltree
 *
 * @param self a avltree instance
 * @param key a key value of same type as keytype
 * @param type the data-type of key
 * @return void
 *
 */
#define ads_avlRemove(self,key,type)  ADS_APPEND(ads_avlRemove_ByRef,self,key,type)

/** @brief returns the element at the respective key and removes it from the tree.
 *
 * @param self a Avltree instance
 * @param key a key value of same type as keytype
 * @param keytype the data-type of key
 * @param returntype the data-type of value
 * @param result the variable where the return value will be stored
 * @return void
 *
 */
#define ads_avlPop(self,key,keytype,returntype,result) ADS_KEY_VALUE_TYPE_FREE(ads_avlPop_ByRef,self,key,keytype,returntype,result)

/** @cond ADS_INTERNAL */
void* ads_avlPop_ByRef(ads_avl self,const void*key);
ads_rt ads_avlRemove_ByRef(ads_avl self,const void*key);
/** @endcond */

/** @brief deletes a avltree and deallocate all the memory associated with it.
 *
 * @param self a avltree instance
 * @return void
 *
 */
void ads_avlDelete(ads_avl _self);

/** @brief set type-definition of the container type of the avltree.
 *
 * @param self ads_avl
 * @param key_or_data bool
 * @param td const ads_td*
 * @return ads_rt
 *
 */
ads_rt ads_avlSetTD(ads_avl self, bool key_or_data ,const ads_td * td);

/** @cond ADS_INTERNAL */
void ads_printAvl(ads_avl self,void (*keyads_outputunction)(void*),void (*dataads_outputunction)(void*));
void ads_printAvlInternal(void*add,const char*cmd,int size);
/** @endcond */

/** @brief return a exact copy of the tree
 *
 * @param self a avltree instance
 * @return a avl tree
 *
 */
ads_avl ads_avlCopy(ads_avl _self);

/** @brief traverse the tree in a particular order and return a ads_vector of data.
 *
 * @param self a avltree instance
 * @param order the tree traversal order
 * @param key_or_data specify weather to fetch data or keys. #ADS_AvltreeKEY #ADS_AvltreeDATA
 * @see ADS_INORDER
 * ADS_PREORDER ADS_POSTORDER
 * @return a ads_vector of data
 *
 */
ads_vector ads_avlTraversal(ads_avl self,int order,bool key_or_data);

/** @} */

#ifdef __cplusplus
}
#endif

#endif



