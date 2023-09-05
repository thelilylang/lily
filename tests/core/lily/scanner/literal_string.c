#include "util.c"

#include <base/test.h>

CASE(literal_string, {
    RUN_SCANNER(FILE_LITERAL_STRING);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_STR);
    TEST_ASSERT(!strcmp(NEXT()->literal_str->buffer, "Hello, World"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_STR);
    TEST_ASSERT(!strcmp(NEXT()->literal_str->buffer, "Hi"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_STR);
    TEST_ASSERT(!strcmp(NEXT()->literal_str->buffer, "H"));
    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_STR);
    TEST_ASSERT(!strcmp(NEXT()->literal_str->buffer, "Hello\\nWorld"));

    FREE_SCANNER();
});
