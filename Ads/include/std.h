
#ifndef ADSSTD_H
#define ADSSTD_H

#ifdef __cplusplus
extern "C" {
#endif


/** @mainpage Abstract Data Structure.
 *
 * @section intro_sec Introduction
 * - It is a type independent data structure library and utility.
 * - all type independent function and structures are made with the help of macro and typeless pointer.
 * - It also brings you some cool c++ structures.
 * - Like :
 *      - foreach loop
 *      - new function
 *      - delete functions
 *      - Iterators.
 * This library also features Garbage Collections.
 * which can be used to free variable automatically.
 *
 * @section dnlod_sec Download
 * - <http://dasemporium.esy.es/downloads/ads.zip>
 *
 * @section install_sec Installation
 *
 * - Step 1: Download the library.
 * - Step 2: extract it.
 * - Step 3: copy the libads.a,libads32.a file to lib folder inside any c compiler folder.
 * - Step 4: copy the ADS folder to the include folder inside any c compiler folder.
 * - Step 5: start using the library.
 *
 * @section aut_sec Credentials
 * @author    Sourav Das
 * @version   1.2
 * @date      2015-2017
 * @warning   Improper use can crash your application
 */


#include "vector.h"
#include "Avl.h"
#include "Deque.h"
#include "Dict.h"
#include "Extrie.h"
#include "Heap.h"
#include "Queue.h"
#include "Set.h"
#include "Stack.h"
#include "str.h"
#include "utility\timeit.h"
#include "trie.h"
#include "Tuple.h"

#ifdef __cplusplus
}
#endif
#endif
