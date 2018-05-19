TEMPLATE    =   lib
CONFIG      +=  staticlib
TARGET      =   module_ui


CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include( ../../build-directories-config.pri )


LIBS    += -lncurses
LIBS    += -lmenu
LIBS    += -lncursesw
LIBS    += -lcdk

LIBS    +=  -L$${DESTDIR}/../lib/
LIBS    +=  -lmodule_core


INCLUDEPATH += ..


HEADERS += ui.h
HEADERS += ui_menu.h
HEADERS += ui_menuPrincipal.h
HEADERS += ui_play_random.h
HEADERS += ui_private.h
HEADERS += ui_utils.h



SOURCES += ui.c
SOURCES += ui_menu.c
SOURCES += ui_menuPrincipal.c
SOURCES += ui_play_random.c
SOURCES += ui_utils.c

