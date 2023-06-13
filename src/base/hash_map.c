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

#include <stdlib.h>

static void *
get_bucket__HashMapBucket(const HashMapBucket *self, char *key);

static void
push_bucket__HashMap(HashMap *self, HashMapBucket *new);

static bool
visit_bucket__HashMap(HashMap *self, Usize index, HashMapBucket *bucket);

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

void *
get_bucket__HashMapBucket(const HashMapBucket *self, char *key)
{
    if (!strcmp(self->pair.key, key)) {
        return self->pair.value;
    }

    return self->next ? get_bucket__HashMapBucket(self->next, key) : NULL;
}

void *
get__HashMap(HashMap *self, char *key)
{
    Usize index = index__HashMap(self, key);
    HashMapBucket *bucket = self->buckets[index];

    return bucket ? get_bucket__HashMapBucket(bucket, key) : NULL;
}

void
push_bucket__HashMap(HashMap *self, HashMapBucket *new)
{
    Usize index = index__HashMap(self, new->pair.key);

    if (self->buckets[index]) {
        HashMapBucket *current = self->buckets[index];

        while (current->next) {
            current = current->next;
        }

        if (new->next) {
            if (visit_bucket__HashMap(self, index, new->next)) {
                new->next = NULL;
            }
        }

        current->next = new;

        return;
    }

    if (new->next) {
        if (visit_bucket__HashMap(self, index, new->next)) {
            new->next = NULL;
        }
    }

    self->buckets[index] = new;
}

bool
visit_bucket__HashMap(HashMap *self, Usize index, HashMapBucket *bucket)
{
    HashMapBucket *current = bucket;

    while (current->next) {
        Usize new_index = index__HashMap(self, current->pair.key);

        if (index != new_index) {
            push_bucket__HashMap(self, current);
            return true;
        }

        current = current->next;
    }

    return false;
}

void
insert__HashMap(HashMap *self, char *key, void *value)
{
    Usize index = index__HashMap(self, key);

    if (!self->buckets) {
        self->buckets = lily_calloc(DEFAULT_HASH_MAP_CAPACITY, PTR_SIZE);

        self->buckets[index] = NEW(HashMapBucket, NEW(HashMapPair, key, value));
        ++self->len;

        return;
    }

    if (self->len + 1 > self->capacity) {
        self->capacity *= 2;

        HashMapBucket **new_buckets = lily_calloc(self->capacity, PTR_SIZE);
        HashMapBucket **old_buckets = self->buckets;

        self->buckets = new_buckets;

        // Re-hash all inserted K-V
        for (Usize i = 0; i < self->len; ++i) {
            if (old_buckets[i]) {
                visit_bucket__HashMap(self, i, old_buckets[i]);
            }
        }

        lily_free(old_buckets);

        // Realod hash
        index = index__HashMap(self, key);
    }

    push_bucket__HashMap(self,
                         NEW(HashMapBucket, NEW(HashMapPair, key, value)));
    ++self->len;
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
