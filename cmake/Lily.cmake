set(LILY_INCLUDE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/local/include
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/local/src/llvm-project/llvm/include
    ${CMAKE_SOURCE_DIR}/lib/local/src/llvm-project/lld/include)

if(LILY_BUILD_LLVM)
  set(LILY_INCLUDE ${LILY_INCLUDE} ${LILY_LLVM_BUILD_DIR}/include)
endif()
