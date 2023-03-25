#include "util.c"

#include <base/test.h>

#include <string.h>

SIMPLE(identifier_dollar, {
    RUN_SCANNER(FILE_IDENTIFIER_DOLLAR);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_DOLLAR);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "Id33"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_DOLLAR);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "ffdle"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_DOLLAR);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "Iddldl"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_DOLLAR);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "_30dd"));

    FREE_SCANNER();
});
