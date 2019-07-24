aux_source_directory(. LIBSRC_DIR)
add_library(mymath ${LIBSRC_DIR})

install (TARGETS mymath DESTINATION ${PROJECT_SOURCE_DIR}/APP1/bin)
install (FILES mymath.h DESTINATION ${PROJECT_SOURCE_DIR}/APP1/include)
