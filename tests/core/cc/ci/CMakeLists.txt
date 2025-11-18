if(LILY_DEBUG)
	add_test(NAME ci_self_test COMMAND "ci" "self-test" "tests/core/cc/ci" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
