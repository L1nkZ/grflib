add_subdirectory("zlib")

if (GRFLIB_BUILD_TESTS)
  add_subdirectory("googletest")
endif (GRFLIB_BUILD_TESTS)