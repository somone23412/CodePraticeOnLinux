cmake_minimum_required(VERSION 2.8)
project(Demo4)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
    )

option(
    USE_MYMATH "USE provided math implementation" ON
    )

if(USE_MYMATH)
    include_directories("${PROJECT_SOURCE_DIR}/mymath")
    add_subdirectory(mymath)
    set (EXTRA_LIBS ${EXTRA_LIBS} mymath)
endif(USE_MYMATH)

aux_source_directory(. SRC_DIR)

add_executable(Demo ${SRC_DIR})

target_link_libraries(Demo ${EXTRA_LIBS})
