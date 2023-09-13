#include "util.c"

#include <base/test.h>

SIMPLE(defer, {
    RUN_PREPARSER(FILE_DEFER);
    FREE_PREPARSER();
});
