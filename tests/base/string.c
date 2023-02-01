#include <base/assert.h>
#include <base/new.h>
#include <base/string.h>

#include <stdio.h>
#include <stdlib.h>

void test_new__String() {
	String *s = NEW(String);

	ASSERT(s->len == 0);
	ASSERT(s->capacity == 0);
	ASSERT(s->default_capacity == 4);

	FREE(String, s);
}

void test_from__String() {
	String *s = from__String("Hello");

	ASSERT(s->len == 5);
	ASSERT(s->capacity == 10);
	ASSERT(s->default_capacity == 10);

	ASSERT(s->buffer[0] == 'H');
	ASSERT(s->buffer[1] == 'e');
	ASSERT(s->buffer[2] == 'l');
	ASSERT(s->buffer[3] == 'l');
	ASSERT(s->buffer[4] == 'o');
	ASSERT(s->buffer[5] == '\0');

	FREE(String, s);
}
