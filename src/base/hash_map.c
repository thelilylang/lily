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

#include <base/alloc.h>
#include <base/hash_map.h>

#include <stdlib.h>

static const HashMapBucket *
get_bucket__HashMapBucket(const HashMapBucket *self, char *key);

static void *
push_bucket__HashMap(HashMap *self, Usize index, HashMapBucket *new);

CONSTRUCTOR(HashMapBucket *, HashMapBucket, HashMapPair pair)
{
    HashMapBucket *self = lily_malloc(sizeof(HashMapBucket));

    self->pair = pair;
    self->next = NULL;

    return self;
}

DESTRUCTOR(HashMapBucket, HashMapBucket *self)
{
    if (self->next) {
        FREE(HashMapBucket, self->next);
    }

    lily_free(self);
}

CONSTRUCTOR(HashMap *, HashMap)
{
    HashMap *self = lily_malloc(sizeof(HashMap));

    self->buckets = NULL;
    self->len = 0;
    self->capacity = DEFAULT_HASH_MAP_CAPACITY;

    return self;
}

const HashMapBucket *
get_bucket__HashMapBucket(const HashMapBucket *self, char *key)
{
    if (!strcmp(self->pair.key, key)) {
        return self;
    }

    return self->next ? get_bucket__HashMapBucket(self->next, key) : NULL;
}

void *
get__HashMap(HashMap *self, char *key)
{
    if (!self->buckets)
        return NULL;

    Usize index = index__HashMap(self, key);
    HashMapBucket *bucket = self->buckets[index];

    if (bucket) {
        const HashMapBucket *res = get_bucket__HashMapBucket(bucket, key);

        return res ? res->pair.value : NULL;
    }

    return NULL;
}

void *
push_bucket__HashMap(HashMap *self, Usize index, HashMapBucket *new)
{
    void *is_exist = get__HashMap(self, new->pair.key);

    if (is_exist) {
        FREE(HashMapBucket, new);

        return is_exist;
    }

    if (self->buckets[index]) {
        HashMapBucket *current = self->buckets[index];

        while (current->next) {
            current = current->next;
        }

        current->next = new;

        return NULL;
    }

    self->buckets[index] = new;

    return NULL;
}

void *
insert__HashMap(HashMap *self, char *key, void *value)
{
    Usize index = index__HashMap(self, key);

    if (!self->buckets) {
        self->buckets = lily_calloc(DEFAULT_HASH_MAP_CAPACITY, PTR_SIZE);

        self->buckets[index] = NEW(HashMapBucket, NEW(HashMapPair, key, value));
        ++self->len;

        return NULL;
    }

    if (self->len + 1 > self->capacity) {
        self->capacity *= 2;

        HashMapBucket **new_buckets = lily_calloc(self->capacity, PTR_SIZE);
        HashMapBucket **old_buckets = self->buckets;

        self->buckets = new_buckets;

        // Re-hash all inserted K-V
        for (Usize i = 0; i < self->len; ++i) {
            HashMapBucket *current = old_buckets[i];

            while (current != NULL) {
                HashMapBucket *next = current->next;
                Usize new_index = index__HashMap(self, current->pair.key);

                current->next = new_buckets[new_index];
                new_buckets[new_index] = current;
                current = next;
            }
        }

        lily_free(old_buckets);

        // Realod index
        index = index__HashMap(self, key);
    }

    void *is_exist = push_bucket__HashMap(
      self, index, NEW(HashMapBucket, NEW(HashMapPair, key, value)));

    if (is_exist) {
        return is_exist;
    }

    ++self->len;

    return NULL;
}

void *
remove__HashMap(HashMap *self, char *key)
{
    if (!self->buckets) {
        return NULL;
    }

    Usize index = index__HashMap(self, key);
    HashMapBucket *current = self->buckets[index];
    HashMapBucket *match = NULL;
    HashMapBucket *prev = NULL;

    while (current) {
        if (!strcmp(current->pair.key, key)) {
            match = current;
            break;
        }

        prev = current;
        current = current->next;
    }

    if (match) {
        void *res = match->pair.value;

        if (prev) {
            prev->next = match->next;

            lily_free(match);
        } else {
            self->buckets[index] = match->next;

            lily_free(match);
        }

        --self->len;

        return res;
    }

    return NULL;
}

DESTRUCTOR(HashMap, HashMap *self)
{
    if (self->buckets) {
        for (Usize i = 0; i < self->capacity; ++i) {
            if (self->buckets[i]) {
                FREE(HashMapBucket, self->buckets[i]);
            }
        }

        lily_free(self->buckets);
    }

    lily_free(self);
}

void *
next__HashMapIter(HashMapIter *self)
{
    if (!self->hash_map->buckets) {
        return NULL;
    }

    for (; !self->current && self->count < self->hash_map->capacity;
         self->current = self->hash_map->buckets[self->count++])
        ;

    if (self->current) {
        void *tmp = self->current->pair.value;

        self->current = self->current->next;

        return tmp;
    } else {
        return NULL;
    }
}
