if(LILY_DEBUG)
  # test_base
  add_executable(test_base ${CMAKE_SOURCE_DIR}/tests/base/base.c
                           ${CMAKE_SOURCE_DIR}/src/ex/bin/test_base.c)
  target_link_libraries(test_base PRIVATE lily_base)
  target_include_directories(test_base PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_base COMMAND test_base)
endif()
