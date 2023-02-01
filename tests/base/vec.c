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

	ASSERT(v->len == 3);
	ASSERT(v->capacity == 4);
	ASSERT(get__Vec(v, 0) == (int*)1);
	ASSERT(get__Vec(v, 1) == (int*)2);
	ASSERT(get__Vec(v, 2) == (int*)3);

	FREE(Vec, v);
}
