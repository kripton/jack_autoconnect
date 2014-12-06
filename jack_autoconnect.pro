#-------------------------------------------------
#
# Project created by QtCreator 2014-12-06T13:03:48
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = jack_autoconnect
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    jackautoconnect.cpp

HEADERS += \
    jackautoconnect.h

LIBS += `pkg-config --cflags --libs jack`
