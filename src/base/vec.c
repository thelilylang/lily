#include <stdlib.h>

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

void grow__Vec(Vec *self, Usize new_capacity) {
	if (new_capacity <= self->capacity) return;

	self->buffer = realloc(self->buffer, 8 * new_capacity);
	self->capacity = new_capacity;
}

void __free__Vec(Vec *self) {
	free(self->buffer);
	free(self);
}
