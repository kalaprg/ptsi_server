TEMPLATE = app
CONFIG += console
CONFIG -= qt
DESTDIR = "../../bin/"

ptsi_windows.depends = ptsi_server

SOURCES += main.cpp

INCLUDEPATH += "../ptsi_server/"
LIBS += -L../ptsi_server -lptsi_server

LIBS += -lboost_system -lboost_filesystem -lboost_date_time -lboost_program_options -lmysqlcppconn

