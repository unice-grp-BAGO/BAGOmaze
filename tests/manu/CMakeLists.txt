include_directories( ${SRC} )


set( DESTDIR_TESTS_MANUELS ${DESTDIR_TESTS}/manu )



# define the executable in terms of the source files
add_executable(         test-gridData test-gridData.c )
target_link_libraries(  test-gridData core ) # Define which libraries are needed by the target
set_target_properties(  test-gridData PROPERTIES    LINKER_LANGUAGE C )
set_target_properties(  test-gridData PROPERTIES    RUNTIME_OUTPUT_DIRECTORY
                                                    ${DESTDIR_TESTS_MANUELS} )
