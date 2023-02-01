#include <base/assert.h>
#include <base/new.h>
#include <base/vec.h>

#include <stdio.h>
#include <stdlib.h>

static void test_new__Vec() {
	Vec *v = NEW(Vec);

	ASSERT(v->buffer == NULL);
	ASSERT(v->len == 0);
	ASSERT(v->capacity == 0);
	ASSERT(v->default_capacity == 4);

	FREE(Vec, v);
}

static void test_push__Vec() {
	Vec *v = NEW(Vec);

	push__Vec(v, (int*)1);
	push__Vec(v, (int*)2);
	push__Vec(v, (int*)3);
	push__Vec(v, (int*)4);
	push__Vec(v, (int*)5);
	push__Vec(v, (int*)6);

	ASSERT(v->len == 6);
	ASSERT(v->capacity == 8);
	ASSERT(v->buffer[0] == (int*)1);
	ASSERT(v->buffer[1] == (int*)2);
	ASSERT(v->buffer[2] == (int*)3);
	ASSERT(v->buffer[3] == (int*)4);
	ASSERT(v->buffer[4] == (int*)5);
	ASSERT(v->buffer[5] == (int*)6);

	FREE(Vec, v);
}
