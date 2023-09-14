#include "util.c"

#include <base/test.h>

SIMPLE(record_object, {
    RUN_PREPARSER(FILE_RECORD_OBJECT);
    FREE_PREPARSER();
});
