cmake_minimum_required(VERSION 2.6)
project(libfilesystem_create)
include_directories("./include")
include_directories("../statsem_string/include")
file(GLOB SOURCES "./src/*.cpp")
set(LIBRARY_OUTPUT_PATH "./bin/")
add_library(filesystem_create STATIC ${SOURCES})



