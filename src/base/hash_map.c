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

#include <base/alloc.h>
#include <base/hash_map.h>
#include <base/new.h>

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

#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(HashMapPair *, HashMapPair, char *key, void *value)
{
    HashMapPair *self = lily_malloc(sizeof(HashMapPair));

    self->key = key;
    self->value = value;

    return self;
}

CONSTRUCTOR(HashMap *, HashMap)
{
    HashMap *self = lily_malloc(sizeof(HashMap));

    self->pairs = NULL;
    self->len = 0;
    self->capacity = DEFAULT_HASH_MAP_CAPACITY;

    return self;
}

void *
get__HashMap(HashMap *self, char *key)
{
    Usize hash = hash__HashMap(self, key);

    while (self->pairs[hash] != NULL) {
        if (!strcmp(self->pairs[hash]->key, key)) {
            return self->pairs[hash]->value;
        }

        hash = (hash + 1) % self->capacity;
    }

    return self->pairs[hash]->value;
}

Usize
hash__HashMap(HashMap *self, char *key)
{
#ifdef HASH_FNV1A
#ifdef PLATFORM_64
    return hash_fnv1a_64(key) % self->capacity;
#else
    return hash_fnv1a_32(key) % self->capacity;
#endif
#elif defined(HASH_CUSTOM)
#ifdef PLATFORM_64
    return hash_custom64(key) % self->capacity;
#else
    return hash_custom32(key) % self->capacity;
#endif
#elif defined(HASH_JENKINS)
    return hash_jenkins(key) % self->capacity;
#elif defined(HASH_SIP)
#ifdef PLATFORM_64
    return hash_sip(
      key, strlen(key), 0x0123456789abcdefULL, 0xfedcba9876543210ULL) % self->capacity;
#else
    return hash_sip(key, strlen(key), 0x01234567, 0x89abcdef) % self->capacity;
#endif
#else
#error "cannot generate an hash"
#endif
}

void
insert__HashMap(HashMap *self, char *key, void *value)
{
    Usize hash = hash__HashMap(self, key);

    if (!self->pairs) {
        self->pairs = lily_calloc(DEFAULT_HASH_MAP_CAPACITY, PTR_SIZE);

        self->pairs[hash] = NEW(HashMapPair, key, value);

        ++self->len;

        return;
    }

    if (self->len + 1 > self->capacity) {
        self->capacity *= 2;

        HashMapPair **new_pairs = lily_calloc(self->capacity, PTR_SIZE);

        // Re-hash all inserted K-V
        for (Usize i = 0; i < self->len; ++i) {
            if (self->pairs[i]) {
                Usize new_hash = hash__HashMap(self, self->pairs[i]->key);

                while (new_pairs[new_hash] != NULL) {
                    new_hash = (new_hash + 1) % self->capacity;
                }

                new_pairs[new_hash] = self->pairs[i];
            }
        }

        lily_free(self->pairs);

        self->pairs = new_pairs;

        // Realod hash
        hash = hash__HashMap(self, key);
    }

    while (self->pairs[hash] != NULL) {
        hash = (hash + 1) % self->capacity;
    }

    self->pairs[hash] = NEW(HashMapPair, key, value);
    ++self->len;
}

DESTRUCTOR(HashMap, HashMap *self)
{
    if (self->pairs) {
        for (Usize i = 0; i < self->capacity; ++i) {
            if (self->pairs[i]) {
                lily_free(self->pairs[i]);
            }
        }

        lily_free(self->pairs);
    }

    lily_free(self);
}
