#include "util.c"

#include <base/test.h>

SIMPLE(alias, {
    RUN_PREPARSER(FILE_ALIAS);
    FREE_PREPARSER();
});
