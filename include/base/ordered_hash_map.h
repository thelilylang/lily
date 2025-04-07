/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_BASE_ORDERED_HASH_MAP_H
#define LILY_BASE_ORDERED_HASH_MAP_H

#include <base/hash_choice.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>
#include <base/types.h>

#include <stdarg.h>
#include <string.h>

#ifdef PLATFORM_64
#define HASH_MAP_SIZE INT64_MAX
#else
#define HASH_MAP_SIZE INT32_MAX
#endif

#define DEFAULT_ORDERED_HASH_MAP_CAPACITY 8

#define FREE_ORD_HASHMAP_VALUES(self, type)                       \
    if (self->buckets) {                                          \
        for (Usize i = 0; i < self->capacity; ++i) {              \
            if (self->buckets[i]) {                               \
                OrderedHashMapBucket *current = self->buckets[i]; \
                while (current->next) {                           \
                    FREE(type, current->pair.value);              \
                    current = current->next;                      \
                }                                                 \
                FREE(type, current->pair.value);                  \
            }                                                     \
        }                                                         \
    }

#ifdef ENV_DEBUG
#define PRINT_STRING_ORD_HASH_MAP(self, debug)                   \
    {                                                            \
        OrderedHashMapIter iter = NEW(OrderedHashMapIter, self); \
        void *current = NULL;                                    \
                                                                 \
        while ((current = next__OrderedHashMapIter(&iter))) {    \
            PRINTLN("{Sr}", debug(current));                     \
        }                                                        \
    }

#define DEBUG_ORD_HASH_MAP_STRING(self, str, type)               \
    {                                                            \
        push_str__String(str, " { ");                            \
        OrderedHashMapIter iter = NEW(OrderedHashMapIter, self); \
        void *current = NULL;                                    \
                                                                 \
        while ((current = next__OrderedHashMapIter(&iter))) {    \
            String *s = to_string__Debug__##type(current);       \
            APPEND_AND_FREE(str, s);                             \
        }                                                        \
        push_str__String(str, " }");                             \
    }
#endif

typedef struct OrderedHashMapPair
{
    char *key;
    void *value;
    Usize id;
} OrderedHashMapPair;

/**
 *
 * @brief Construct OrderedHashMapPair type.
 */
inline CONSTRUCTOR(OrderedHashMapPair,
                   OrderedHashMapPair,
                   char *key,
                   void *value,
                   Usize id)
{
    return (OrderedHashMapPair){ .key = key, .value = value, .id = id };
}

typedef struct OrderedHashMapBucket
{
    OrderedHashMapPair pair;
    struct OrderedHashMapBucket *next; // OrederedHashMapBucket*?
} OrderedHashMapBucket;

/**
 *
 * @brief Construct OrderedHashMapBucket type.
 */
CONSTRUCTOR(OrderedHashMapBucket *,
            OrderedHashMapBucket,
            OrderedHashMapPair pair);

/**
 *
 * @brief Free OrderedHashMapBucket type.
 */
DESTRUCTOR(OrderedHashMapBucket, OrderedHashMapBucket *self);

typedef struct OrderedHashMap
{
    OrderedHashMapBucket **buckets; // OrderedHashMapBucket**?
    Usize len;
    Usize capacity;
} OrderedHashMap;

/**
 *
 * @brief Construct OrderedHashMap type.
 */
CONSTRUCTOR(OrderedHashMap *, OrderedHashMap);

/**
 *
 * @brief Get value by key.
 * @return If the key does not exist, return NULL.
 */
void *
get__OrderedHashMap(OrderedHashMap *self, char *key);

/**
 *
 * @brief Get the id from the pair.
 */
const Usize *
get_id__OrderedHashMap(OrderedHashMap *self, char *key);

/**
 *
 * @brief Get value from id.
 * @return If the key does not exist, return NULL.
 */
void *
get_from_id__OrderedHashMap(OrderedHashMap *self, Usize id);

/**
 *
 * @brief Get pair (key-value) from id.
 * @return OrderedHashMapPair*? (&)
 */
OrderedHashMapPair *
get_pair_from_id__OrderedHashMap(OrderedHashMap *self, Usize id);

/**
 *
 * @brief Generate an hash.
 */
