# Needed for POSIX 'fileno'
add_definitions(-D_POSIX_C_SOURCE=200809L)

add_library(grf
  STATIC
    "grf.h"
    "grf.c"
    "grfcrypt.h"
    "grfcrypt.c"
    "grfsupport.h"
    "grfsupport.c"
    "rgz.h"
    "rgz.c"
)
target_include_directories(grf PUBLIC ${GRF_COMMON_INCLUDE_DIRS})
target_link_libraries(grf PRIVATE grflib::zlib)
add_library(grflib::grf ALIAS grf)

if (GRFLIB_BUILD_TESTS)
  add_executable(grf_test
    "grf_test.cc"
  )
  target_link_libraries(grf_test PRIVATE grflib::grf gtest gtest_main)
endif (GRFLIB_BUILD_TESTS)