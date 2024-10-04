#include <base/assert.h>
#include <base/new.h>
#include <base/string.h>
#include <base/test.h>
#include <base/vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(vec);

CASE(vec_append, {
    Vec *v1 = init__Vec(3, (int *)0, (int *)1, (int *)2);
    Vec *v2 = init__Vec(3, (int *)3, (int *)4, (int *)5);

    append__Vec(v1, v2);

    TEST_ASSERT(v1->len == 6);

    TEST_ASSERT(v1->buffer[0] == (int *)0);
    TEST_ASSERT(v1->buffer[1] == (int *)1);
    TEST_ASSERT(v1->buffer[2] == (int *)2);
    TEST_ASSERT(v1->buffer[3] == (int *)3);
    TEST_ASSERT(v1->buffer[4] == (int *)4);
    TEST_ASSERT(v1->buffer[5] == (int *)5);

    FREE(Vec, v1);
    FREE(Vec, v2);
});

CASE(vec_contains_found_case, {
    Vec *v =
      init__Vec(3, from__String("A"), from__String("B"), from__String("C"));

    {
        String *to_found = from__String("B");

        TEST_ASSERT(contains__Vec(v, to_found));

        FREE(String, to_found);
    }

    FREE_BUFFER_ITEMS(v->buffer, v->len, String);
    FREE(Vec, v);
});

CASE(vec_contains_not_found_case, {
    Vec *v =
      init__Vec(3, from__String("A"), from__String("B"), from__String("C"));

    {
        String *not_found = from__String("D");

        TEST_ASSERT(!contains__Vec(v, not_found));

        FREE(String, not_found);
    }

    FREE_BUFFER_ITEMS(v->buffer, v->len, String);
    FREE(Vec, v);
});

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

CASE(vec_join, {
    Vec *v =
      init__Vec(3, from__String("A"), from__String("B"), from__String("C"));
    String *s = join__Vec(v, ',');

    TEST_ASSERT(!strcmp(s->buffer, "A,B,C"));

    FREE_BUFFER_ITEMS(v->buffer, v->len, String);
    FREE(Vec, v);
    FREE(String, s);
});

CASE(vec_last, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);

    TEST_ASSERT(last__Vec(v) == (int *)3);

    FREE(Vec, v);
});

CASE(vec_safe_last, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);

    TEST_ASSERT(safe_last__Vec(v) == (int *)3);

    // Remove all items
    pop__Vec(v);
    pop__Vec(v);
    pop__Vec(v);

    TEST_ASSERT(!safe_last__Vec(v));

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

CASE(vec_replace, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);

    TEST_ASSERT(v->buffer[0] == (int *)1);
    TEST_ASSERT(v->buffer[1] == (int *)2);
    TEST_ASSERT(v->buffer[2] == (int *)3);

    replace__Vec(v, 0, (int *)4);

    TEST_ASSERT(v->buffer[0] == (int *)4);

    replace__Vec(v, 1, (int *)5);

    TEST_ASSERT(v->buffer[1] == (int *)5);

    replace__Vec(v, 2, (int *)6);

    TEST_ASSERT(v->buffer[2] == (int *)6);

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

CASE(vec_safe_get, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);

    TEST_ASSERT(safe_get__Vec(v, 0) == (int *)1);
    TEST_ASSERT(safe_get__Vec(v, 1) == (int *)2);
    TEST_ASSERT(safe_get__Vec(v, 2) == (int *)3);
    TEST_ASSERT(!safe_get__Vec(v, 3));

    FREE(Vec, v);
});

CASE(vec_slice, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);
    Vec *v_sliced = slice__Vec(v, 1, 2);

    TEST_ASSERT(v_sliced->len == 2);

    TEST_ASSERT(v_sliced->buffer[0] == (int *)2);
    TEST_ASSERT(v_sliced->buffer[1] == (int *)3);

    FREE(Vec, v);
    FREE(Vec, v_sliced);
});

SUITE(vec_iter);

CASE(vec_iter_next, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);
    VecIter iter = NEW(VecIter, v);
    int *current;
    bool enter_in_loop = false;
    int i = 1;

    while ((current = next__VecIter(&iter))) {
        enter_in_loop = true;
        TEST_ASSERT(current == (int *)(Uptr)i++);
    }

    TEST_ASSERT(enter_in_loop);

    FREE(Vec, v);
});

CASE(vec_iter_current, {
    Vec *v = init__Vec(3, (int *)1, (int *)2, (int *)3);
    VecIter iter = NEW(VecIter, v);

    TEST_ASSERT(current__VecIter(&iter) == (int *)1);

    next__VecIter(&iter);

    TEST_ASSERT(current__VecIter(&iter) == (int *)2);

    next__VecIter(&iter);

    TEST_ASSERT(current__VecIter(&iter) == (int *)3);

    next__VecIter(&iter);

    TEST_ASSERT(!current__VecIter(&iter));

    FREE(Vec, v);
});
