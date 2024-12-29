/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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
#include <base/assert.h>
#include <base/ordered_hash_map.h>

#include <stdio.h>
#include <stdlib.h>

static const OrderedHashMapBucket *
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

const OrderedHashMapBucket *
get_bucket__OrderedHashMapBucket(const OrderedHashMapBucket *self, char *key)
{
    if (!strcmp(self->pair.key, key)) {
        return self;
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

    if (bucket) {
        const OrderedHashMapBucket *res =
          get_bucket__OrderedHashMapBucket(bucket, key);

        return res ? res->pair.value : NULL;
    }

    return NULL;
}

const Usize *
get_id__OrderedHashMap(OrderedHashMap *self, char *key)
{
    if (!self->buckets)
        return NULL;

    Usize index = index__OrderedHashMap(self, key);
    OrderedHashMapBucket *bucket = self->buckets[index];

    if (bucket) {
        const OrderedHashMapBucket *res =
          get_bucket__OrderedHashMapBucket(bucket, key);

        return res ? &res->pair.id : NULL;
    }

    return NULL;
}

void *
get_from_id__OrderedHashMap(OrderedHashMap *self, Usize id)
{
    OrderedHashMapPair *pair = get_pair_from_id__OrderedHashMap(self, id);

    if (pair) {
        return pair->value;
    }

    return NULL;
}

OrderedHashMapPair *
get_pair_from_id__OrderedHashMap(OrderedHashMap *self, Usize id)
{
    if (id > self->capacity || !self->buckets)
        return NULL;

    for (Usize i = 0; i < self->capacity; ++i) {
        OrderedHashMapBucket *current_bucket = self->buckets[i];

        if (current_bucket) {
            while (current_bucket->next) {
                if (current_bucket->pair.id == id) {
                    return &current_bucket->pair;
                }

                current_bucket = current_bucket->next;
            }

            if (current_bucket->pair.id == id) {
                return &current_bucket->pair;
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

    if (is_exist) {
        FREE(OrderedHashMapBucket, new);

        return is_exist;
    }

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

OrderedHashMap *
init__OrderedHashMap(Usize count, ...)
{
    va_list arg;

    va_start(arg, count);

    OrderedHashMap *res = vinit__OrderedHashMap(count, arg);

    va_end(arg);

    return res;
}

OrderedHashMap *
vinit__OrderedHashMap(Usize count, va_list arg)
{
    OrderedHashMap *self = NEW(OrderedHashMap);

    for (Usize i = 0; i < count; ++i) {
        OrderedHashMapInitPair pair = va_arg(arg, OrderedHashMapInitPair);

        if (insert__OrderedHashMap(self, pair.key, pair.value)) {
            UNREACHABLE(
              "duplicate key in initialization of the OrderedHashMap");
        }
    }

    return self;
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

void *
last__OrderedHashMap(OrderedHashMap *self)
{
    ASSERT(self->len > 0);

    return get_from_id__OrderedHashMap(self, self->len - 1);
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

CONSTRUCTOR(OrderedHashMapIter2,
            OrderedHashMapIter2,
            OrderedHashMap *ordered_hash_map,
            OrderedHashMap *ordered_hash_map2)
{
    ASSERT(ordered_hash_map->len == ordered_hash_map2->len);

    return (OrderedHashMapIter2){ .ordered_hash_map = ordered_hash_map,
                                  .ordered_hash_map2 = ordered_hash_map2,
                                  .current = { 0 },
                                  .count = 0 };
}

void **
next__OrderedHashMapIter2(OrderedHashMapIter2 *self)
{
    self->current[0] =
      get_from_id__OrderedHashMap(self->ordered_hash_map, self->count);
    self->current[1] =
      get_from_id__OrderedHashMap(self->ordered_hash_map2, self->count++);

    // Verify only the first value, because the both map have the same length
    return *self->current ? self->current : NULL;
}
