cmake_minimum_required(VERSION 2.8)
project(Demo3)
aux_source_directory(. SRC_DIR)
add_subdirectory(calc)
add_executable(Demo ${SRC_DIR})
target_link_libraries(Demo calc)
