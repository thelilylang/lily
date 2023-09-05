#include "util.c"

#include <base/test.h>

CASE(literal_suffix_usize, {
    RUN_SCANNER(FILE_LITERAL_SUFFIX_USIZE);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_usize, 30);
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE);
    TEST_ASSERT_EQ(NEXT()->literal_suffix_usize, 43981);

    FREE_SCANNER();
});
