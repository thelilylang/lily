#include "util.c"

#include <base/test.h>

CASE(literal_suffix_int8, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_INT8);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int8, 12);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int8, 10);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int8, 1);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int8, 0);

    FREE_SCANNER();
});
