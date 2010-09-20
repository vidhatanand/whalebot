include_directories("./include")
aux_source_directory("./src" SOURCES)
add_library(string_parse STATIC ${SOURCES})
