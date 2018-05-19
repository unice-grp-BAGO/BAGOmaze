TEMPLATE    =   app
TARGET      =   BAGOmaze


CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include( ../build-directories-config.pri )


# Attention : Ordre des libs important !
LIBS    +=  -L$${DESTDIR}/../lib/
LIBS    +=  -lmodule_ui
LIBS    +=  -lmodule_core

LIBS    += -lncurses
LIBS    += -lmenu
LIBS    += -lncursesw
LIBS    += -lcdk


INCLUDEPATH += .


SOURCES += main.c
