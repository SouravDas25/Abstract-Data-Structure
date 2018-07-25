Abstract-Data-Structure
=======================
A Generic Data Structure Library in C.

Abstract Data Structure Library is created to give a solution to generic data structure in C. Everybody is fimiliar with Type-Problem in C i.e. a common task cannot be applied to all datatypes in a single function. Which is of course taken care by c++ through templating and function overloading. 

But in Ads it is taken care of by preprocessors and conventional programming technique.Though There are limitations to what Ads can achive. 
Generic Programming is the main feature of this library one Data Structure can be used to store any datatype even user defined types. 
The Semi-Automatic Garbage Collector and StaticDB is inbuilt with the Ads Library with lots of c++ feature in c. 
e.g.

Data Structure ADT's (Type Independent) ::

```c
ads_vector v = ads_new(ads_vector,int,2);  //creating a new vector.
ads_vectorAppend(v,25,int);  //inserting values to the vector.
ads_vectorAppend(v,30,int);  
ads_vectorAppend(v,35,int); //reallocation occurred
int j;
ads_vectorGetItem(v,0,int,j); //getting values from vector.
ads_delete(v); //deleting the vector.
```
     
Foreach loops and Iterators :: 

```c
// ITERATING OVER EACH VECTOR ITEM.
ads_foreach(item , ads_vectorIterator(v1) , int)
{
       sum += item;
}ads_next(item);
```
     
New Style Output Formatting :: 

        ads_printf("printing Vector = %[d] ",v); //printing vectors directly.
        ads_printf("printing Vector = {[d]} ",v); //new style formatting vectors directly.
        
* Object marshalling
* DataBase Support
* Struct based Objects
* Limited OOP concepts
* And Many More


Installation 
-------------
* Step 1: Download the library.
* Step 2: extract it.
* Step 3: Open the libads.cpb with code blocks.
* Step 4: built the project in your system and the library is ready.
* Step 5: start using the library.


warning
-------
Improper use can crash your application.
