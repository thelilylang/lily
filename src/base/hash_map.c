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

#include <base/hash_map.h>
#include <base/new.h>

#include <stdlib.h>

HashMap *
__new__HashMap()
{
    HashMap *hm = malloc(sizeof(HashMap));

    hm->key = NEW(Vec);
    hm->value = NEW(Vec);
    hm->len = 0;

    return hm;
}

void *
get__HashMap(HashMap *self, void *key)
{
    for (int i = 0; i < self->len; i++) {
        if (self->key->buffer[i] == key) {
            return self->value->buffer[i];
        }
    }

    return NULL;
}

void *
insert__HashMap(HashMap *self, void *key, void *value)
{
    for (int i = 0; i < self->len; i++) {
        if (self->key->buffer[i] == key) {
            void *old_value = self->value->buffer[i];

            self->value->buffer[i] = value;

            return old_value;
        }
    }

    push__Vec(self->key, key);
    push__Vec(self->value, value);
    self->len++;

    return NULL;
}

void
__free__HashMap(HashMap *self)
{
    FREE(Vec, self->key);
    FREE(Vec, self->value);
    free(self);
}
