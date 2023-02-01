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

#include "base/vec.h"
#include <base/assert.h>
#include <base/string.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

String* __new__String() {
	String *s = malloc(sizeof(String));

	s->buffer = malloc(1);
	s->buffer[0] = '\0';
	s->len = 0;
	s->capacity = 0;
	s->default_capacity = 4;

	return s;
}

String* from__String(char *buffer) {
	String *s = malloc(sizeof(String));

	s->capacity = strlen(buffer) * 2;
	s->default_capacity = s->capacity;
	s->buffer = malloc(s->capacity);
	s->len = strlen(buffer);	

	for (int i = 0; i < s->len; i++) {
		s->buffer[i] = buffer[i];
	}

	s->buffer[s->len] = '\0';

	return s;
}

char get__String(String *self, Usize index) {
	ASSERT(index < self->len);

	return self->buffer[index];
}

void grow__String(String *self, Usize new_capacity) {
	ASSERT(new_capacity >= self->capacity);

	self->buffer = realloc(self->buffer, new_capacity);
	self->capacity = new_capacity;
}

void __free__String(String *self) {
	free(self->buffer);
	free(self);
}
