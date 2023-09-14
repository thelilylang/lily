#include "util.c"

#include <base/test.h>

SIMPLE(fun, {
    RUN_PREPARSER(FILE_FUN);
    FREE_PREPARSER();
});
