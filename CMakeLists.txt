# cmake version
cmake_minimum_required ( VERSION 2.8.10 FATAL_ERROR )

# define project name and langage
project ( MyProject C )

# set( DESTDIR_APP ${CMAKE_SOURCE_DIR}/out/app )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/app )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib )

# Include external libraries
## ncurses
find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIR} )
set( CURSES_LIBS -lmenu -lncurses )
## CDK
set( CDK_LIBS -lcdk )


# define executable names
set ( EXE BAGOmaze )

# define directories
set ( SRC ${CMAKE_SOURCE_DIR}/src )
add_subdirectory( ${SRC} )


#
#   Configuration liee aux tests
#
ENABLE_TESTING()
set( TESTS ${CMAKE_SOURCE_DIR}/tests )
add_subdirectory( ${TESTS} )
