#include <stdlib.h>
#include <stdio.h>

#include <base/assert.h>
#include <base/vec.h>

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
	v->buffer = malloc(8 * v->capacity);

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
	if (new_capacity <= self->capacity) return;

	self->buffer = realloc(self->buffer, 8 * new_capacity);
	self->capacity = new_capacity;
}

void push__Vec(Vec *self, void *item) {
	if (!self->capacity)
		grow__Vec(self, self->default_capacity);
	else if (self->len == self->capacity)
		grow__Vec(self, self->capacity * 2);

	self->buffer[self->len++] = item;
}

void ungrow__Vec(Vec *self, Usize new_capacity) {
	if (new_capacity >= self->capacity) return;

	self->buffer = realloc(self->buffer, 8 * new_capacity);
	self->capacity = new_capacity;
}

void __free__Vec(Vec *self) {
	free(self->buffer);
	free(self);
}
