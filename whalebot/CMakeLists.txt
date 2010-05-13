cmake_minimum_required(VERSION 2.6)

add_subdirectory(statsem_string)
add_subdirectory(statsem_filesystem)
add_subdirectory(webspider)
add_dependencies( webspider libstring_parse libfilesystem_create )


