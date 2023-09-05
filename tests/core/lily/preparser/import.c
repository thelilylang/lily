#include "util.c"

#include <base/test.h>

SIMPLE(import, {
    RUN_PREPARSER(FILE_IMPORT);

    PRIVATE_IMPORT_ITERATOR();
    PUBLIC_IMPORT_ITERATOR();

    TEST_ASSERT_EQ(CURRENT_PRIVATE_IMPORT()->as, NULL);
    TEST_ASSERT(!strcmp(CURRENT_PRIVATE_IMPORT()->value->buffer, "@sys"));

    TEST_ASSERT(!strcmp(CURRENT_PUBLIC_IMPORT()->as->buffer, "s"));
    TEST_ASSERT(!strcmp(CURRENT_PUBLIC_IMPORT()->value->buffer, "@std"));

    FREE_PREPARSER();
});
