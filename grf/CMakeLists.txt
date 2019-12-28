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