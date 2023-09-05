#include "util.c"

#include <base/test.h>

SIMPLE(import, {
    RUN_PREPARSER(FILE_IMPORT);

    PRIVATE_IMPORT_ITERATOR();

    TEST_ASSERT_EQ(CURRENT_PRIVATE_IMPORT()->as, NULL);
    TEST_ASSERT(!strcmp(CURRENT_PRIVATE_IMPORT()->value->buffer, "@sys"));

    FREE_PREPARSER();
    // SCANNER_ITERATOR();

    // TEST_ASSERT_EQ(CURRENT()->kind, LILY_TOKEN_KIND_LITERAL_CSTR);
    // TEST_ASSERT(!strcmp(NEXT()->literal_cstr, "Hello"));

    // FREE_SCANNER();
});
