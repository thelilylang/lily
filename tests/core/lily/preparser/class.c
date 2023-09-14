#include "util.c"

#include <base/test.h>

SIMPLE(class, {
    RUN_PREPARSER(FILE_CLASS);
    FREE_PREPARSER();
});
