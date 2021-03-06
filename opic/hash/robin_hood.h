/**
 * @file robin_hood.h
 * @brief A general hashmap/hashset/hashmultimap implemantation using
 * robin hood hashing.
 * @author Felix Chern
 * @date Sun Apr  2 07:09:50 2017 (-0700)
 * @copyright 2017 Felix Chern
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */

#ifndef OPIC_HASH_ROBIN_HOOD_H
#define OPIC_HASH_ROBIN_HOOD_H 1

#include <stdbool.h>
#include "opic/common/op_macros.h"
#include "op_hash.h"
#include "opic/op_malloc.h"

OP_BEGIN_DECLS

/**
 * @ingroup hash
 * @struct RobinHoodHash　
 * @brief RobinHoodHash is an opaque object that manage associations of
 * fixed length key-value pairs.
 *
 * The size of key and value is configured at the construction time of
 * RobinHoodHash, and can not be changed later. This design is similar to
 * fixed length fields `CHAR(30)` in SQL which improve both space and runtime
 * efficiencies.
 *
 * Note that user can spcify size of value to 0 to make RobinHoodHash
 * as a hash set. Similarly, make value size `sizeof(opref_t)` and
 * store a opref_t referencing another container (binary serach tree
 * or whatever), can turn RobinHoodHash into a hash-multiset.
 *
 * This object is not thread safe.
 *
 * @todo There is a wide space character "　" appended after
 * "RobinHoodHash" in this document. Somehow doxygen (1.8.13) cannot
 * process "RobinHoodHash" as a struct name. Any character edit to the
 * string works, except the form "RobinHoodHash". As a workaround, I use
 * a wide space character to finish up the document first. The real struct
 * in code does not use the wide space character.
 */
typedef struct RobinHoodHash RobinHoodHash;

/**
 * @relates RobinHoodHash　
 * @brief Constructor for RobinHoodHash.
 *
 * @param heap OPHeap instance.
 * @param rhh_ref reference to the RobinHoodHash pointer for assigining
 * RobinHoodHash instance.
 * @param num_objects number of objects we decided to put in.
 * @param load (0.0-1.0) how full the hash table could be
 * before expansion.
 * @param keysize length of key measured in bytes. Cannot be zero.
 * @param valsize length of value measured in bytes. This vlaue
 * can be zero and the hash table would work like a hash set.
 * @return true when the allocation succeeded, false otherwise.
 */
bool RHHNew(OPHeap* heap, RobinHoodHash** rhh_ref, uint64_t num_objects,
            double load, size_t keysize, size_t valsize);

/**
 * @relates RobinHoodHash　
 * @brief Destructor for RobinHoodHash.
 *
 * @param rhh the RobinHoodHash instance to destory.
 */
void RHHDestroy(RobinHoodHash* rhh);

/**
 * @relates RobinHoodHash　
 * @brief Associates the specified key with the specified value in
 * RobinHoodHash with specified hash function.
 *
 * @param rhh RobinHoodHash instance.
 * @param hasher hash function.
 * @param key pointer to the key.
 * @param val pointer to the value.
 * @return true if the operation succeeded, false otherwise.
 *
 * The content pointed by key and val will be copied into the hash table.
 * If the value size were 0, only the key get copied. When there's a
 * key collision, the coresponding value get replaced.
 *
 * If the inserted key-value pairs exceeded the original size user configured,
 * the hash table will resized with a larger capacity. If the resize failed,
 * false is returned.
 */
bool RHHPutCustom(RobinHoodHash* rhh, OPHash hasher, void* key, void* val);

/**
 * @relates RobinHoodHash　
 * @brief Obtain the value associated with the specified key and hash
 * function. Returns NULL if the key was not found.
 *
 * @param rhh RobinHoodHash instance.
 * @param hasher hash function.
 * @param key pointer to the key.
 * @return pointer to the value if found, else NULL.
 *
 * If the value size were set to 0, RHHGetCustom would still return a pointer
 * to where it would store the value. User can still use the returned value to
 * exam if the key were present in the hash table.
 */
void* RHHGetCustom(RobinHoodHash* rhh, OPHash hasher, void* key);

