#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(string);

CASE(string_new, {
    String *s = NEW(String);

    TEST_ASSERT(s->len == 0);
    TEST_ASSERT(s->capacity == 0);

    FREE(String, s);
});

CASE(string_clone, {
    String *s = from__String("Hello");
    String *clone = clone__String(s);

    TEST_ASSERT(!strcmp(s->buffer, "Hello"));
    TEST_ASSERT(!strcmp(clone->buffer, "Hello"));

    push_str__String(clone, " World");

    TEST_ASSERT(!strcmp(clone->buffer, "Hello World"));
    TEST_ASSERT(!strcmp(s->buffer, "Hello"));

    FREE(String, s);
    FREE(String, clone);
});

CASE(string_from, {
    String *s = from__String("Hello");

    TEST_ASSERT(s->len == 5);
    TEST_ASSERT(s->capacity == 10);

    TEST_ASSERT(s->buffer[0] == 'H');
    TEST_ASSERT(s->buffer[1] == 'e');
    TEST_ASSERT(s->buffer[2] == 'l');
    TEST_ASSERT(s->buffer[3] == 'l');
    TEST_ASSERT(s->buffer[4] == 'o');
    TEST_ASSERT(s->buffer[5] == '\0');

    FREE(String, s);
});

CASE(string_get_slice, {
    String *s = from__String("Hello");
    char *slice = get_slice__String(s, 0, 2);

    TEST_ASSERT(slice[0] == 'H');
    TEST_ASSERT(slice[1] == 'e');

    FREE(String, s);
    lily_free(slice);
});

CASE(string_split, {
    String *s = from__String("Hello\nWorld");
    Vec *split = split__String(s, '\n');

    TEST_ASSERT(!strcmp(CAST(String *, split->buffer[0])->buffer, "Hello"));
    TEST_ASSERT(!strcmp(CAST(String *, split->buffer[1])->buffer, "World"));

    FREE(String, s);

    FREE_BUFFER_ITEMS(split->buffer, split->len, String);
    FREE(Vec, split);
});

CASE(string_pop, {
    String *s = from__String("Hello");

    TEST_ASSERT(pop__String(s) == 'o');
    TEST_ASSERT(pop__String(s) == 'l');
    TEST_ASSERT(pop__String(s) == 'l');
    TEST_ASSERT(pop__String(s) == 'e');
    TEST_ASSERT(pop__String(s) == 'H');

    FREE(String, s);
});

CASE(string_push, {
    String *s = NEW(String);

    push__String(s, 'H');
    push__String(s, 'e');
    push__String(s, 'l');
    push__String(s, 'l');
    push__String(s, 'o');

    TEST_ASSERT(s->len == 5);
    TEST_ASSERT(s->capacity == 8);

    TEST_ASSERT(s->buffer[0] == 'H');
    TEST_ASSERT(s->buffer[1] == 'e');
    TEST_ASSERT(s->buffer[2] == 'l');
    TEST_ASSERT(s->buffer[3] == 'l');
    TEST_ASSERT(s->buffer[4] == 'o');
    TEST_ASSERT(s->buffer[5] == '\0');

    FREE(String, s);
});
