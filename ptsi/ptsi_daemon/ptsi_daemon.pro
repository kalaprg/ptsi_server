TEMPLATE = app
CONFIG += console
CONFIG -= qt
DESTDIR = "../../bin/"

INCLUDEPATH += "../ptsi_server/"
LIBS += -L../ptsi_server -lptsi_server

LIBS += -llogstream
LIBS += -lboost_system -lboost_program_options

SOURCES += \
    main.cpp \
    daemon.cpp

HEADERS += \
    daemon.h
