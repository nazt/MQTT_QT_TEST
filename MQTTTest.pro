#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T21:57:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MQTTTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../qmqtt/release/ -lqmqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qmqtt/debug/ -lqmqtt
else:unix: LIBS += -L$$PWD/../qmqtt/ -lqmqtt

INCLUDEPATH += $$PWD/../qmqtt
DEPENDPATH += $$PWD/../qmqtt
