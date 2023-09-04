#include "util.c"

#include <base/test.h>

CASE(literal_int_16, {
    RUN_SCANNER(FILE_LITERAL_INT_16);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_16);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_16->buffer, "FF"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_16);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_16->buffer, "FF"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_16);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_16->buffer, "ABC0"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_16);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_16->buffer, "123"));

    FREE_SCANNER();
});
