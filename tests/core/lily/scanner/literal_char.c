#include "util.c"

#include <base/test.h>

CASE(literal_char, {
    RUN_SCANNER(FILE_LITERAL_CHAR);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CHAR);
    TEST_ASSERT_EQ(NEXT()->literal_byte, 'a');
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CHAR);
    TEST_ASSERT_EQ(NEXT()->literal_byte, 'b');
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CHAR);
    TEST_ASSERT_EQ(NEXT()->literal_byte, 'c');
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CHAR);
    TEST_ASSERT_EQ(NEXT()->literal_byte, '\n');
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CHAR);
    TEST_ASSERT_EQ(NEXT()->literal_byte, '\t');

    FREE_SCANNER();
});
