#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T11:46:04
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IV_LIFAN_V2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lowerlayer/braking.cpp \
    lowerlayer/lowerlayer.cpp \
    thread/thread.cpp \
    can/can.cpp \
    lowerlayer/gear.cpp \
    lowerlayer/steering.cpp \
    lowerlayer/throttle.cpp \
    lowerlayer/lightHorn.cpp \
    lowerlayer/remoteCtrl.cpp \
    lowerlayer/transmission.cpp \
    gps/gps.cpp \
    lidar4/ibeo4.cpp \
    decisionMaking/decisionMake.cpp \
    decisionMaking/followTrack.cpp \
    decisionMaking/coordinateTransfer.cpp \
    decisionMaking/subFunctions.cpp \
    decisionMaking/parameters.cpp \
    decisionMaking/curvature.cpp \
    decisionMaking/obstacle.cpp \
    lidar4/ibeo4_main.cpp \
    decisionMaking/detectOverlay.cpp \
    gui/paintwidget.cpp \
    decisionMaking/vehicleSpeed.cpp \
    decisionMaking/manyConditions.cpp \
    decisionMaking/modeSelector.cpp \
    detectlane/lanedetection.cpp \
    gui/camerawidget.cpp \
    decisionMaking/obstacle_left.cpp \
    decisionMaking/obstacle_right.cpp \
    trafficlight/trafficlight.cpp \
    lidar16/velodyne.cpp \
    Logo/logo.cpp \
    driver/lowlayer.cpp \
    driver/braking.cpp \
    driver/can.cpp \
    driver/gear.cpp \
    driver/lightHorn.cpp \
    driver/main.cpp \
    driver/mainwindow.cpp \
    driver/remoteCtrl.cpp \
    driver/steering.cpp \
    driver/threadcpp.cpp \
    driver/throttle.cpp \
    driver/transmission.cpp \
    driver/usbcan.cpp

HEADERS  += mainwindow.h \
    lowerlayer/braking.h \
    lowerlayer/lowerlayer.h \
    thread/thread.h \
    can/can.h \
    lowerlayer/gear.h \
    lowerlayer/steering.h \
    lowerlayer/throttle.h \
    lowerlayer/lightHorn.h \
    lowerlayer/remoteCtrl.h \
    lowerlayer/transmission.h \
    gps/gps.h \
    lidar4/ibeo4.h \
    decisionMaking/decisionMake.h \
    decisionMaking/parameters.h \
    decisionMaking/curvature.h \
    decisionMaking/constant.h \
    decisionMaking/includes.h \
    decisionMaking/variables.h \
    gui/paintwidget.h \
    detectlane/curvefitting.h \
    detectlane/Data.h \
    detectlane/KeyPointExt.h \
    gui/camerawidget.h \
    decisionMaking/functions.h \
    trafficlight/trafficlight.h \
    lidar16/velodyne.h \
    Logo/logo.h \
    driver/lowlayer.h \
    driver/gear.h \
    driver/mainwindow.h \
    driver/remoteCtrl.h \
    driver/steering.h \
    driver/thread.h \
    driver/throttle.h \
    driver/transmission.h \
    driver/usbcan.h

FORMS    += mainwindow.ui \
    driver/mainwindow.ui
#-------------------------------------------------------------------
INCLUDEPATH += $$PWD/thread #must be added
DEPENDPATH += $$PWD/thread

INCLUDEPATH += $$PWD/lowerlayer #must be added
DEPENDPATH += $$PWD/lowerlayer

INCLUDEPATH += $$PWD/sourceCode #must be added
DEPENDPATH += $$PWD/sourceCode

INCLUDEPATH += $$PWD/lidar4 #must be added
DEPENDPATH += $$PWD/lidar4

INCLUDEPATH += $$PWD/lidar4/ibeosdk #must be added
DEPENDPATH += $$PWD/lidar4/ibeosdk

INCLUDEPATH += $$PWD/lidar16 #must be added
DEPENDPATH += $$PWD/lidar16

INCLUDEPATH += $$PWD/gps #must be added
DEPENDPATH += $$PWD/gps

INCLUDEPATH += $$PWD/esr #must be added
DEPENDPATH += $$PWD/esr

INCLUDEPATH += $$PWD/decisionMaking #must be added
DEPENDPATH += $$PWD/decisiontMaking

INCLUDEPATH += $$PWD/gui #mudecisionMakingst be added
DEPENDPATH += $$PWD/gui

INCLUDEPATH += $$PWD/trafficlight
DEPENDPATH += $$PWD/trafficlight

INCLUDEPATH += $$PWD/Logo
DEPENDPATH += $$PWD/Logo


#---------------------------------------------------------------------------

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/can/release/ -lcanlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/can/debug/ -lcanlib
else:unix: LIBS += -L$$PWD/can/ -lcanlib

INCLUDEPATH += $$PWD/can
DEPENDPATH += $$PWD/can

INCLUDEPATH += $$PWD/can/include
DEPENDPATH += $$PWD/can/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lidar4/release/ -libeosdk_d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lidar4/debug/ -libeosdk_d
else:unix: LIBS += -L$$PWD/lidar4/ -libeosdk_d

INCLUDEPATH += $$PWD/lidar4
DEPENDPATH += $$PWD/lidar4

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lidar4/release/ -lboost_thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lidar4/debug/ -lboost_thread
else:unix: LIBS += -L$$PWD/lidar4/ -lboost_thread

INCLUDEPATH += $$PWD/lidar4
DEPENDPATH += $$PWD/lidar4

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lidar4/release/ -lboost_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lidar4/debug/ -lboost_system
else:unix: LIBS += -L$$PWD/lidar4/ -lboost_system

INCLUDEPATH += $$PWD/lidar4
DEPENDPATH += $$PWD/lidar4

#--------------------------------------------------------------------
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lopencv_highgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lopencv_highgui
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lopencv_core
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

SUBDIRS += \
    driver/driver.pro

OTHER_FILES += \
    driver/driver.pro.user \
    driver/driver.pro.user.3b73c3d \
    driver/driver.pro.user.cbca968 \
    driver/driver.pro.user.FC2404 \
    driver/driver.pro.user.tt3132

