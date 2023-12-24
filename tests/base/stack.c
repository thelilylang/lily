#include <base/assert.h>
#include <base/new.h>
#include <base/stack.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>

SUITE(stack);

CASE(stack_new, {
    Stack *s = NEW(Stack, 1024);

    FREE(Stack, s);
});

CASE(stack_push, {
    Stack *s = NEW(Stack, 1024);

    push__Stack(s, (int *)10);
    push__Stack(s, (int *)20);
    push__Stack(s, (int *)30);

    FREE(Stack, s);
});

CASE(stack_pop, {
    Stack *s = NEW(Stack, 1024);

    push__Stack(s, (int *)10);
    push__Stack(s, (int *)20);
    push__Stack(s, (int *)30);

    TEST_ASSERT_EQ(pop__Stack(s), (int *)30);
    TEST_ASSERT_EQ(pop__Stack(s), (int *)20);
    TEST_ASSERT_EQ(pop__Stack(s), (int *)10);

    FREE(Stack, s);
});

CASE(stack_empty, {
    Stack *s = NEW(Stack, 1024);

    push__Stack(s, (int *)10);
    push__Stack(s, (int *)20);
    push__Stack(s, (int *)30);

    pop__Stack(s);
    pop__Stack(s);
    pop__Stack(s);

    TEST_ASSERT(empty__Stack(s));

    FREE(Stack, s);
});