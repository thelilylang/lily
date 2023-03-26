#include "util.c"

#include <base/test.h>

#include <string.h>

SIMPLE(identifier_normal, {
    RUN_SCANNER(FILE_IDENTIFIER_NORMAL);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_NORMAL);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "name1"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_NORMAL);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "name_2"));

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_IDENTIFIER_NORMAL);
    TEST_ASSERT(!strcmp(NEXT()->identifier_normal->buffer, "_name3"));

    FREE_SCANNER();
});
