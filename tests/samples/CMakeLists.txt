if (LILY_DEBUG)
	add_test(NAME lily_test_samples COMMAND "./scripts/test_samples.sh" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
	add_test(NAME lily_test_samples_run COMMAND "./scripts/run_bins.sh" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
