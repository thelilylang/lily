#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/str.h>
#include <base/string.h>
#include <base/test.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(str);

CASE(str_split, {
    char *s = "Hello\nWorld\nHi";
    Vec *split = split__Str(s, '\n');

    TEST_ASSERT(!strcmp(split->buffer[0], "Hello"));
    TEST_ASSERT(!strcmp(split->buffer[1], "World"));
    TEST_ASSERT(!strcmp(split->buffer[2], "Hi"));

    for (Usize i = 0; i < split->len; i++)
        lily_free(split->buffer[i]);

    FREE(Vec, split);
});

CASE(str_get_slice, {
    char *s = "Hello";
    char *slice = get_slice__Str(s, 0, 2);

    TEST_ASSERT(slice[0] == 'H');
    TEST_ASSERT(slice[1] == 'e');
    TEST_ASSERT(slice[2] == '\0');

    lily_free(slice);
});

CASE(str_replace, {
    String *s = from__String("Hello");

    replace__Str(s->buffer, 'l', '%');

    TEST_ASSERT(!strcmp(s->buffer, "He%%o"));

    FREE(String, s);
});

CASE(str_count_c, {
    String *s = from__String("Hello");

    TEST_ASSERT(count_c__Str(s->buffer, 'l') == 2);
    TEST_ASSERT(count_c__Str(s->buffer, 'e') == 1);
    TEST_ASSERT(count_c__Str(s->buffer, 'z') == 0);

    FREE(String, s);
});
