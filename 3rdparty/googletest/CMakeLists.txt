set(BUILD_GMOCK "Builds the googlemock subproject" OFF)
set(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)

add_subdirectory("googletest" EXCLUDE_FROM_ALL)