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
#include <base/ordered_hash_map.h>

#include <stdlib.h>

static void *
get_bucket__OrderedHashMapBucket(const OrderedHashMapBucket *self, char *key);

static void *
push_bucket__OrderedHashMap(OrderedHashMap *self,
                            Usize index,
                            OrderedHashMapBucket *new);

CONSTRUCTOR(OrderedHashMapBucket *,
            OrderedHashMapBucket,
            OrderedHashMapPair pair)
{
    OrderedHashMapBucket *self = lily_malloc(sizeof(OrderedHashMapBucket));

    self->pair = pair;
    self->next = NULL;

    return self;
}

DESTRUCTOR(OrderedHashMapBucket, OrderedHashMapBucket *self)
{
    if (self->next) {
        FREE(OrderedHashMapBucket, self->next);
    }

    lily_free(self);
}

CONSTRUCTOR(OrderedHashMap *, OrderedHashMap)
{
    OrderedHashMap *self = lily_malloc(sizeof(OrderedHashMap));

    self->buckets = NULL;
    self->len = 0;
    self->capacity = DEFAULT_ORDERED_HASH_MAP_CAPACITY;

    return self;
}

void *
get_bucket__OrderedHashMapBucket(const OrderedHashMapBucket *self, char *key)
{
    if (!strcmp(self->pair.key, key)) {
        return self->pair.value;
    }

    return self->next ? get_bucket__OrderedHashMapBucket(self->next, key)
                      : NULL;
}

void *
get__OrderedHashMap(OrderedHashMap *self, char *key)
{
	if (!self->buckets)
		return NULL;

    Usize index = index__OrderedHashMap(self, key);
    OrderedHashMapBucket *bucket = self->buckets[index];

    return bucket ? get_bucket__OrderedHashMapBucket(bucket, key) : NULL;
}

void *
get_from_id__OrderedHashMap(OrderedHashMap *self, Usize id)
{
    for (Usize i = 0; i < self->capacity; ++i) {
        OrderedHashMapBucket *current_bucket = self->buckets[i];

        if (current_bucket) {
            while (current_bucket->next) {
                if (current_bucket->pair.id == id) {
                    return current_bucket->pair.value;
                }

                current_bucket = self->buckets[i]->next;
            }

            if (current_bucket->pair.id == id) {
                return current_bucket->pair.value;
            }
        }
    }

    return NULL;
}

void *
push_bucket__OrderedHashMap(OrderedHashMap *self,
                            Usize index,
                            OrderedHashMapBucket *new)
{
    void *is_exist = get__OrderedHashMap(self, new->pair.key);

    if (is_exist)
        return is_exist;

    if (self->buckets[index]) {
        OrderedHashMapBucket *current = self->buckets[index];

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
insert__OrderedHashMap(OrderedHashMap *self, char *key, void *value)
{
    Usize index = index__OrderedHashMap(self, key);

    if (!self->buckets) {
        self->buckets =
          lily_calloc(DEFAULT_ORDERED_HASH_MAP_CAPACITY, PTR_SIZE);
        self->buckets[index] =
          NEW(OrderedHashMapBucket,
              NEW(OrderedHashMapPair, key, value, self->len++));

        return NULL;
    }

    if (self->len + 1 > self->capacity) {
        self->capacity *= 2;

        OrderedHashMapBucket **new_buckets =
          lily_calloc(self->capacity, PTR_SIZE);
        OrderedHashMapBucket **old_buckets = self->buckets;

        self->buckets = new_buckets;

        // Re-hash all inserted K-V
        for (Usize i = 0; i < self->len; ++i) {
            OrderedHashMapBucket *current = old_buckets[i];

            while (current != NULL) {
                OrderedHashMapBucket *next = current->next;
                Usize new_index =
                  index__OrderedHashMap(self, current->pair.key);

                current->next = new_buckets[new_index];
                new_buckets[new_index] = current;
                current = next;
            }
        }

        lily_free(old_buckets);

        // Reload index
        index = index__OrderedHashMap(self, key);
    }

    void *is_exist = push_bucket__OrderedHashMap(
      self,
      index,
      NEW(OrderedHashMapBucket,
          NEW(OrderedHashMapPair, key, value, self->len)));

    if (is_exist) {
        return is_exist;
    }

    ++self->len;

    return NULL;
}

DESTRUCTOR(OrderedHashMap, OrderedHashMap *self)
{
    if (self->buckets) {
        for (Usize i = 0; i < self->capacity; ++i) {
            if (self->buckets[i]) {
                FREE(OrderedHashMapBucket, self->buckets[i]);
            }
        }

        lily_free(self->buckets);
    }

    lily_free(self);
}

void *
next__OrderedHashMapIter(OrderedHashMapIter *self)
{
    return get_from_id__OrderedHashMap(self->ordered_hash_map, self->count++);
}
