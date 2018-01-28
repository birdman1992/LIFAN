#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T16:22:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = driver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    threadcpp.cpp \
    lowlayer.cpp \
    usbcan.cpp

HEADERS  += mainwindow.h \
    thread.h \
    can/controlcan.h \
    lowlayer.h \
    usbcan.h

FORMS    += mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/can/release/ -lcontrolcan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/can/debug/ -lcontrolcan
else:unix: LIBS += -L$$PWD/can/ -lcontrolcan

INCLUDEPATH += $$PWD/can
DEPENDPATH += $$PWD/can
