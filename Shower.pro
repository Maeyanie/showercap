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
    pidthread.cpp \
    config.cpp \
    thermistor.cpp \
    onewire.cpp \
    i2csensor.cpp \
    showermode.cpp \
    mainwindow.cpp \
    bathmode.cpp

HEADERS  += \
    pidthread.h \
    config.h \
    spline.h \
    showermode.h \
    mainwindow.h \
    bathmode.h

FORMS    += \
    showermode.ui \
    mainwindow.ui \
    bathmode.ui

win32: LIBS += -LD:/Devel/build/wiringPiFake/lib/
win32: INCLUDEPATH += D:/Devel/build/wiringPiFake/include
win32: DEPENDPATH += D:/Devel/build/wiringPiFake/include

LIBS += -lwiringPi
