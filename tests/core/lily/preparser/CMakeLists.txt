if(LILY_DEBUG)
  # test_core_preparser
  add_executable(
    test_core_preparser
    ${CMAKE_SOURCE_DIR}/tests/core/lily/preparser/preparser.c
    ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_preparser.c)
  target_link_libraries(test_core_preparser PRIVATE lily_core_lily_preparser)
  target_include_directories(test_core_preparser PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_preparser COMMAND test_core_preparser WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
