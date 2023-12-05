set(CI_SRC
	${CMAKE_SOURCE_DIR}/src/bin/ci/main.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/ast.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic/error.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic/warning.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/file.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/generator.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/parser.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/scanner.c
	${CMAKE_SOURCE_DIR}/src/core/cc/ci/token.c)

add_executable(ci ${CI_SRC})
target_link_libraries(ci PRIVATE lily_base lily_core_shared)
target_include_directories(ci PRIVATE ${LILY_INCLUDE})
