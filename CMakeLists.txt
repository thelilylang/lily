cmake_minimum_required(VERSION 3.10)
project(lily)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(BASE_SRC
    src/base/vec.c)

add_library(lily_base STATIC ${BASE_SRC})
target_include_directories(lily_base PUBLIC include)
