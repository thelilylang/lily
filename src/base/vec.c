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

#include <stdlib.h>
#include <stdio.h>

#include <base/assert.h>
#include <base/vec.h>
#include <base/macros.h>

Vec* __new__Vec() {
	Vec* v = malloc(sizeof(struct Vec));

	v->buffer = NULL;
	v->len = 0;
	v->capacity = 0;
	v->default_capacity = 4;

	return v;
}

Vec* from__Vec(void **buffer, Usize len) {
	Vec* v = __new__Vec();

	v->default_capacity = len;
	v->len = len;
	v->capacity = len * 2;
	v->buffer = malloc(PTR_SIZE * v->capacity);

	for (Usize i = len; i--;) {
		v->buffer[i] = buffer[i];
	}

	return v;
}

void* get__Vec(Vec *self, Usize index) {
	ASSERT(index < self->len);

	return self->buffer[index];
}

void grow__Vec(Vec *self, Usize new_capacity) {
	ASSERT(new_capacity >= self->capacity);

	self->buffer = realloc(self->buffer, PTR_SIZE * new_capacity);
	self->capacity = new_capacity;
}

void* pop__Vec(Vec *self) {
	ASSERT(self->len > 0);

	void *item = self->buffer[--self->len];

	ungrow__Vec(self);

	return item;
}

void push__Vec(Vec *self, void *item) {
	if (!self->capacity)
		grow__Vec(self, self->default_capacity);
	else if (self->len == self->capacity)
		grow__Vec(self, self->capacity * 2);

	self->buffer[self->len++] = item;
}

void* remove__Vec(Vec *self, Usize index) {
	ASSERT(index < self->len);

	void* item = self->buffer[index];
	self->len -= 1;

	// Align the rest of the buffer
	for (Usize i = index; i < self->len; i++) {
		self->buffer[i] = self->buffer[i + 1];
	}

	ungrow__Vec(self);

	return item;
}

void reverse__Vec(Vec *self) {
	Usize i = 0;
	Usize j = self->len - 1;

	while (i < j) {
		void* tmp = self->buffer[i];
		self->buffer[i] = self->buffer[j];
		self->buffer[j] = tmp;

		i++;
		j--;
	}
}

void ungrow__Vec(Vec *self) {
	if (self->len <= self->capacity / 2) {
		self->capacity /= 2;
		self->buffer = realloc(self->buffer, PTR_SIZE * self->capacity);
	}
}

void __free__Vec(Vec *self) {
	free(self->buffer);
	free(self);
}
