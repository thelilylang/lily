#include "util.c"

#include <base/test.h>

CASE(literal_suffix_float32, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_FLOAT32);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_float32, (Float32)3.3);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_float32, (Float32)3.333);

    FREE_SCANNER();
});
