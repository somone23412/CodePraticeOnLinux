cmake_minimum_required(VERSION 2.8)
project(Demo2)
aux_source_directory(. SRC_DIR)
add_executable(Demo ${SRC_DIR})
