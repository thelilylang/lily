#include <base/assert.h>
#include <base/new.h>
#include <base/test.h>
#include <base/vec.h>

#include <stdio.h>
#include <stdlib.h>

SUITE(vec);

CASE(vec_init, {
    Vec *v = init__Vec(3, (int *)0, (int *)1, (int *)2);

    TEST_ASSERT(v->len == 3);
    TEST_ASSERT(v->capacity == 4);

    TEST_ASSERT(v->buffer[0] == (int *)0);
    TEST_ASSERT(v->buffer[1] == (int *)1);
    TEST_ASSERT(v->buffer[2] == (int *)2);

    FREE(Vec, v);
});

CASE(vec_new, {
    Vec *v = NEW(Vec);

    TEST_ASSERT(v->buffer == NULL);
    TEST_ASSERT(v->len == 0);
    TEST_ASSERT(v->capacity == 0);
    TEST_ASSERT(v->default_capacity == 4);

    FREE(Vec, v);
});

CASE(vec_from, {
    int *buffer[5] = ARRAY((int *)0, (int *)1, (int *)2, (int *)3, (int *)4);
    Vec *v = from__Vec((void **)buffer, 5);

    TEST_ASSERT(v->buffer != NULL);
    TEST_ASSERT(v->len == 5);
    TEST_ASSERT(v->capacity == 10);
    TEST_ASSERT(v->default_capacity == 5);

    for (Usize i = 5; --i;) {
        TEST_ASSERT(v->buffer[i] == buffer[i]);
    }

    FREE(Vec, v);
});

CASE(vec_get, {
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);

    TEST_ASSERT(get__Vec(v, 0) == (int *)1);

    FREE(Vec, v);
});

CASE(vec_insert, {
    Vec *v = init__Vec(4, (int *)1, (int *)2, (int *)4, (int *)5);

    insert__Vec(v, (int *)3, 2);

    TEST_ASSERT(v->buffer[0] == (int *)1);
    TEST_ASSERT(v->buffer[1] == (int *)2);
    TEST_ASSERT(v->buffer[2] == (int *)3);
    TEST_ASSERT(v->buffer[3] == (int *)4);
    TEST_ASSERT(v->buffer[4] == (int *)5);

    FREE(Vec, v);
});

CASE(vec_insert_after, {
    Vec *v = init__Vec(4, (int *)1, (int *)2, (int *)4, (int *)5);

    insert_after__Vec(v, (int *)3, 2);

    TEST_ASSERT(v->buffer[0] == (int *)1);
    TEST_ASSERT(v->buffer[1] == (int *)2);
    TEST_ASSERT(v->buffer[2] == (int *)4);
    TEST_ASSERT(v->buffer[3] == (int *)3);
    TEST_ASSERT(v->buffer[4] == (int *)5);

    FREE(Vec, v);
});

CASE(vec_pop, {
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);

    TEST_ASSERT(pop__Vec(v) == (int *)3);
    TEST_ASSERT(pop__Vec(v) == (int *)2);
    TEST_ASSERT(pop__Vec(v) == (int *)1);

    FREE(Vec, v);
});

CASE(vec_push, {
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);
    push__Vec(v, (int *)4);
    push__Vec(v, (int *)5);
    push__Vec(v, (int *)6);

    TEST_ASSERT(v->len == 6);
    TEST_ASSERT(v->capacity == 8);
    TEST_ASSERT(v->buffer[0] == (int *)1);
    TEST_ASSERT(v->buffer[1] == (int *)2);
    TEST_ASSERT(v->buffer[2] == (int *)3);
    TEST_ASSERT(v->buffer[3] == (int *)4);
    TEST_ASSERT(v->buffer[4] == (int *)5);
    TEST_ASSERT(v->buffer[5] == (int *)6);

    FREE(Vec, v);
});

CASE(vec_remove, {
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);
    push__Vec(v, (int *)4);
    push__Vec(v, (int *)5);
    push__Vec(v, (int *)6);

    TEST_ASSERT(remove__Vec(v, 3) == (int *)4);

    TEST_ASSERT(v->len == 5);
    TEST_ASSERT(v->buffer[0] == (int *)1);
    TEST_ASSERT(v->buffer[1] == (int *)2);
    TEST_ASSERT(v->buffer[2] == (int *)3);
    TEST_ASSERT(v->buffer[3] == (int *)5);
    TEST_ASSERT(v->buffer[4] == (int *)6);

    TEST_ASSERT(remove__Vec(v, 0) == (int *)1);

    TEST_ASSERT(v->len == 4);
    TEST_ASSERT(v->buffer[0] == (int *)2);
    TEST_ASSERT(v->buffer[1] == (int *)3);
    TEST_ASSERT(v->buffer[2] == (int *)5);
    TEST_ASSERT(v->buffer[3] == (int *)6);

    FREE(Vec, v);
});

CASE(vec_reverse, {
    Vec *v = NEW(Vec);

    push__Vec(v, (int *)1);
    push__Vec(v, (int *)2);
    push__Vec(v, (int *)3);

    reverse__Vec(v);

    TEST_ASSERT(v->buffer[0] == (int *)3);
    TEST_ASSERT(v->buffer[1] == (int *)2);
    TEST_ASSERT(v->buffer[2] == (int *)1);

    FREE(Vec, v);
});
