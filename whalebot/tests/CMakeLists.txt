set(TEST_APP "test_all")

include_directories( ../string_tokenizev2
                     ../webspider/include
                     ../ )
                     
aux_source_directory(./ SOURCES)
list(APPEND SOURCES
                    ${CMAKE_CURRENT_SOURCE_DIR}/../webspider/src/cookie.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/../webspider/src/host.cpp )

find_library( GTEST_LIB
                            NAMES gtest )

#neon

find_path( NEON_INCLUDE_DIR
           NAMES "neon/ne_session.h" )


find_library( NEON_LIBRARY
              NAMES "neon" )

add_executable(${TEST_APP} ${SOURCES})
target_link_libraries(${TEST_APP}
                                ${GTEST_LIB}
                                ${NEON_LIBRARY})