/**
 * @relates RobinHoodHash　
 * @brief Deletes the key-value entry in hash table with specified hasher.
 *
 * @param rhh RobinHoodHash instance.
 * @param hasher hash function.
 * @param key pointer to the key.
 * @return pointer to the value if it found, else NULL.
 *
 * The hash table may shrink if too many entries were deleted.
 */
void* RHHDeleteCustom(RobinHoodHash* rhh, OPHash hasher, void* key);

/**
 * @relates RobinHoodHash　
 * @brief Associates the specified key with the specified value in
 * RobinHoodHash using the default hash function.
 *
 * @param rhh RobinHoodHash instance.
 * @param key pointer to the key.
 * @param val pointer to the value.
 * @return true if the operation succeeded, false otherwise.
 *
 * The content pointed by key and val will be copied into the hash table.
 * If the value size were 0, only the key get copied. When there's a
 * key collision, the coresponding value get replaced.
 *
 * If the inserted key-value pairs exceeded the original size user configured,
 * the hash table will resized with a larger capacity. If the resize failed,
 * false is returned.
 */
static inline bool
RHHPut(RobinHoodHash* rhh, void* key, void* val)
{
  return RHHPutCustom(rhh, OPDefaultHash, key, val);
}

/**
 * @relates RobinHoodHash　
 * @brief Obtain the value associated with the specified key using
 * the default hash function. Returns NULL if the key was not found.
 *
 * @param rhh RobinHoodHash instance.
 * @param key pointer to the key.
 * @return pointer to the value if found, else NULL.
 *
 * If the value size were set to 0, RHHGetCustom would still return a pointer
 * to where it would store the value. User can still use the returned value to
 * exam if the key were present in the hash table.
 */
static inline void*
RHHGet(RobinHoodHash* rhh, void* key)
{
  return RHHGetCustom(rhh, OPDefaultHash, key);
}

/**
 * @relates RobinHoodHash　
 * @brief Deletes the key-value entry in hash table using the default
 * hash function.
 *
 * @param rhh RobinHoodHash instance.
 * @param key pointer to the key.
 * @return pointer to the value if it found, else NULL.
 *
 * The hash table may shrink if too many entries were deleted.
 */
static inline void*
RHHDelete(RobinHoodHash* rhh, void* key)
{
  return RHHDeleteCustom(rhh, OPDefaultHash, key);
}

/**
 * @relates RobinHoodHash　
 * @brief Obtain the number of objects stored in this hash table.
 */
uint64_t RHHObjcnt(RobinHoodHash* rhh);

/**
 * @relates RobinHoodHash　
 * @brief Obtain the number of objects can be stored in this hash table.
 */
uint64_t RHHCapacity(RobinHoodHash* rhh);

/**
 * @relates RobinHoodHash　
 * @brief Obtain the size of the key configured for this hash table.
 */
size_t RHHKeysize(RobinHoodHash* rhh);

/**
 * @relates RobinHoodHash　
 * @brief Obtain the size of the value configured for this hash table.
 */
size_t RHHValsize(RobinHoodHash* rhh);

/**
 * @relates RobinHoodHash　
 * @brief Iterates over all key-value pairs in this hash table with
 * user specified context.
 *
 * @param rhh RobinHoodHash instance.
 * @param iterator function pointer to user defined iterator function.
 * @param context user defined context.
 *
 * @code
 * // Function interface matches OPHashIterator
 * void my_iterator(void* key, void* value,
 *                  size_t keysize, size_t valsize,
 *                  void* context)
 * {
 *   // Obtain the object we passed in.
 *   struct MyStruct* my_s = (struct MyStruct*) context;
 *
 *   // assumes both key and value were null terminated string
 *   printf("key: %s, value: %s\n", key, value);
 * }
 *
 * // User defined context
 * struct MyStruct my_s;
 *
 * // RHHIterate takes in a RobinHoodHash object, a fuction pointer
 * // OPHashIterator and a user defined context for iteration.
 * RHHIterate(rhh, &my_iterator, &my_s);
 * @endcode
 */
void RHHIterate(RobinHoodHash* rhh, OPHashIterator iterator, void* context);

/**
 * @relates RobinHoodHash　
 * @brief Prints the accumulated count for each probing number.
 * @todo make this API easier to process by cilent, not just printing.
 */
void RHHPrintStat(RobinHoodHash* rhh);


OP_END_DECLS

#endif
/* robin_hood.h ends here */
