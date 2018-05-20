TEMPLATE    =   app
TARGET      =   BAGOmaze


CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include( ../build-directories-config.pri )


# ##############################################################################
#   Librairies projet
#
# Attention : Ordre des libs important !
LIBS    +=  -L$${DESTDIR}/../lib/

# lib "UI"
LIBS            +=  -lmodule_ui
PRE_TARGETDEPS  +=  $${DESTDIR}/../lib/libmodule_ui.a

# lib "core"
LIBS            +=  -lmodule_core
PRE_TARGETDEPS  +=  $${DESTDIR}/../lib/libmodule_core.a


# ##############################################################################
#   Librairies systeme
#
LIBS    += -lncurses
LIBS    += -lmenu
LIBS    += -lncursesw
LIBS    += -lcdk


INCLUDEPATH += .


SOURCES += main.c
