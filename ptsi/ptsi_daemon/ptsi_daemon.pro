TEMPLATE = app
CONFIG += console
CONFIG -= qt

!win32: {
    INCLUDEPATH += "../ptsi_server/"
    DESTDIR = "../../bin/"
    LIBS += -L../ptsi_server -lptsi_server

    LIBS += -llogstream
    LIBS += -lboost_system -lboost_filesystem -lboost_date_time -lboost_program_options -lmysqlcppconn

    SOURCES += \
        main.cpp \
        daemon.cpp

    HEADERS += \
        daemon.h
}
win32:SOURCES += dummy.cpp
