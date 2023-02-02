cmake_minimum_required(VERSION 3.10)
project(lily)

set(DEBUG 1) # 0 for release, 1 for debug

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -O3)

set(BASE_SRC
	src/base/atoi.c
	src/base/color.c
	src/base/file.c
	src/base/format.c
	src/base/hash_map.c
	src/base/itoa.c
	src/base/string.c
  	src/base/vec.c)

add_library(lily_base STATIC ${BASE_SRC})
target_include_directories(lily_base PUBLIC include)

add_executable(lily src/bin/main.c)
target_link_libraries(lily lily_base)

if (DEBUG)
	add_executable(test_base tests/base/base.c)
	target_link_libraries(test_base lily_base)

	add_test(NAME test_base COMMAND test_base)

	add_executable(bench_vec benchmarks/base/vec.c)
	target_link_libraries(bench_vec lily_base)

	add_executable(bench_string benchmarks/base/string.c)
	target_link_libraries(bench_string lily_base)

	add_executable(bench_hash_map benchmarks/base/hash_map.c)
	target_link_libraries(bench_hash_map lily_base)
endif()
