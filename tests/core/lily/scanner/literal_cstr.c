#include "util.c"

#include <base/test.h>

CASE(literal_cstr, {
    RUN_SCANNER(FILE_LITERAL_CSTR);
    SCANNER_ITERATOR();

    TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CSTR);
    TEST_ASSERT(!strcmp(NEXT()->literal_cstr, "Hello"));

    FREE_SCANNER();
});
