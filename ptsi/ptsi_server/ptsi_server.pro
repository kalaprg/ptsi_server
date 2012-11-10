TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

LIBS += -lboost_system

SOURCES += \
    ptsiserver.cpp

HEADERS += \
    ptsiserver.h

