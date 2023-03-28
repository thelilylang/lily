#include "util.c"

#include <base/test.h>

#include <core/lily/parser.h>

SUITE(data_type);

CASE(data_type_array_dynamic, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_DYNAMIC);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind, LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC);
    TEST_ASSERT_EQ(dt->array.data_type->kind, LILY_AST_DATA_TYPE_KIND_UINT32);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_dynamic2, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_DYNAMIC_2);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind, LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC);
    TEST_ASSERT_EQ(dt->array.data_type, NULL);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_sized, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_SIZED);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind, LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED);
    TEST_ASSERT_EQ(dt->array.size, 30);
    TEST_ASSERT_EQ(dt->array.data_type->kind, LILY_AST_DATA_TYPE_KIND_UINT64);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_sized2, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_SIZED2);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind, LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED);
    TEST_ASSERT_EQ(dt->array.size, 30);
    TEST_ASSERT_EQ(dt->array.data_type, NULL);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_multi_pointers, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_MULTI_POINTERS);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind,
                   LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS);
    TEST_ASSERT_EQ(dt->array.data_type->kind, LILY_AST_DATA_TYPE_KIND_UINT8);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_multi_pointers2, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_MULTI_POINTERS_2);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind,
                   LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS);
    TEST_ASSERT_EQ(dt->array.data_type, NULL);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_undetermined, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_UNDETERMINED);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind, LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED);
    TEST_ASSERT_EQ(dt->array.data_type->kind, LILY_AST_DATA_TYPE_KIND_UINT32);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_array_undetermined2, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_ARRAY_UNDETERMINED_2);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_ARRAY);
    TEST_ASSERT_EQ(dt->array.kind, LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED);
    TEST_ASSERT_EQ(dt->array.data_type, NULL);

    FREE_RUN_PARSE_DATA_TYPE();
});

CASE(data_type_lambda, {
    RUN_PARSE_DATA_TYPE(FILE_DATA_TYPE_LAMBDA);

    TEST_ASSERT_EQ(dt->kind, LILY_AST_DATA_TYPE_KIND_LAMBDA);

    FREE_RUN_PARSE_DATA_TYPE();
});