inline Usize
hash__OrderedHashMap(OrderedHashMap *self, char *key)
{
#ifdef HASH_FNV1A
#ifdef PLATFORM_64
    return hash_fnv1a_64(key);
#else
    return hash_fnv1a_32(key);
#endif
#elif defined(HASH_CUSTOM)
#ifdef PLATFORM_64
    return hash_custom64(key);
#else
    return hash_custom32(key);
#endif
#elif defined(HASH_JENKINS)
    return hash_jenkins(key);
#elif defined(HASH_SIP)
    return hash_sip(key, strlen(key), SIP_K0, SIP_K1);
#else
#error "cannot generate an hash"
#endif
}

/**
 *
 * @brief Get index from the hash.
 */
inline Usize
index__OrderedHashMap(OrderedHashMap *self, char *key)
{
    return hash__OrderedHashMap(self, key) % self->capacity;
}

typedef struct OrderedHashMapInitPair
{
    char *key;
    void *value;
} OrderedHashMapInitPair;

/**
 *
 * @brief Initialize OrderedHashMap with `...` parameter.
 * @param ... struct OrderedHashMapInitPair
 */
OrderedHashMap *
init__OrderedHashMap(Usize count, ...);

/**
 *
 * @brief Alternative version of the `init__OrderedHashMap` function, to pass
 * the argument directly.
 */
OrderedHashMap *
vinit__OrderedHashMap(Usize count, va_list arg);

/**
 *
 * @brief Insert key-value pair into OrderedHashMap.
 * @return If the key already exists, return the value of the key, otherwise
 * return NULL.
 */
void *
insert__OrderedHashMap(OrderedHashMap *self, char *key, void *value);

/**
 *
 * @brief Get the last item from the OrderedHashMap.
 */
void *
last__OrderedHashMap(OrderedHashMap *self);

/**
 *
 * @brief Free OrderedHashMap type.
 */
DESTRUCTOR(OrderedHashMap, OrderedHashMap *self);

typedef struct OrderedHashMapIterPair
{
    char *key;   // char*? (&)
    void *value; // void*? (&)
} OrderedHashMapIterPair;

/**
 *
 * @brief Construct OrderedHashMapIterPair type.
 */
inline CONSTRUCTOR(OrderedHashMapIterPair,
                   OrderedHashMapIterPair,
                   char *key,
                   void *value)
{
    return (OrderedHashMapIterPair){ .key = key, .value = value };
}

#define ORD_HASH_MAP_ITER_PAIR_NULL() NEW(OrderedHashMapIterPair, NULL, NULL)
#define ORD_HASH_MAP_ITER_PAIR_IS_NULL(_self) \
    ({                                        \
        OrderedHashMapIterPair _pair = _self; \
        (!(_pair).key || !(_pair).value);     \
    })

typedef struct OrderedHashMapIter
{
    OrderedHashMap *ordered_hash_map;
    Usize count;
} OrderedHashMapIter;

/**
 *
 * @brief Construct OrderedHashMapIter type.
 */
inline CONSTRUCTOR(OrderedHashMapIter,
                   OrderedHashMapIter,
                   OrderedHashMap *ordered_hash_map)
{
    return (OrderedHashMapIter){ .ordered_hash_map = ordered_hash_map,
                                 .count = 0 };
}

/**
 *
 * @brief Get the next value.
 */
void *
next__OrderedHashMapIter(OrderedHashMapIter *self);

/**
 *
 * @brief Get the next pair value.
 */
OrderedHashMapIterPair
next_pair__OrderedHashMapIter(OrderedHashMapIter *self);

typedef struct OrderedHashMapIter2
{
    OrderedHashMap *ordered_hash_map;
    OrderedHashMap *ordered_hash_map2;
    void *current[2];
    Usize count;
} OrderedHashMapIter2;

/**
 *
 * @brief Construct OrderedHashMapIter2 type.
 */
CONSTRUCTOR(OrderedHashMapIter2,
            OrderedHashMapIter2,
            OrderedHashMap *ordered_hash_map,
            OrderedHashMap *ordered_hash_map2);

/**
 *
 * @brief Get the next value.
 * @return [value, value2]
 */
void **
next__OrderedHashMapIter2(OrderedHashMapIter2 *self);

#endif // LILY_BASE_ORDERED_HASH_MAP_H
