#include "util.c"

#include <base/test.h>

CASE(literal_bytes, {
    RUN_SCANNER(FILE_LITERAL_BYTES);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_BYTES);
    TEST_ASSERT(!strcmp((const char *)NEXT()->literal_bytes, "Hello"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_BYTES);
    TEST_ASSERT(!strcmp((const char *)NEXT()->literal_bytes, "Hi"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_BYTES);
    TEST_ASSERT(!strcmp((const char *)NEXT()->literal_bytes, "H"));

    FREE_SCANNER();
});
