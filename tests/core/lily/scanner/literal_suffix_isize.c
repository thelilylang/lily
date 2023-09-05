#include "util.c"

#include <base/test.h>

CASE(literal_suffix_isize, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_ISIZE);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_isize, 33);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_isize, 255);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_isize, 1000);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_isize, 22);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_isize, 4);

    FREE_SCANNER();
});
