include_directories( ${SRC} )


set( DESTDIR_TESTS_MANUELS ${DESTDIR_TESTS}/manu )


# set up the source files compilation
# add the source files
set( TEST_FILES
    test-gridData.c
)



# define the executable in terms of the source files
add_executable( test-gridData test-gridData.c )

# Define which libraries are needed by the target
target_link_libraries( test-gridData framework ${CURSES_LIBRARIES} ${CURSES_LIBS} ${CDK_LIBS} )

# Set properties of the target
set_target_properties( test-gridData PROPERTIES LINKER_LANGUAGE C )
set_target_properties( test-gridData PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DESTDIR_TESTS_MANUELS} )
