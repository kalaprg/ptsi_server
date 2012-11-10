TEMPLATE = app
CONFIG += console
CONFIG -= qt
DESTDIR = "../../bin/"

SOURCES += main.cpp

INCLUDEPATH += "../ptsi_server/"
LIBS += -L../ptsi_server -lptsi_server

LIBS += -lboost_system -lboost_program_options

