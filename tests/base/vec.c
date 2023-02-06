#include <base/assert.h>
#include <base/new.h>
#include <base/vec.h>

#include <stdio.h>
#include <stdlib.h>

void
test_init__Vec()
{
    Vec *v = init__Vec(3, (int*)0, (int*)1, (int*)2);

    ASSERT(v->len == 3);
    ASSERT(v->capacity == 4);

    ASSERT(v->buffer[0] == (int*)0);
    ASSERT(v->buffer[1] == (int*)1);
    ASSERT(v->buffer[2] == (int*)2);

    FREE(Vec, v);
}

void
test_new__Vec()
{
    Vec *v = NEW(Vec);

    ASSERT(v->buffer == NULL);
    ASSERT(v->len == 0);
    ASSERT(v->capacity == 0);
    ASSERT(v->default_capacity == 4);

    FREE(Vec, v);
}

void
test_from__Vec()
{
    int *buffer[5] = { (int *)0, (int *)1, (int *)2, (int *)3, (int *)4 };
    Vec *v = from__Vec((void **)buffer, 5);

    ASSERT(v->buffer != NULL);
    ASSERT(v->len == 5);
    ASSERT(v->capacity == 10);
    ASSERT(v->default_capacity == 5);

    for (Usize i = 5; i--;) {
        ASSERT(v->buffer[i] == buffer[i]);
    }

    FREE(Vec, v);
}

void
test_get__Vec()
{
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);

    ASSERT(get__Vec(v, 0) == (int *)1);

    FREE(Vec, v);
}

void
test_pop__Vec()
{
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);

    ASSERT(pop__Vec(v) == (int *)3);
    ASSERT(pop__Vec(v) == (int *)2);
    ASSERT(pop__Vec(v) == (int *)1);

    FREE(Vec, v);
}

void
test_push__Vec()
{
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);
    push__Vec(v, (int *)4);
    push__Vec(v, (int *)5);
    push__Vec(v, (int *)6);

    ASSERT(v->len == 6);
    ASSERT(v->capacity == 8);
    ASSERT(v->buffer[0] == (int *)1);
    ASSERT(v->buffer[1] == (int *)2);
    ASSERT(v->buffer[2] == (int *)3);
    ASSERT(v->buffer[3] == (int *)4);
    ASSERT(v->buffer[4] == (int *)5);
    ASSERT(v->buffer[5] == (int *)6);

    FREE(Vec, v);
}

void
test_remove__Vec()
{
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);
    push__Vec(v, (int *)4);
    push__Vec(v, (int *)5);
    push__Vec(v, (int *)6);

    ASSERT(remove__Vec(v, 3) == (int *)4);
    ;

    ASSERT(v->len == 5);
    ASSERT(v->buffer[0] == (int *)1);
    ASSERT(v->buffer[1] == (int *)2);
    ASSERT(v->buffer[2] == (int *)3);
    ASSERT(v->buffer[3] == (int *)5);
    ASSERT(v->buffer[4] == (int *)6);

    ASSERT(remove__Vec(v, 0) == (int *)1);

    ASSERT(v->len == 4);
    ASSERT(v->buffer[0] == (int *)2);
    ASSERT(v->buffer[1] == (int *)3);
    ASSERT(v->buffer[2] == (int *)5);
    ASSERT(v->buffer[3] == (int *)6);

    FREE(Vec, v);
}

void
test_reverse__Vec()
{
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);

    reverse__Vec(v);

    ASSERT(v->buffer[0] == (int *)3);
    ASSERT(v->buffer[1] == (int *)2);
    ASSERT(v->buffer[2] == (int *)1);

    FREE(Vec, v);
}
