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

#ifndef LILY_BASE_HASH_MAP_H
#define LILY_BASE_HASH_MAP_H

#include <base/hash_choice.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>
#include <base/types.h>

#include <string.h>

#ifdef PLATFORM_64
#define HASH_MAP_SIZE INT64_MAX
#else
#define HASH_MAP_SIZE INT32_MAX
#endif

#define DEFAULT_HASH_MAP_CAPACITY 8

#define FREE_HASHMAP_VALUES(self, type)                    \
    if (self->buckets) {                                   \
        for (Usize i = 0; i < self->capacity; ++i) {       \
            if (self->buckets[i]) {                        \
                HashMapBucket *current = self->buckets[i]; \
                while (current->next) {                    \
                    FREE(type, current->pair.value);       \
                    current = current->next;               \
                }                                          \
                FREE(type, current->pair.value);           \
            }                                              \
        }                                                  \
    }

#ifdef ENV_DEBUG
#define PRINT_STRING_HASH_MAP(self, debug)             \
    {                                                  \
        HashMapIter iter = NEW(HashMapIter, self);     \
        void *current = NULL;                          \
                                                       \
        while ((current = next__HashMapIter(&iter))) { \
            PRINTLN("{Sr}", debug(current));           \
        }                                              \
    }

#define DEBUG_HASH_MAP_STRING(self, str, type)             \
    {                                                      \
        push_str__String(str, " { ");                      \
        HashMapIter iter = NEW(HashMapIter, self);         \
        void *current = NULL;                              \
                                                           \
        while ((current = next__HashMapIter(&iter))) {     \
            String *s = to_string__Debug__##type(current); \
            APPEND_AND_FREE(str, s);                       \
        }                                                  \
        push_str__String(str, " }");                       \
    }

#define DEBUG_HASH_MAP_STR(self, str, type)              \
    {                                                    \
        push_str__String(str, " { ");                    \
        HashMapIter iter = NEW(HashMapIter, self);       \
        void *current = NULL;                            \
                                                         \
        while ((current = next__HashMapIter(&iter))) {   \
            char *s = to_string__Debug__##type(current); \
            PUSH_STR_AND_FREE(str, s);                   \
        }                                                \
        push_str__String(str, " }");                     \
    }
#endif

typedef struct HashMapPair
{
    char *key;
    void *value;
} HashMapPair;

/**
 *
 * @brief Construct HashMapPair type.
 */
inline CONSTRUCTOR(HashMapPair, HashMapPair, char *key, void *value)
{
    return (HashMapPair){ .key = key, .value = value };
}

typedef struct HashMapBucket
{
    HashMapPair pair;
    struct HashMapBucket *next; // HashMapBucket*?
} HashMapBucket;

/**
 *
 * @brief Construct HashMapBucket type.
 */
CONSTRUCTOR(HashMapBucket *, HashMapBucket, HashMapPair pair);

/**
 *
 * @brief Free HashMapBucket type.
 */
DESTRUCTOR(HashMapBucket, HashMapBucket *self);

typedef struct HashMap
{
    HashMapBucket **buckets; // HashMapBucket**?
    Usize len;
    Usize capacity;
} HashMap;

/**
 *
 * @brief Construct HashMap type.
 */
CONSTRUCTOR(HashMap *, HashMap);

/**
 *
 * @brief Get value by key.
 * @return If the key does not exist, return NULL.
 */
void *
get__HashMap(HashMap *self, char *key);

/**
 *
 * @brief Generate an hash.
 */
inline Usize
hash__HashMap(HashMap *self, char *key)
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
index__HashMap(HashMap *self, char *key)
{
    return hash__HashMap(self, key) % self->capacity;
}

/**
 *
 * @brief Insert key-value pair into HashMap.
 * @return If the key already exists, return the value of the key, otherwise
 * return NULL.
 */
void *
insert__HashMap(HashMap *self, char *key, void *value);

/**
 *
 * @brief Remove a pair from a key.
 * @return void*?
 */
void *
remove__HashMap(HashMap *self, char *key);

/**
 *
 * @brief Free HashMap type.
 */
DESTRUCTOR(HashMap, HashMap *self);

typedef struct HashMapIterPair
{
    char *key;   // char*? (&)
    void *value; // void*? (&)
} HashMapIterPair;

/**
 *
 * @brief Construct HashMapIterPair type.
 */
inline CONSTRUCTOR(HashMapIterPair, HashMapIterPair, char *key, void *value)
{
    return (HashMapIterPair){ .key = key, .value = value };
}

#define HASH_MAP_ITER_PAIR_NULL() NEW(HashMapIterPair, NULL, NULL)
#define HASH_MAP_ITER_PAIR_IS_NULL(_self) \
    ({                                    \
        HashMapIterPair _pair = _self;    \
        (!(_pair).key || !(_pair).value); \
    })

typedef struct HashMapIter
{
    HashMap *hash_map;
    HashMapBucket *current;
    Usize count;
} HashMapIter;

/**
 *
 * @brief Construct HashMapIter type.
 */
inline CONSTRUCTOR(HashMapIter, HashMapIter, HashMap *hash_map)
{
    return (HashMapIter){ .hash_map = hash_map, .current = NULL, .count = 0 };
}

/**
 *
 * @brief Get the next value.
 */
void *
next__HashMapIter(HashMapIter *self);

/**
 *
 * @brief Get the next pair.
 */
HashMapIterPair
next_pair__HashMapIter(HashMapIter *self);

#endif // LILY_BASE_HASH_MAP_H
