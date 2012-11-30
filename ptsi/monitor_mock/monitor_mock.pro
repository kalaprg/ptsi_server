TEMPLATE = app
CONFIG += console
CONFIG -= qt
DESTDIR = "../../bin/"

LIBS += -lboost_system -lncurses

SOURCES += main.cpp
