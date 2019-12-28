cmake_minimum_required(VERSION 3.8)

project(grflib LANGUAGES C CXX)

list(APPEND GRF_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("grf")
