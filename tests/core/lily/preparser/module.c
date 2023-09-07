#include "util.c"

#include <base/test.h>

SIMPLE(module, {
    RUN_PREPARSER(FILE_MODULE);

    DECLS_ITERATOR();

    TEST_ASSERT_EQ(CURRENT_DECL()->kind, LILY_PREPARSER_DECL_KIND_MODULE);
    TEST_ASSERT(!strcmp(NEXT_DECL()->module.name->buffer, "X"));

    TEST_ASSERT_EQ(CURRENT_DECL()->kind, LILY_PREPARSER_DECL_KIND_MODULE);
    TEST_ASSERT(!strcmp(NEXT_DECL()->module.name->buffer, "X.Y"));

    FREE_PREPARSER();
});
