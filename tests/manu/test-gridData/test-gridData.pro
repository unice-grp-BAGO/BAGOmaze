TEMPLATE    =   app
#TARGET      =


CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include( ../../../build-directories-config.pri )


LIBS    +=  -L$${DESTDIR}/../lib/
LIBS    +=  -lmodule_core


INCLUDEPATH += ../../../src/


SOURCES += src/test-gridData.c
