TEMPLATE    =   lib
CONFIG      +=  staticlib
TARGET      =   module_core


CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include( ../../build-directories-config.pri )


HEADERS += grid.h
HEADERS += log/log.h


SOURCES += grid.c
