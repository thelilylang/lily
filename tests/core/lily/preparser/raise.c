#include "util.c"

#include <base/test.h>

SIMPLE(raise, {
    RUN_PREPARSER(FILE_RAISE);
    FREE_PREPARSER();
});
