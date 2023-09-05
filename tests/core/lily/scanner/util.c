#ifndef UTIL_C
#define UTIL_C

#include <base/file.h>

#include <core/lily/scanner/scanner.h>

#define FILE_AT_KEYWORD "./tests/core/lily/scanner/input/at_keyword.lily"
#define FILE_COMMENT_DOC "./tests/core/lily/scanner/input/comment_doc.lily"
#define FILE_IDENTIFIER_DOLLAR \
    "./tests/core/lily/scanner/input/identifier_dollar.lily"
#define FILE_IDENTIFIER_MACRO \
    "./tests/core/lily/scanner/input/identifier_macro.lily"
#define FILE_IDENTIFIER_NORMAL \
    "./tests/core/lily/scanner/input/identifier_normal.lily"
#define FILE_IDENTIFIER_OPERATOR \
    "./tests/core/lily/scanner/input/identifier_operator.lily"
#define FILE_KEYWORD "./tests/core/lily/scanner/input/keyword.lily"
#define FILE_LITERAL_BYTE "./tests/core/lily/scanner/input/literal_byte.lily"
#define FILE_LITERAL_BYTES "./tests/core/lily/scanner/input/literal_bytes.lily"
#define FILE_LITERAL_CHAR "./tests/core/lily/scanner/input/literal_char.lily"
#define FILE_LITERAL_INT_10 \
    "./tests/core/lily/scanner/input/literal_int_10.lily"
#define FILE_LITERAL_INT_16 \
    "./tests/core/lily/scanner/input/literal_int_16.lily"
#define FILE_LITERAL_INT_2 "./tests/core/lily/scanner/input/literal_int_2.lily"
#define FILE_LITERAL_INT_8 "./tests/core/lily/scanner/input/literal_int_8.lily"
#define FILE_LITERAL_STRING \
    "./tests/core/lily/scanner/input/literal_string.lily"
#define FILE_LITERAL_SUFFIX_FLOAT32 \
    "./tests/core/lily/scanner/input/literal_suffix_float32.lily"
#define FILE_LITERAL_SUFFIX_FLOAT64 \
    "./tests/core/lily/scanner/input/literal_suffix_float64.lily"
#define FILE_LITERAL_SUFFIX_INT16 \
    "./tests/core/lily/scanner/input/literal_suffix_int16.lily"
#define FILE_LITERAL_SUFFIX_INT32 \
    "./tests/core/lily/scanner/input/literal_suffix_int32.lily"
#define FILE_LITERAL_SUFFIX_INT64 \
    "./tests/core/lily/scanner/input/literal_suffix_int64.lily"
#define FILE_LITERAL_SUFFIX_INT8 \
    "./tests/core/lily/scanner/input/literal_suffix_int8.lily"
#define FILE_LITERAL_SUFFIX_ISIZE \
    "./tests/core/lily/scanner/input/literal_suffix_isize.lily"
#define FILE_LITERAL_SUFFIX_UINT16 \
    "./tests/core/lily/scanner/input/literal_suffix_uint16.lily"
#define FILE_LITERAL_SUFFIX_UINT32 \
    "./tests/core/lily/scanner/input/literal_suffix_uint32.lily"
#define FILE_SEPARATOR "./tests/core/lily/scanner/input/separator.lily"
#define FILE_OPERATOR "./tests/core/lily/scanner/input/operator.lily"

LilyScanner
run_scanner(File *file)
{
    LilyScanner scanner =
      NEW(LilyScanner, NEW(Source, NEW(Cursor, file->content), file));

    run__LilyScanner(&scanner, false);

    return scanner;
}

#define RUN_SCANNER(filename)                  \
    char *content = read_file__File(filename); \
    File file = NEW(File, filename, content);  \
    LilyScanner scanner = run_scanner(&file);

#define FREE_SCANNER() \
    FREE(File, &file); \
    FREE(LilyScanner, &scanner);

#define GET_TOKEN(idx) CAST(LilyToken *, get__Vec(scanner.tokens, idx))

#define SCANNER_ITERATOR() Usize it = 0;

#define CURRENT() CAST(LilyToken *, get__Vec(scanner.tokens, it))
#define NEXT() CAST(LilyToken *, get__Vec(scanner.tokens, it++))
#define PREVIOUS() CAST(LilyToken *, get__Vec(scanner.tokens, it - 1))

#endif // UTIL_C
