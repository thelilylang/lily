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

CONSTRUCTOR(HashMap *, HashMap)
{
    HashMap *self = malloc(sizeof(HashMap));

    self->keys = NEW(Vec);
    self->values = NEW(Vec);
    self->len = 0;

    return self;
}

void *
get__HashMap(HashMap *self, void *key)
{
    for (int i = 0; i < self->len; i++) {
        if (self->keys->buffer[i] == key) {
            return self->values->buffer[i];
        }
    }

    return NULL;
}

void *
insert__HashMap(HashMap *self, void *key, void *value)
{
    for (int i = 0; i < self->len; i++) {
        if (self->keys->buffer[i] == key) {
            void *old_value = self->values->buffer[i];

            self->values->buffer[i] = value;

            return old_value;
        }
    }

    push__Vec(self->keys, key);
    push__Vec(self->values, value);
    self->len++;

    return NULL;
}

DESTRUCTOR(HashMap, HashMap *self)
{
    FREE(Vec, self->keys);
    FREE(Vec, self->values);
    free(self);
}
