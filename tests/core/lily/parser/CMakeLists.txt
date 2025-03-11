if(LILY_DEBUG)
  # test_core_parser
  add_executable(
    test_core_parser ${CMAKE_SOURCE_DIR}/tests/core/lily/parser/parser.c
                     ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_parser.c)
  target_link_libraries(
    test_core_parser PRIVATE lily_core_lily_parser lily_core_lily_package
                             ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
  target_include_directories(test_core_parser PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_parser COMMAND test_core_parser)
endif()
