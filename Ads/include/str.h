/** @file str.h
 * @brief String data structure.
 *
 * This String Data Structure is more efficient and easy to use than
 * Any c style string which is a char array.
 */



#ifndef ADS_String_H
#define ADS_String_H

#include "utility.h"
#include "vector.h"
#include "stringpool.h"

#define ads_endOfStr '\0'

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup string
 * @{
 * @brief A dynamic string class.
 * @details A string is a dynamic array of character can grow to
 * fit more elements. It is same as the c++ String Class except ads_str are immutable.
 *
 * <h4>Format Specifier</h4>
 *      - @b @%str - prints the string within quotes(\"any string\")
 *      - @b {str} - prints the string within quotes(\"any string\")
 *      - @b @%string - prints the string without quotes(any string)
 *      - @b {string} - prints the string without quotes(any string)
 *
 * <h4>Example</h4>
 *
 * Creating a string.
 * @code {.c}
 * ads_str s = ads_strNew("Abstract");
 * @endcode
 *
 * Concatenating some other string(s).
 * @code {.c}
 * ads_strCat(s," Data Structure");
 * ads_strCat(s," version %f",__ADS_VERSION__);
 * @endcode
 *
 * Copying a string.
 * @code {.c}
 * ads_str cos = ads_copy(ads_str,s);
 * @endcode
 *
 * reset a string value.
 * @code {.c}
 * ads_strSet(cos,"resetting the string value");
 * @endcode
 *
 * string Building.
 * @code {.c}
 * char tmp[ads_strLen(s)+1];
 * ads_foreach(ch,ads_strIterator(s),char)
 * {
 *      tmp[ads_enumerate(ch)] = 'Z' - toupper(ch);
 * }ads_next(ch);
 * tmp[ads_strLen(s)] = '\0';
 * ads_str bs = ads_strNew("%s",tmp);
 * @endcode
 * the ads_enumerate() function returns the index of the current character.
 *
 * printing the string to stdout.
 * @code {.c}
 * ads_printf("the new string is = %str ",s);
 * @endcode
 *
 * deleting the string.
 * @code {.c}
 * ads_delete(s); //deleting the string.
 * @endcode
 *
 */

/**
 * @struct string str.h <ads/str.h>
 * @brief A dynamic string class.
 *
 * @details A string is a dynamic array of character can grow to
 * fit more elements. It is same as the c++ String Class
 */
ads_classProto(ads_str);


extern const ads_td * const ads_strTD; /**< used To create vector of ads_str */


/** @brief String Constructor
 *
 * @param format a constant c style ads_str which will be formatter before a ads_str object is created
 * @param ... variable argument for formatting
 * @return a ads_str instance.
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_strNew("ADS");
 * ads_str s1 = ads_strNew("%s","ADS");
 * ads_str s2 = ads_strNew("ADS %d ",3.11);
 * ads_str s3 = ads_strNew("{str}",s2);
 * ads_str s4 = ads_new(ads_str, "{str}",s2);
 * @endcode
 */

ads_str ads_strNew(const char * format,...);

/** @brief get a c style string from a ads_str object.
 *
 * @param self a ads_str instance
 * @param destination a c style ads_str.
 * @return destination
 *
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * int len = ads_strLen(s);
 * char cstyle[len];
 * ads_strToCstr(s,cstyle);
 * @endcode
 *
 * if destination = NULL then
 * it allocates dynamic memory for c array and returns it
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * char * cstyle = ads_strToCstr(s,NULL);
 * ads_delete(cstyle);
 * @endcode
 */
char* ads_strToCstr(const ads_str self,char * destinaton);

/** @brief get a copy of the ads_str.
 *
 * @param self a ads_str instance
 * @return a ads_str instance.
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * ads_str copys = ads_copy(ads_str,s);
 * @endcode
 */
ads_str ads_strCopy(ads_str _self);

void* ads_strCopy_ByRef_RtnRef(const void * s);

/**< @cond ADS_INTERNAL */
void ads_sprintstring(void*add,const char * cmd,int size);
void ads_printstring(void*add,const char * cmd,int size);
void print_unicodestring(void* _self);
/**< @endcond  */

/** @brief String Destructor
 *
 * @param self a ads_str instance
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * ads_delete(s);
 * @endcode
 */
void ads_strDelete(ads_str _self);

void ads_strDelete_ByRef(void*s);

const char* ads_strGetCstr(const ads_str self);

/** @brief ads_str compare function.
 *
 * @param self a ads_str instance
 * @param format can be used to compare c style ads_str directly or can be formatted to compare ads_str object by "%str"
 * @return return strcmp result
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * ads_str s1 = ads_new(ads_str,"ads");
 * char a[] = "Ads";
 * ads_strCmp(s,a);
 * ads_strCmp(s,"{str}",s1);
 * ads_strCmp(s,"{s}",a);
 * @endcode
 */
int ads_strCmp(const ads_str self,const char* format,...);

/** @brief use to compare ads_str object directly with ads_str object
 *
 * @param self a ads_str instance
 * @param other a ads_str instance
 * @return return strcmp result
 *
 * This function can be used with other data structure for internal ads_str comparison
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * ads_str s1 = ads_new(ads_str,"ads");
 * ads_cmpString(s,s1);
 * @endcode
 */
int ads_cmpString( void* self, void* other);

/** @brief returns the amount of memory occupied by the ads_str object.
 *
 * @param self a ads_str instance
 * @return return memory in bytes
 *
 */
int ads_strSizeOf(const ads_str self);

/** @brief returns the no of characters in the ads_str
 *
 * @param self a ads_str instance
 * @return length of the ads_str
 *
 */
int ads_strLen(const ads_str self);

/** @brief replace the ads_str with the given ads_str
 *
 * @param self a ads_str instance
 * @param format a c ads_str which will be formatted before replacing
 * @param ... formatting argument
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * ads_strSet(s,"%s {s}","Abstract Data Structure","3.11");
 * @endcode
 */
void ads_strSet(ads_str self,const char * format,...);

/** @brief concatenation of the ads_str with the given ads_str
 *
 * @param self a ads_str instance
 * @param format ads_str to be concatenated
 * @param ... formatting argument
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"ADS");
 * ads_strCat(s,"Can Add anything as C-str to the ads_str");
 * ads_strCat(s,"it even can be formatted : %s %f","is in version",3.11);
 * @endcode
 */
void ads_strCat(ads_str self,const char* format,...);

/** @brief returns the character at the particular index
 *
 * @param self a ads_str instance
 * @param index character at position.
 * @return character at index
 *
 */
char ads_strCharAt(ads_str self,int index);

/** @brief copy a part of the c style ads_str and create a new ads_str with it.
 *
 * @param self a c style ads_str instance
 * @param lowerlimit the position from where the ads_str should be copied
 * @return upperlimit the position to which the ads_str should be copied
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_cstrGetSlice("FOOBAR",3,5);
 * @endcode
 */
ads_str ads_cstrGetSlice(const char* self,int lowerlimit,int upperlimit);

/** @brief copy a part of the ads_str and create a new ads_str with it.
 *
 * @param self a ads_str instance
 * @param lowerlimit the position from where the ads_str should be copied
 * @param upperlimit the position to which the ads_str should be copied
 * @return a ads_str instance
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"FooBar");
 * ads_strGetSlice(s,3,5);
 * @endcode
 */
ads_str ads_strGetSlice(const ads_str self,int lowerlimit ,int upperlimit );

/** @brief replace a part of the ads_str with the given ads_str
 *
 * @param self a ads_str instance
 * @param lowerlimit position from where the replacement start
 * @param upperlimit position from where the replacement end
 * @param format formatting text
 * @param ... formatting argument
 *
 * <h4>Example</h4>
 * @code {.c}
 * ads_str s = ads_new(ads_str,"FooBar");
 * ads_strSetSlice(s,0,2,"Dance ");
 * @endcode
 *
 */
ads_rt ads_strSetSlice(const ads_str self,int lowerlimit,int upperlimit,const char * format,... );

/** @brief Convert the first Occurrence of the character to Upper case
 *
 * @param  self a ads_str instance
 */
void ads_strCapitalize(const ads_str self);

/** @brief return the number of times a ads_str occurs in the self ads_str.
 *
 * @param self a c style ads_str instance
 * @param other a ads_str instance
 * @return Number of times a ads_str is a substring of self
 *
 */
int ads_cstrCount(const char* self,const char*other);

/** @brief return the number of times a ads_str occurs in the self ads_str.
 *
 * @param self a ads_str instance
 * @param other a ads_str instance
 * @return Number of times a ads_str is a substring of self
 *
 */
int ads_strCount(ads_str self,const char* format,...);

/** @brief find the first occurrence of the ads_str in self and return its position
 *
 * @param self a c style ads_str instance
 * @param other a ads_str instance
 * @return starting position of the substring
 *
 */
int ads_cstrFind(const char* self,const char*other);

/** @brief find the first occurrence of the ads_str in self and return its position
 *
 * @param self a ads_str instance
 * @param other a ads_str instance
 * @return starting position of the substring
 *
 */
int ads_strFind(ads_str self,const char* other,...);

/** @brief verifies weather a ads_str is a subset of self or not.
 *
 * @param self a c style ads_str instance
 * @param other a ads_str instance
 * @return true if subset else false
 *
 */
bool ads_cstrContains(const char* self,const char* other);

/** @brief verifies weather a ads_str is a subset of self or not.
 *
 * @param self a ads_str instance
 * @param other a ads_str instance
 * @return true if subset else false
 *
 */
bool ads_strContains(ads_str self,const char*other,...);

/** @brief find the first occurrence of the ads_str in self and return its position
 *
 * @param self a ads_str instance
 * @param other a ads_str instance
 * @return starting position of the substring

 * Raise Value Error if not found.
 *
 */
int ads_cstrIndex(const char*self,const char*other);

/** @brief find the first occurrence of the ads_str in self and return its position
 *
 * @param self a ads_str instance
 * @param other a ads_str instance
 * @return starting position of the substring

 * Raise Value Error if not found.
 *
 */
int ads_strIndex(ads_str self,const char* format,...);

/** @brief returns true if all character are alphanumeric
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */
bool ads_cstrIsAlnum(const char* self);

/** @brief returns true if all character are alphanumeric
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsAlnum(ads_str self);

/** @brief returns true if all character are alphabetic
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */

bool ads_cstrIsAlpha(const char* self);

/** @brief returns true if all character are alphabetic
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsAlpha(ads_str self);

/** @brief returns true if the ads_str is a floating point number
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */
bool ads_cstrIsDouble(const char* self);

/** @brief returns true if the ads_str is a floating point number
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsDouble(ads_str self);

/** @brief returns true if the ads_str is a integer number
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */
bool ads_cstrIsInteger(const char* self);

/** @brief returns true if the ads_str is a integer number
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsInteger(ads_str self);

/** @brief returns true if all the character are in upper case
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */
bool ads_cstrIsUpper(const char* self);

/** @brief returns true if all the character are in upper case
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsUpper(ads_str self);

/** @brief returns true if all the character are in lower case
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */
bool ads_cstrIsLower(const char* self);

/** @brief returns true if all the character are in lower case
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsLower(ads_str self);

/** @brief returns true if all the character are spaces
 *
 * @param self a c style ads_str instance
 * @return returns boolean
 *
 */
bool ads_cstrIsSpace(const char* self);

/** @brief returns true if all the character are spaces
 *
 * @param self a ads_str instance
 * @return returns boolean
 *
 */
bool ads_strIsSpace(ads_str self);

/** @brief convert all character in the ads_str to upper case
 *
 * @param self a ads_str instance
 *
 */
void ads_strToLower(const ads_str self);

/** @brief convert all character in the ads_str to lower case
 *
 * @param self a ads_str instance
 *
 */
void ads_strToUpper(const ads_str self);

/** @brief replace all occurrence of other with newstr
 *
 * @param self a ads_str instance
 * @param other the ads_str to find in self
 * @param newstr the ads_str to replace the other with
 */
ads_rt ads_cstrReplace(const ads_str self,const char*other,const char*newstr);

/** @brief replace all occurrence of other with newstr
 *
 * @param self a ads_str instance
 * @param other the ads_str to find in self
 * @param newstr the ads_str to replace the other with
 */
void ads_strReplace(ads_str self,ads_str other,ads_str newstr);

/** @brief remove all leading and trailing spaces from the ads_str
 *
 * @param self a ads_str instance
 *
 */
void ads_strStrip(const ads_str self);

/** @brief swap cases of each character in the ads_str
 *
 * @param self a ads_str instance
 *
 */
void ads_strSwapCase(const ads_str self);

/** @brief breaks the ads_str into pieces where it finds format characters into a ads_vector of strings
 *
 * @param str ads_str to break into
 * @param format the characters where the ads_str should be broken if found
 * @param ... variable argument for format
 * @return a ads_vector of ads_str.
 *
 */
ads_vector ads_cstrSplit(const char* str,const char* format,...);

/** @brief breaks the ads_str into pieces where it finds format characters into a ads_vector of strings
 *
 * @param str ads_str to break into
 * @param format the characters where the ads_str should be broken if found
 * @param ... variable argument for format
 * @return a ads_vector of ads_str.
 *
 */
ads_vector ads_strSplit(const ads_str self, const char *format,...);

/** @brief joins a ads_vector of strings into a single ads_str
 *
 * @param l1 ads_vector of ads_str
 * @param delimiter character to place in between two ads_str before joining them
 * @return a complete joined ads_str
 *
 */
ads_str ads_cstrJoin(ads_vector l1,const char * delimiter);

/** @brief joins a ads_vector of strings into a single ads_str
 *
 * @param l1 ads_vector of ads_str
 * @param delimiter character to place in between two ads_str before joining them
 * @return a complete joined ads_str
 *
 */
ads_str ads_strJoin(ads_vector l1,const ads_str delimiter);

/** @brief concatenate the ads_str with itself n number of times
 *
 * @param self a ads_str instance
 * @param multiplier the no of times it should be multiplied
 *
 */
ads_rt ads_strMul(const ads_str self,int multiplier);

/** @brief center the ads_str by adding fill character to the left or right
 *
 * @param self a ads_str instance
 * @param width the total width of the ads_str after centering
 * @param fill a padding character
 *
 */
void ads_strCentered(const ads_str self,int width,const char  fill );

/** @brief add zero in front of the ads_str if width > ads_str.length
 *
 * @param self a ads_str instance
 * @param width the total width of the ads_str after filling
 *
 */
void ads_strZFill(const ads_str self,int width);

/** @brief reverse the given ads_str
 *
 * @param self a ads_str instance
 *
 */
void ads_strReverse(const ads_str self);

/** @brief return true if ads_str end with the substring
 *
 * @param self a ads_str instance
 * @param substring a ads_str instance
 * @return a boolean
 */
bool ads_cstrEndWith(const char * self,const char * substring);

/** @brief return true if ads_str end with the substring
 *
 * @param self a ads_str instance
 * @param substring a ads_str instance
 * @return a boolean
 */
bool ads_strEndWith(ads_str self,const char * format,...);

/** @brief return true if ads_str start with the substring
 *
 * @param self a ads_str instance
 * @param substring a ads_str instance
 * @return a boolean
 */
bool ads_cstrStartWith(const char *self,const char * substring);

/** @brief return true if ads_str start with the substring
 *
 * @param self a ads_str instance
 * @param substring a ads_str instance
 * @return a boolean
 */
bool ads_strStartWith(ads_str self,const char * format,...);

/** @brief return hash value of the ads_str
 *
 * @param self a ads_str instance
 * @return uint64_t hash value
 */
uint64_t ads_strHash(ads_str _self);

uint64_t ads_strHash_ByRef(void** _self);

/** @brief convert a ads_str in any base to int64
 *
 * @param self a ads_str instance
 * @param base the base of the numeric ads_str value
 * @return a int64
 */
int64_t ads_cstrToInt64(const char * src ,int base); /*AnyBase(2-36) To Base 10 int conversion*/

/** @brief convert a ads_str in any base to int64
 *
 * @param self a ads_str instance
 * @param base the base of the numeric ads_str value
 * @return a int64
 */
int64_t ads_strToInt64(ads_str self,int base );       /*AnyBase(2-36) To Base 10 int conversion*/

/** @brief convert a int64 to a ads_str of any base
 *
 * @param num a int64 integer number
 * @param base the base of the numeric ads_str value
 * @return a ads_str
 */
ads_str ads_strFrmInt64(int64_t num,int base); 		/*Base 10 to AnyBase(2-36) int Converter*/

/** @brief convert a int to a ads_str of any base
 *
 * @param num a int integer number
 * @param base the base of the numeric ads_str value
 * @return a ads_str
 */
ads_str ads_strFrmInt(int num,int base );				/*Base 10 to AnyBase(2-36) int Converter*/

/** @brief convert a ads_str in any base to a double
 *
 * @param self a ads_str instance
 * @param base the base of the numeric ads_str value
 * @return a double
 */
double ads_strToDouble(const ads_str self ,int base ); /*AnyBase(2-36) To Base 10 double conversion*/

/** @brief convert a ads_str in any base to a double
 *
 * @param self a ads_str instance
 * @param base the base of the numeric ads_str value
 * @return a double
 */
double ads_cstrToDouble(const char* self ,int base ); /*AnyBase(2-36) To Base 10 double conversion*/

/** @brief convert a double to a ads_str in any base
 *
 * @param num  a double value
 * @param digit the no of significant decimal place
 * @param base the base of the numeric ads_str value
 * @return a double
 */
ads_str ads_strFrmDouble(double num ,int base,int digit); /*Base 10 to AnyBase(2-36) double Converter*/

/** @brief convert a int64 to ads_str in binary representation
 *
 * @param n int64 integer number
 * @return a ads_str instance
 */
ads_str ads_binary(int64_t n);

/** @brief convert a int64 to ads_str in hexadecimal representation
 *
 * @param n int64 integer number
 * @return a ads_str instance
 */
ads_str ads_hexadecimal(int64_t n);

/** @brief returns a ads_str iterator
 *
 * @param self a ads_str instance
 * @return a ads_str iterator
 */
ads_iterator ads_strIterator(ads_str self);

/** @brief get a ads_str input after printing the prompt to stdout
 *
 * @param self a c style ads_str instance
 * @return a ads_str
 */
ads_str ads_getLineInput(const char * prompt);

/** @brief returns a random ads_str with the given length
 *
 * @param size int the no of character in the ads_str
 * @return ads_str
 *
 */
ads_str ads_strRand(int size);

/**< @cond ADS_INTERNAL */
void* ads_strParse(const char * stream,int len);
ads_str init_newstring(const char*oldstr);
void ads_sprintString(void*add,const char * cmd,int size);
void ads_printString(void*add,const char * cmd,int size);
/**< @endcond */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
