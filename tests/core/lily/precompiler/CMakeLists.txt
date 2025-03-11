if(LILY_DEBUG)
  # test_core_precompiler
  add_executable(
    test_core_precompiler
    ${CMAKE_SOURCE_DIR}/tests/core/lily/precompiler/precompiler.c
    ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_precompiler.c)
  target_link_libraries(test_core_precompiler
                        PRIVATE lily_core_lily_precompiler)
  target_include_directories(test_core_precompiler PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_precompiler COMMAND test_core_precompiler)
endif()
