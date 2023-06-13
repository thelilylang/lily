/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#include <base/platform.h>
#include <base/types.h>
#include <base/vec.h>

#include <string.h>

#ifdef PLATFORM_64
#define HASH_MAP_SIZE INT64_MAX
#else
#define HASH_MAP_SIZE INT32_MAX
#endif

#define DEFAULT_HASH_MAP_CAPACITY 8

// Choice different algorithm to create an hash
#define HASH_FNV1A
#undef HASH_FNV1A

#define HASH_CUSTOM
#undef HASH_CUSTOM

#define HASH_JENKINS
#undef HASH_JENKINS

#define HASH_SIP
// #undef HASH_SIP

#ifdef HASH_FNV1A
#include <base/hash/fnv.h>
#elif defined(HASH_CUSTOM)
#include <base/hash/custom.h>
#elif defined(HASH_JENKINS)
#include <base/hash/jenkins.h>
#elif defined(HASH_SIP)
#include <base/hash/sip.h>
#else
#error "cannot generate an hash"
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
#ifdef PLATFORM_64
    return hash_sip(
      key, strlen(key), 0x0123456789abcdefULL, 0xfedcba9876543210ULL);
#else
    return hash_sip(key, strlen(key), 0x01234567, 0x89abcdef);
#endif
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
void
insert__HashMap(HashMap *self, char *key, void *value);

/**
 *
 * @brief Free HashMap type.
 */
DESTRUCTOR(HashMap, HashMap *self);

#endif // LILY_BASE_HASH_MAP_H
