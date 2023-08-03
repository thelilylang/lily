cmake_minimum_required(VERSION 3.20)
project(
  lily
  VERSION 0.0.0
  HOMEPAGE_URL "https://github.com/ArthurPV/lily"
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -fvisibility-inlines-hidden -fpic")
add_subdirectory(./llvm/llvm)
