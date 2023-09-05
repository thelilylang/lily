#include "util.c"

#include <base/test.h>

CASE(literal_suffix_uint8, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_UINT8);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_uint8, 200);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_uint8, 15);

    FREE_SCANNER();
});
