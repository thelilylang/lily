if(LILY_DEBUG)
  # test_core_scanner
  add_executable(
    test_core_scanner ${CMAKE_SOURCE_DIR}/tests/core/lily/scanner/scanner.c
                      ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_scanner.c)
  target_link_libraries(test_core_scanner PRIVATE lily_core_lily_scanner)
  target_include_directories(test_core_scanner PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_scanner COMMAND test_core_scanner WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
