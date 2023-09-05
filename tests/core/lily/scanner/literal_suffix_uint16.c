#include "util.c"

#include <base/test.h>

CASE(literal_suffix_uint16, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_UINT16);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_uint16, 20);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_uint16, 255);

    FREE_SCANNER();
});
