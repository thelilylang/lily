#include "util.c"

#include <base/test.h>

CASE(literal_suffix_uint64, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_UINT64);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_uint16, 44040);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_uint16, 43690);

    FREE_SCANNER();
});
