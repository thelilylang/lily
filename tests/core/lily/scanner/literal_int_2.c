#include "util.c"

#include <base/test.h>

CASE(literal_int_2, {
    RUN_SCANNER(FILE_LITERAL_INT_2);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_2);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_2->buffer, "1000"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_2);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_2->buffer, "101"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_2);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_2->buffer, "101"));

    FREE_SCANNER();
});
