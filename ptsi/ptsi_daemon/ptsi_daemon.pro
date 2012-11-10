TEMPLATE = app
CONFIG += console
CONFIG -= qt
DESTDIR = "../../bin/"

LIBS += -llogstream
LIBS += -lboost_system -lboost_program_options

SOURCES += main.cpp \
    ptsiserver.cpp \
    daemon.cpp

HEADERS += \
    ptsiserver.h \
    daemon.h

