#include "util.c"

#include <base/test.h>

SIMPLE(trait, {
    RUN_PREPARSER(FILE_TRAIT);
    FREE_PREPARSER();
});
