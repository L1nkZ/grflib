cmake_minimum_required(VERSION 3.8)

project(grflib LANGUAGES C CXX)

option(GRFLIB_BUILD_EXAMPLES "Build code examples" OFF)

set(CMAKE_C_STANDARD 99)
set(C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND GRF_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("3rdparty")
add_subdirectory("grf")
add_subdirectory("examples")
