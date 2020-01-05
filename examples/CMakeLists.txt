add_executable(read_grf_c "read_grf.c")
target_link_libraries(read_grf_c PRIVATE grflib::grf)

add_executable(extract_all_grf_c "extract_all_grf.c")
target_link_libraries(extract_all_grf_c PRIVATE grflib::grf)

add_executable(read_rgz_c "read_rgz.c")
target_link_libraries(read_rgz_c PRIVATE grflib::grf)
