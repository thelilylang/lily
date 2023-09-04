#include "util.c"

#include <base/test.h>

CASE(literal_suffix_int16, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_INT16);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int16, 323);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int16, 255);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_int16, 2);

    FREE_SCANNER();
});
