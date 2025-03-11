if(LILY_BUILD_LLVM)
  message(
    WARNING
      "Make sure your machine is powerful enough to build LLVM. If not, you can install LLVM on your system and build without having to build LLVM (takes far fewer resources and is much faster)."
  )

  # NOTE: The `llvm-projet` submodule is configured in `cmake/LLVM.cmake`.
endif()
