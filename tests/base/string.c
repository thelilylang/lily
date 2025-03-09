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

CASE(replace_sub, {
    String *s = from__String("Hello X");

    replace_sub__String(s, "X", "World!");

    TEST_ASSERT(s->len == 12);

    TEST_ASSERT(s->buffer[0] == 'H');
    TEST_ASSERT(s->buffer[1] == 'e');
    TEST_ASSERT(s->buffer[2] == 'l');
    TEST_ASSERT(s->buffer[3] == 'l');
    TEST_ASSERT(s->buffer[4] == 'o');
    TEST_ASSERT(s->buffer[5] == ' ');
    TEST_ASSERT(s->buffer[6] == 'W');
    TEST_ASSERT(s->buffer[7] == 'o');
    TEST_ASSERT(s->buffer[8] == 'r');
    TEST_ASSERT(s->buffer[9] == 'l');
    TEST_ASSERT(s->buffer[10] == 'd');
    TEST_ASSERT(s->buffer[11] == '!');

    FREE(String, s);
});

CASE(replace_sub2, {
    String *s = from__String("X + 1 = 2; X + X = 2");

    replace_sub__String(s, "X", "1");

    TEST_ASSERT(s->len == 20);

    TEST_ASSERT(s->buffer[0] == '1');
    TEST_ASSERT(s->buffer[1] == ' ');
    TEST_ASSERT(s->buffer[2] == '+');
    TEST_ASSERT(s->buffer[3] == ' ');
    TEST_ASSERT(s->buffer[4] == '1');
    TEST_ASSERT(s->buffer[5] == ' ');
    TEST_ASSERT(s->buffer[6] == '=');
    TEST_ASSERT(s->buffer[7] == ' ');
    TEST_ASSERT(s->buffer[8] == '2');
    TEST_ASSERT(s->buffer[9] == ';');
    TEST_ASSERT(s->buffer[10] == ' ');
    TEST_ASSERT(s->buffer[11] == '1');
    TEST_ASSERT(s->buffer[12] == ' ');
    TEST_ASSERT(s->buffer[13] == '+');
    TEST_ASSERT(s->buffer[14] == ' ');
    TEST_ASSERT(s->buffer[15] == '1');
    TEST_ASSERT(s->buffer[16] == ' ');
    TEST_ASSERT(s->buffer[17] == '=');
    TEST_ASSERT(s->buffer[18] == ' ');
    TEST_ASSERT(s->buffer[19] == '2');

    FREE(String, s);
});

CASE(replace_sub3, {
    String *s = from__String("H\\nI");

    replace_sub__String(s, "\\n", "\n");

    TEST_ASSERT(s->len == 3);

    TEST_ASSERT(s->buffer[0] == 'H');
    TEST_ASSERT(s->buffer[1] == '\n');
    TEST_ASSERT(s->buffer[2] == 'I');

    FREE(String, s);
});

CASE(replace_sub4, {
    String *s = from__String("H\nI");

    replace_sub__String(s, "\n", "\\n");

    TEST_ASSERT(s->len == 4);

    TEST_ASSERT(s->buffer[0] == 'H');
    TEST_ASSERT(s->buffer[1] == '\\');
    TEST_ASSERT(s->buffer[2] == 'n');
    TEST_ASSERT(s->buffer[3] == 'I');

    FREE(String, s);
});

CASE(replace_sub5, {
    String *s = from__String("1:2:3\nPASS\n\n");

    replace_sub__String(s, "\n", "\\n");

    TEST_ASSERT(s->len == 15);

    TEST_ASSERT(s->buffer[0] == '1');
    TEST_ASSERT(s->buffer[1] == ':');
    TEST_ASSERT(s->buffer[2] == '2');
    TEST_ASSERT(s->buffer[3] == ':');
    TEST_ASSERT(s->buffer[4] == '3');
    TEST_ASSERT(s->buffer[5] == '\\');
    TEST_ASSERT(s->buffer[6] == 'n');
    TEST_ASSERT(s->buffer[7] == 'P');
    TEST_ASSERT(s->buffer[8] == 'A');
    TEST_ASSERT(s->buffer[9] == 'S');
    TEST_ASSERT(s->buffer[10] == 'S');
    TEST_ASSERT(s->buffer[11] == '\\');
    TEST_ASSERT(s->buffer[12] == 'n');
    TEST_ASSERT(s->buffer[13] == '\\');
    TEST_ASSERT(s->buffer[14] == 'n');

    FREE(String, s);
});
