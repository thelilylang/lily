#include "util.c"

#include <base/test.h>

#include <string.h>

SIMPLE(identifier_operator, {
    RUN_SCANNER(FILE_IDENTIFIER_OPERATOR);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_OPERATOR);
    TEST_ASSERT(!strcmp(NEXT()->identifier_macro->buffer, "+"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_OPERATOR);
    TEST_ASSERT(!strcmp(NEXT()->identifier_macro->buffer, "-"));

    FREE_SCANNER();
});
