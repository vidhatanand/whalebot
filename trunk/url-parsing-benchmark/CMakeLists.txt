cmake_minimum_required(VERSION 2.6)
project(url_parse)

include_directories( "../whalebot/3dparty/google-url"
                     "../whalebot/3dparty/google-url/googleurl"
                     "../whalebot/webspider/include"
                     "../whalebot/statsem_string/include" )

link_directories ( "../whalebot/3dparty/google-url" )

#htmlcxx
find_library ( HTMLCXX_LIBRARY  "htmlcxx" )

#libneon
find_library (NEON_LIBRARY "neon")

find_package ( Boost 1.37 REQUIRED date_time serialization )

include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${Boost_LIBRARY_DIRS} )

aux_source_directory("./" SOURCES)

list( APPEND SOURCES "../whalebot/webspider/src/prefix.cpp")

list ( APPEND CMAKE_CXX_FLAGS "-Wall")

add_executable( url_parse ${SOURCES})

target_link_libraries ( url_parse
                        ${Boost_LIBRARIES}
                        gurl
                        pthread
                        icuuc
                        ${HTMLCXX_LIBRARY}
                        ${NEON_LIBRARY} )
