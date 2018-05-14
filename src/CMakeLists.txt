include_directories( . )

add_subdirectory( core )
add_subdirectory( ui )




# define the executable in terms of the source files
add_executable( ${EXE} main.c )

target_link_libraries( ${EXE}
    ui
    core
    ${CURSES_LIBRARIES}
    ${CURSES_LIBS}
    ${CDK_LIBS}
)


# how to install this executable
install( TARGETS ${EXE} RUNTIME DESTINATION bin )
