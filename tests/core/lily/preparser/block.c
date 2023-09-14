#include "util.c"

#include <base/test.h>

SIMPLE(block, {
    RUN_PREPARSER(FILE_BLOCK);
    FREE_PREPARSER();
});
