#include "util.c"

#include <base/test.h>

CASE(literal_int_8, {
    RUN_SCANNER(FILE_LITERAL_INT_8);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_8);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_8->buffer, "233"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_8);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_8->buffer, "223"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_INT_8);
    TEST_ASSERT(!strcmp(NEXT()->literal_int_8->buffer, "442"));

    FREE_SCANNER();
});
