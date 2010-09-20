cmake_minimum_required(VERSION 2.6)
project(whalebot)

set( WHALEBOT_ROOT ${CMAKE_CURRENT_BINARY_DIR} )



if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE )
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${WHALEBOT_ROOT}/${CMAKE_BUILD_TYPE}/lib" )



set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")



add_subdirectory( statsem_string )
add_subdirectory( 3dparty/google-url )
add_subdirectory( webspider )
