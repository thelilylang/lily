#include "util.c"

#include <base/test.h>

SIMPLE(macro, {
    RUN_PREPARSER(FILE_MACRO);

    PRIVATE_MACROS_ITERATOR();
    PUBLIC_MACROS_ITERATOR();

    TEST_ASSERT(!strcmp(CURRENT_PRIVATE_MACRO()->name->buffer, "write"));
    TEST_ASSERT_EQ(CURRENT_PRIVATE_MACRO()->params->len, 1);
    TEST_ASSERT_EQ(CURRENT_PRIVATE_MACRO()->tokens->len, 15);

    TEST_ASSERT(!strcmp(CURRENT_PUBLIC_MACRO()->name->buffer, "add"));
    TEST_ASSERT_EQ(CURRENT_PUBLIC_MACRO()->params->len, 2);
    TEST_ASSERT_EQ(CURRENT_PUBLIC_MACRO()->tokens->len, 8);

    FREE_PREPARSER();
});
