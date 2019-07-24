cmake_minimum_required(VERSION 2.8)

project(APP1)

set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)

option(
    USE_MYMATH "USE provided math implementation" ON
    )

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
    )

if(USE_MYMATH)
    include_directories(${PROJECT_SOURCE_DIR}/mymath)
    add_subdirectory(${PROJECT_SOURCE_DIR}/mymath)
    set (EXTRA_LIBS ${EXTRA_LIBS} mymath)
endif(USE_MYMATH)

aux_source_directory(. SRC_DIR)

add_executable(Demo ${SRC_DIR})

target_link_libraries(Demo ${EXTRA_LIBS})

install (TARGETS Demo DESTINATION ${PROJECT_SOURCE_DIR}/APP1/bin)
install (
    FILES "${PROJECT_SOURCE_DIR}/config.h" DESTINATION ${PROJECT_SOURCE_DIR}/APP1/include
    )
