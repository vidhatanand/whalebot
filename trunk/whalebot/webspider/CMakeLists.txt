find_package(Boost 1.37 REQUIRED filesystem system thread program_options date_time)

#icu
find_path( ICU_INCLUDE_DIR
           NAMES unicode/utypes.h )


find_library( ICU_LIBRARY
              NAMES icuuc )

#neon

find_path( NEON_INCLUDE_DIR
           NAMES "neon/ne_session.h" )


find_library( NEON_LIBRARY
              NAMES "neon" )


#htmlcxx
find_path ( HTMLCXX_INCLUDE_DIR
			NAMES "htmlcxx/html/utils.h" ) 
find_library ( HTMLCXX_LIBRARY  "htmlcxx" )

include_directories( "./include"
                     "../"
                     "../statsem_string/include"
                     "../3dparty/google-url"
                     "../3dparty/google-url/googleurl"
                     "../3dparty/google-url/googleurl/src"
                     ${Boost_INCLUDE_DIRS}
                     ${ICU_INCLUDE_DIR}
                     ${NEON_INCLUDE_DIR}
		     ${HTMLCXX_INCLUDE_DIR}
                     "../servers/one_message_server/include" )

link_directories ( ${Boost_LIBRARY_DIRS} )

aux_source_directory("./src/" SOURCES)

add_executable( whalebot ${SOURCES} )
target_link_libraries ( whalebot ${Boost_LIBRARIES}
                                 ${HTMLCXX_LIBRARY}
                                 string_parse
                                 pthread
                                 gurl
                                 ${ICU_LIBRARY}
                                 ${NEON_LIBRARY} )

install ( TARGETS whalebot RUNTIME DESTINATION bin CONFIGURATIONS RELEASE )
