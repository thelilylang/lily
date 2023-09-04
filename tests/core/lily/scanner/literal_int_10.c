#include "util.c"

#include <base/test.h>

CASE(literal_int_10, {
    RUN_SCANNER(FILE_LITERAL_INT_10);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_10);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_10->buffer, "1"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_10);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_10->buffer, "20"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_10);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_10->buffer, "50"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_10);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_10->buffer, "5"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_10);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_10->buffer, "3"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_10);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_10->buffer, "0"));

    FREE_SCANNER();
});
