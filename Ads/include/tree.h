/** @file Tree.h
 * @brief a tree is a widely used abstract data type implementing hierarchical tree structure.
 *
 */

#ifndef ADS_UNBALANCEDTREE_H
#define ADS_UNBALANCEDTREE_H

#include "utility.h"
#include "list.h"
#include "str.h"
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup tree
 * @{
 * @brief a tree is a widely used abstract data type implementing hierarchical tree structure.
 *
 * @details  a tree is a widely used abstract data type (ADT) or data structure implementing this ADT that simulates a hierarchical tree structure,
 * with a root value and subtrees of children with a parent node,
 * represented as a set of linked nodes.
 *
 * <b>
 * tree key system :
 * - key parsing :
 *      + For Example key =: "0.1.0"
 *          - by default parent = root node , the parsing start from left to right
 *          - key[0] = 0 = 0th child of root , i.e. parent.0
 *          - key[1] = . = parent = parent.0
 *          - key[2] = 1 = 1st child of the current parent( root.0th's child ) , i.e. parent.1
 *          - key[3] = . = parent = parent.1
 *          - key[4] = 0 = 0th child of the current parent( root.0th.1st's child ) ,i.e. parent.0
 *          - key-leaf = root.0th.1st.0th leaf node
 * - root key = "" (empty string)
 *      - all elements in root follow indexing
 *      - all parents are separated by "."(period) character
 *      - for example :
 *          - first element in root is accessed by "0"
 *          - second element in root is accessed by "1"
 *          - third element by "2"
 *      - now the first child of the second element of root is accessed by "1.0"
 *      - thus "1" is the parent and "0: is the child
 * </b>
 */

/**
 * @struct ads_treeLeaf tree.h <ads/tree.h>
 * @brief a internal tree node .
 */
ads_classProto(ads_treeLeaf);

/**
 * @struct tree tree.h <ads/tree.h>
 * @brief a tree is a widely used abstract data type implementing hierarchical tree structure.
 *
 * @details  a tree is a widely used abstract data type (ADT)—or data structure implementing this ADT—that simulates a hierarchical tree structure,
 * with a root value and subtrees of children with a parent node,
 * represented as a set of linked nodes.
 */
ads_classProto(ads_tree);

/** @brief ads_tree constructor.
 *
 * @param type the data-type or domain of the containing data
 * @return a ads_tree instance
 *
 */
#define ads_treeNew(type) ads_initTree(sizeof(type))

/** @cond ADS_INTERNAL */
ads_tree ads_initTree(int datatype);
void ads_treeSetCD(ads_tree self,void** (*copyConstructor) (void**),void (*destructor)(void**));
/** @endcond */

/** @brief returns a ads_treeLeaf at the current parent key
 *
 * @param self a ads_tree instance
 * @param parent a ads_string containing the parent key
 * @return a ads_treeLeaf
 *
 */
ads_treeLeaf ads_treeGetToBranch(ads_tree self,const char * parent);

/** @brief insert a value into the parent key(ads_treeLeaf) of the ads_tree
 *
 * @param self a ads_tree instance
 * @param parent the parent key
 * @param value the value to be inserted
 = @param type the container type of the ads_tree
 * @return void
 *
 */
#define ads_treeInsert(self,parent,value,type) ADS_SETITEM(ads_treeInsert_ByRef,self,parent,value,type)

/** @cond ADS_INTERNAL */
void ads_treeInsert_ByRef(ads_tree self,const char * parents,void*data);
/** @endcond */

/** @brief copies a element from the ads_tree at the key and returns it.
 *
 * @param self a ads_tree instance
 * @param key the parent.child key
 * @param type the container type of the ads_tree
 * @param result a variable where the return value is stores.
 * @return void
 *
 */
#define ads_treeGetItem(self,key,type,result) ADS_GETITEM(ads_treeGetItem_RtnRef,self,key,type,result)

/** @cond ADS_INTERNAL */
void* ads_treeGetItem_RtnRef(ads_tree self,char * parents);
/** @endcond */

/** @brief replace the value at the parent key(ads_treeLeaf) of the ads_tree
 *
 * @param self a ads_tree instance
 * @param parent the parent key
 * @param value the value to be inserted
 = @param type the container type of the ads_tree
 * @return void
 *
 */
#define ads_treeSetItem(self,parent,value,type) ADS_SETITEM(ads_treeSetItem_ByRef,self,parent,value,type)

/** @cond ADS_INTERNAL */
void ads_treeSetItem_ByRef(ads_tree self,const char * parents,void*data);
/** @endcond */

/** @brief removes a ads_treeLeaf at the current key
 *
 * @param self a ads_tree instance
 * @param parent a ads_string containing the parent key
 * @return void
 *
 */
void ads_treeDelItem(ads_tree self,const char * parents);

/** @brief printing function.
 *
 * @param self a ads_tree instance
 * @return void
 *
 */
void ads_printTree(ads_tree self,void(*printfunction)(void*));

/** @brief copy constructor.
 *
 * @param self a ads_tree instance
 * @return ads_tree instance
 *
 */
ads_tree ads_treeCopy(ads_tree _self);

/** @brief destructor.
 *
 * @param self a ads_tree instance
 * @return void
 *
 */
void ads_treeDelete( ads_tree _self);

/** @cond ADS_INTERNAL */
ads_treeLeaf ads_treeLeafNew(void*data,ads_treeLeaf parent,ads_td * pdtd);
ads_treeLeaf ads_treeLeafCopy(ads_treeLeaf _self);
void ads_treeLeafDelete(ads_treeLeaf _self);
/** @endcond */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
