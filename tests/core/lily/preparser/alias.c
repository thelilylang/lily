#include "util.c"

#include <base/test.h>

SIMPLE(alias, {
    RUN_PREPARSER(FILE_IMPORT);
    FREE_PREPARSER();
});
