#include "util.c"

#include <base/test.h>
#include <stdio.h>

SIMPLE(package, {
    RUN_PREPARSER(FILE_PACKAGE);

    TEST_ASSERT(!strcmp(preparser_info.package->name->buffer, "example"));
    TEST_ASSERT_EQ(preparser_info.package->sub_packages->len, 3);
    TEST_ASSERT(!strcmp(CAST(LilyPreparserSubPackage *,
                             get__Vec(preparser_info.package->sub_packages, 0))
                          ->name->buffer,
                        "A"));
    TEST_ASSERT(!strcmp(CAST(LilyPreparserSubPackage *,
                             get__Vec(preparser_info.package->sub_packages, 1))
                          ->name->buffer,
                        "B"));
    TEST_ASSERT(!strcmp(CAST(LilyPreparserSubPackage *,
                             get__Vec(preparser_info.package->sub_packages, 2))
                          ->name->buffer,
                        "C"));

    FREE_PREPARSER();
});
