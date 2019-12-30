# Needed for POSIX 'fileno'
add_definitions(-D_POSIX_C_SOURCE=200809L)

set(GRFLIB_FILES
    "grftypes.h"
    "grf.h"
    "grf.c"
    "grfcrypt.h"
    "grfcrypt.c"
    "grfsupport.h"
    "grfsupport.c"
    "rgz.h"
    "rgz.c"
)

if (GRFLIB_BUILD_SHARED_LIBS)
  add_library(grf
    SHARED
      ${GRFLIB_FILES}
  )
else (GRFLIB_BUILD_SHARED_LIBS)
  add_library(grf
    STATIC
      ${GRFLIB_FILES}
  )
  target_compile_definitions(grf PRIVATE GRFLIB_STATIC)
endif (GRFLIB_BUILD_SHARED_LIBS)

target_include_directories(grf PUBLIC ${GRF_COMMON_INCLUDE_DIRS})
target_link_libraries(grf PRIVATE grflib::zlib)

add_library(grflib::grf ALIAS grf)

if (GRFLIB_BUILD_TESTS)
  add_executable(grf_test
    "grf_test.cc"
  )
  target_link_libraries(grf_test PRIVATE grflib::grf gtest gtest_main)

  add_executable(rgz_test
    "rgz_test.cc"
  )
  target_link_libraries(rgz_test PRIVATE grflib::grf gtest gtest_main)
endif (GRFLIB_BUILD_TESTS)
