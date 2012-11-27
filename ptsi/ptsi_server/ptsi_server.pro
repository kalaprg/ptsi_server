TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

LIBS += -lboost_system -lmysqlcppconn

SOURCES += \
    ptsiserver.cpp \
    session.cpp \
    connection.cpp \
    common.cpp \
    internalconnection.cpp

HEADERS += \
    ptsiserver.h \
    session.h \
    connection.h \
    common.h \
    internalconnection.h

