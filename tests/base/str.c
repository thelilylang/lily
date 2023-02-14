#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/str.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_split__Str()
{
    char *s = "Hello\nWorld\nHi";
    Vec *split = split__Str(s, '\n');

    ASSERT(!strcmp(split->buffer[0], "Hello"));
    ASSERT(!strcmp(split->buffer[1], "World"));
    ASSERT(!strcmp(split->buffer[2], "Hi"));

    for (Usize i = 0; i < split->len; i++)
        lily_free(split->buffer[i]);

    FREE(Vec, split);
}