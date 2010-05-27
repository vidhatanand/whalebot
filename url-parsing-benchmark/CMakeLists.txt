cmake_minimum_required(VERSION 2.6)
project(url_parse)

add_subdirectory("./google-url")


include_directories("./google-url")
include_directories("./google-url/googleurl")
include_directories("../whalebot/webspider/include")
include_directories("../whalebot/statsem_string/include")

link_directories ("../whalebot/statsem_string/bin")

#htmlcxx
find_library ( HTMLCXX_LIBRARY  "htmlcxx" )

find_package ( Boost 1.37 REQUIRED date_time )

include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${Boost_LIBRARY_DIRS} )

aux_source_directory("./" SOURCES)

list(APPEND SOURCES
        "../whalebot/webspider/src/link.cpp"
        "../whalebot/webspider/src/prefix.cpp"
        "../whalebot/webspider/src/link_factory.cpp" )

add_executable( url_parse ${SOURCES})
target_link_libraries ( url_parse ${Boost_LIBRARIES} string_parse gurl pthread icuuc  ${HTMLCXX_LIBRARY})
