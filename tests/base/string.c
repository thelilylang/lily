#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_new__String()
{
    String *s = NEW(String);

    ASSERT(s->len == 0);
    ASSERT(s->capacity == 0);

    FREE(String, s);
}

void
test_clone__String()
{
    String *s = from__String("Hello");
    String *clone = clone__String(s);

    ASSERT(!strcmp(s->buffer, "Hello"));
    ASSERT(!strcmp(clone->buffer, "Hello"));

    push_str__String(clone, " World");

    ASSERT(!strcmp(clone->buffer, "Hello World"));
    ASSERT(!strcmp(s->buffer, "Hello"));

    FREE(String, s);
    FREE(String, clone);
}

void
test_from__String()
{
    String *s = from__String("Hello");

    ASSERT(s->len == 5);
    ASSERT(s->capacity == 10);

    ASSERT(s->buffer[0] == 'H');
    ASSERT(s->buffer[1] == 'e');
    ASSERT(s->buffer[2] == 'l');
    ASSERT(s->buffer[3] == 'l');
    ASSERT(s->buffer[4] == 'o');
    ASSERT(s->buffer[5] == '\0');

    FREE(String, s);
}

void
test_split__String()
{
    String *s = from__String("Hello\nWorld");
    Vec *split = split__String(s, '\n');

    ASSERT(!strcmp(CAST(String *, split->buffer[0])->buffer, "Hello"));
    ASSERT(!strcmp(CAST(String *, split->buffer[1])->buffer, "World"));

    FREE(String, s);

    FREE_BUFFER_ITEMS(split->buffer, split->len, String);
    FREE(Vec, split);
}

void
test_pop__String()
{
    String *s = from__String("Hello");

    ASSERT(pop__String(s) == 'o');
    ASSERT(pop__String(s) == 'l');
    ASSERT(pop__String(s) == 'l');
    ASSERT(pop__String(s) == 'e');
    ASSERT(pop__String(s) == 'H');

    FREE(String, s);
}

void
test_push__String()
{
    String *s = NEW(String);

    push__String(s, 'H');
    push__String(s, 'e');
    push__String(s, 'l');
    push__String(s, 'l');
    push__String(s, 'o');

    ASSERT(s->len == 5);
    ASSERT(s->capacity == 8);

    ASSERT(s->buffer[0] == 'H');
    ASSERT(s->buffer[1] == 'e');
    ASSERT(s->buffer[2] == 'l');
    ASSERT(s->buffer[3] == 'l');
    ASSERT(s->buffer[4] == 'o');
    ASSERT(s->buffer[5] == '\0');

    FREE(String, s);
}
