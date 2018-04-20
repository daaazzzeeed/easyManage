#-------------------------------------------------
#
# Project created by QtCreator 2017-11-29T13:13:27
#
#-------------------------------------------------

QT       += core gui sql
#test
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = brtrMpiControl
TEMPLATE = app


SOURCES += src/main.cpp\
        src/brtrcontrol.cpp

HEADERS  += src/brtrcontrol.h

FORMS    += ui/brtrcontrol.ui

include ($$PWD/../../../../include/kpa_app_inc.pri)
