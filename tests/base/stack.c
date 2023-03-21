#include <base/assert.h>
#include <base/new.h>
#include <base/stack.h>

#include <stdio.h>
#include <stdlib.h>

void
test_new__Stack()
{
    Stack *s = NEW(Stack, 1024);

    FREE(Stack, s);
}

void
test_push__Stack()
{
    Stack *s = NEW(Stack, 1024);

    push__Stack(s, (int*)10);
    push__Stack(s, (int*)20);
    push__Stack(s, (int*)30);

    FREE(Stack, s);
}

void
test_pop__Stack()
{
    Stack *s = NEW(Stack, 1024);

    push__Stack(s, (int*)10);
    push__Stack(s, (int*)20);
    push__Stack(s, (int*)30);

    ASSERT_EQ(pop__Stack(s), (int*)30);
    ASSERT_EQ(pop__Stack(s), (int*)20);
    ASSERT_EQ(pop__Stack(s), (int*)10);

    FREE(Stack, s);
}

void
test_empty__Stack()
{
    Stack *s = NEW(Stack, 1024);

    push__Stack(s, (int*)10);
    push__Stack(s, (int*)20);
    push__Stack(s, (int*)30);

    pop__Stack(s);
    pop__Stack(s);
    pop__Stack(s);

    ASSERT(empty__Stack(s));

    FREE(Stack, s);
}