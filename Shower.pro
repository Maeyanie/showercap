#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T00:01:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shower
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pidthread.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    pidthread.h \
    config.h

FORMS    += mainwindow.ui
