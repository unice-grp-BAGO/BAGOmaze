include_directories( . )

# set up the source files compilation
# add the source files
set ( SRC
# main.c
	core/log/log.h
	core/grid.h
	core/grid.c
	ui/ui.c
	ui/ui.h
	ui/ui_menu.c
	ui/ui_menu.h
	ui/ui_menuPrincipal.c
	ui/ui_menuPrincipal.h
	ui/ui_private.h
	ui/ui_utils.c
	ui/ui_utils.h
)


# define the executable in terms of the source files
add_executable( ${EXE} main.c ${SRC} )
#target_link_libraries( ${EXE} ${CURSES_LIBRARIES} ${CDK_LIBRARIES} )
target_link_libraries( ${EXE} ${CURSES_LIBRARIES} ${CURSES_LIBS} ${CDK_LIBS} )


# how to install this executable
install( TARGETS ${EXE} RUNTIME DESTINATION bin )
