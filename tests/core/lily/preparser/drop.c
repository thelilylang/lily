#include "util.c"

#include <base/test.h>

SIMPLE(drop, {
    RUN_PREPARSER(FILE_DROP);
    FREE_PREPARSER();
});
