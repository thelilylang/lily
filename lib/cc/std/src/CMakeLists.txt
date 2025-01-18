# NOTE: For the time being, this part of the code is disabled by default for
# compilation, as it is still under development and very experimental.
if(LILY_COMPILE_LIB_CC_STD)
  set(LIB_CC_STD_SRC
      ${CMAKE_SOURCE_DIR}/lib/cc/std/src/errno.c
      ${CMAKE_SOURCE_DIR}/lib/cc/std/src/string/strerror.c
      ${CMAKE_SOURCE_DIR}/lib/cc/std/src/sys/exit.c
      ${CMAKE_SOURCE_DIR}/lib/cc/std/src/sys/write.c)

  # c
  add_library(c SHARED ${LIB_CC_STD_SRC})
  target_compile_options(c PRIVATE -nostdlib -nostdinc -nodefaultlibs)
  target_include_directories(c PRIVATE ${CMAKE_SOURCE_DIR}/lib/cc/std/include)
  set_target_properties(c PROPERTIES SOVERSION 6)

  # crt
  add_library(crt STATIC ${CMAKE_SOURCE_DIR}/lib/cc/std/src/start.c)
  target_compile_options(crt PRIVATE -nostdlib -nostdinc -nodefaultlibs)
  target_include_directories(crt PRIVATE ${CMAKE_SOURCE_DIR}/lib/cc/std/include)
endif()
