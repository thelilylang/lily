#include "util.c"

#include <base/test.h>

CASE(literal_suffix_float64, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_FLOAT64);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_float64, (Float64)0.22);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_float64, (Float64)3.3);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_float64, (Float64)222.33);

    FREE_SCANNER();
});
