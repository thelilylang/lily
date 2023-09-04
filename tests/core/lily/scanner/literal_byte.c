#include "util.c"

#include <base/test.h>

CASE(literal_byte, {
    RUN_SCANNER(FILE_LITERAL_BYTE);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_BYTE);
    TEST_ASSERT_EQ(NEXT()->literal_byte, 'a');
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_BYTE);
    TEST_ASSERT_EQ(NEXT()->literal_byte, 'b');
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_BYTE);
    TEST_ASSERT_EQ(NEXT()->literal_byte, 'c');

    FREE_SCANNER();
});
