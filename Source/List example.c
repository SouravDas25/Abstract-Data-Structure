
/**
	@file listexample.c
	@brief Main function to demonstrate the list ADS working.

	@author SD(Sourav Das)
*/

#include "Ads\List.h"

/**
	* This file helps to demonstrate the list ADS working.
	* a list is a dynamic array .
		- it an grow in size if needed (automatically).
		- it has constant access time complexity i.e. O(1).
		- but it has O(k) insertion and deletion time complexity where k is the index to insert or delete.

	* For Now This Library is only supported in C99 and above.
*/

/**
	* initializing a list and adding elements to it.
*/
void initializations()
{
	list l = newlist(int,10); ///initialize a list variable with type int and size 10
	/** l = [ ] , an empty list*/

	printlist(l,printint); /// printlist is a function for printing all elements to stdout.

	int p = 4 ;
	builtlist(l,int,1,2,3,p,p+1); ///append the list with the arguments given
	/** l = [1,2,3,4,5] */

	printlist(l,printint);

	/** OR Use a For loop to append variables */

	int k;
	for(k=6;k<=10;k++)
	{
		list_append(l,k,int); ///append or insert a single element at the end of the list.
	}
	/** l = [1,2,3,4,5,6,7,8,9,10] */

    printlist(l,printint);

	delete(l); ///free the memory occupied by the list
}

/**
	* Basic operation on list
		* 1. Retrieving values from a particular index (get)
		* 2. Overwriting a value at a particular index (set)
		* 3. Inserting a value at a particular index (insert)
		* 4. Deleting a value of a particular index (delete)
		* 5. Removing a value and Returning it (pop)
		* 6. Getting No of elements in list at present (length)
*/

void BasicOperations()
{
	list l = newlist(int,10); //! initialize a list variable with type int and size 10

	int i,index;
	for(i= 1;i<=10;i++)
	{
        list_append(l,i,int);
	}
	/** l = [1,2,3,4,5,6,7,8,9,10] */

	/// Retrieving the value at index.
    index = 2;  ///index of the list start from 0 to (length of the list - 1)

    int p = list_getitem(l,index,int);
    /** p = 3 , as p =  l[2] ,correct type of the element should be supplied for proper casting */

	///Overwriting a value at a index
	index = 8;

	list_setitem(l,index,p,int);
	/** l[8] = p , l = [1,2,3,4,5,6,7,8,3,10] */

    ///Inserting a value at a index
    index = 5 ;
    p = 55;

    list_insert(l,index,p,int); /// insert 55 at index 5
    /** l = [1,2,3,4,5,55,6,7,8,3,10] */

    /// Deleting a value of a index
    index = 0;

    list_delitem(l,index);
    /** l = [2,3,4,5,55,6,7,8,3,10] */

    ///Removing a value and Returning it

    p = list_pop(l,int); ///Removes the first element from the list and returns it.
    /** p = 2 , l = [3,4,5,55,6,7,8,3,10] */

    ///Getting No of elements in list at present

    int len = list_len(l); ///returns the current no of item in list
}

/**
	* Additional operation on list
		* 1. Importing all values from another list (extend)
		* 2. Reversing the current arrangement of list (reverse)
		* 3. Reduce the memory occupied by the list (shrink)
		* 4. Removing all elements from the list (clear)
		* 5. Find the index position of a value (index)
		* 6. Counting the no of times a value appear in list (count)
		* 7. Remove all occurrence of a value from list (remove)
		* 8. Arrange a all the element in the list in a particular order (sort)
		* 9. Perform a efficient binary search on the list to find a element (binary search)
        * 10. Loop through all the element on the list one by one (iteration)
*/

void AdditionalOperations()
{
	list l = newlist(int,10); //! initialize a list variable with type int and size 10

	list l2 = newlist(int,10);

	builtlist(l,int,1,2,3,4,5,6,7,8,9,10);
	/** l = [1,2,3,4,5,6,7,8,9,10] */

	int sum = 0;
    ///Iterate Over all the element in the list.
    foreach(i, list_iterator(l),int) 		/// for-each loop are only available with ADS DataType.
    {
        sum += i; 			/** for-each loop takes 3 argument (loop variable, iterator object , return type)*/
		list_append(l2,i,int);					/** every ADS DataType has its own iterator function which return a iterator over itself */

    }next(i); 	///call next(loop variable) after the for-each loop is compulsory or for-each wont work

    /** OR we can use a for loop to loop through the list */

    int i;
    for(i = 0;i < list_len(l);i++)
	{
        sum += list_getitem(l,i,int);
		list_append(l2,list_getitem(l,i,int),int);
	}

    ///Importing all values from another list.

    list_extend(l,l2); /// appends all element of l2 to l

    /** l = [1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10] , */
    ///Reversing the current arrangement of list.

    list_reverse(l);
    /** l = [10,9,8,7,6,5,4,3,2,1,10,9,8,7,6,5,4,3,2,1] */

    ///Removing all elements from the list.

    list_clear(l);
    /** l = [ ] */

    ///Find the index position of a value.

    int p = 9;

    int index = list_index(l,compareint,p,int);
    /** index = 1 , l = [10,9,8,7,6,5,4,3,2,1,10,9,8,7,6,5,4,3,2,1] */

    ///Counting the no of times a value appear in list.

    int count = list_count(l,compareint,p,int);
    /** count = 2 , l = [10,9,8,7,6,5,4,3,2,1,10,9,8,7,6,5,4,3,2,1] */

    ///Remove all occurrence of a value from list.

    list_remove(l,compareint,p,int);
    /** l = [10,8,7,6,5,4,3,2,1,10,8,7,6,5,4,3,2,1] */

    ///Arrange a all the element in the list in a particular order.

    list_sort(l,compareint);
    /** l = [1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,10,10] */

    ///Perform a efficient binary search on the list to find a element.

    index = list_binarysearch(l,compareint,p,int);

    /**index = -1 , l = [1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,10,10] */

}

