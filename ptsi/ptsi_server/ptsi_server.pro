TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

LIBS += -lboost_system -lmysqlcppconn

SOURCES += \
    ptsiserver.cpp \
    session.cpp \
    common.cpp \
    internalconnection.cpp \
    biosignaldata.cpp \
    tlsconnection.cpp \

HEADERS += \
    ptsiserver.h \
    session.h \
    connection.h \
    common.h \
    internalconnection.h \
    biosignaldata.h \
    tlsconnection.h \
    connection_impl.h

