#include "util.c"

#include <base/test.h>

SIMPLE(next, {
    RUN_PREPARSER(FILE_NEXT);
    FREE_PREPARSER();
});
