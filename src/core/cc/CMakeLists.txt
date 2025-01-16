include(${CMAKE_SOURCE_DIR}/cmake/Lily.cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/cc/ci)

# NOTE: For the time being, this part of the code is disabled by default for
# compilation, as it is still under development and very experimental.
if(LILY_COMPILE_CC_RUNTIME)
  set(CC_RUNTIME_SRC ${CMAKE_SOURCE_DIR}/src/core/cc/runtime/sys/exit.c
                     ${CMAKE_SOURCE_DIR}/src/core/cc/runtime/sys/write.c)

  # cc_runtime
  add_library(cc_runtime STATIC ${CC_RUNTIME_SRC})
  target_include_directories(cc_runtime PRIVATE ${LILY_INCLUDE})
  target_compile_options(cc_runtime PRIVATE -nostdlib)

  set(CC_SRC ${CMAKE_SOURCE_DIR}/src/core/cc/runtime/start.c)

  # c
  add_library(c SHARED ${CC_SRC})
  target_link_libraries(c PRIVATE cc_runtime)
  target_include_directories(c PRIVATE ${LILY_INCLUDE})
  target_compile_options(c PRIVATE -nostdlib)
endif()
