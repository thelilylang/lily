#include "util.c"

#include <base/test.h>

#include <string.h>

SIMPLE(identifier_macro, {
    RUN_SCANNER(FILE_IDENTIFIER_MACRO);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_MACRO);
    TEST_ASSERT(!strcmp(NEXT()->identifier_macro->buffer, "name"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_MACRO);
    TEST_ASSERT(!strcmp(NEXT()->identifier_macro->buffer, "name"));

    FREE_SCANNER();
});
