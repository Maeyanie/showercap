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
    config.cpp \
    in_i2csensor.cpp \
    in_onewire.cpp \
    in_thermistor.cpp \
    out_servo.cpp \
    out_motor.cpp \
    settempdialog.cpp \
    out_motor_softpwm.cpp \
    out_stepper.cpp \
    tab_shower.cpp \
    tab_bath.cpp \
    tab_manual.cpp \
    qpresetbutton.cpp \
    tab_tools.cpp

HEADERS  += mainwindow.h \
    pidthread.h \
    config.h \
    spline.h \
    io.h \
    settempdialog.h \
    rt_usp10972.h \
    rt_ppg102a1.h \
    rt_pr103j2.h \
    qpresetbutton.h

FORMS    += mainwindow.ui \
    settempdialog.ui

win32: LIBS += -LD:/Devel/build/wiringPiFake/lib/
win32: INCLUDEPATH += D:/Devel/build/wiringPiFake/include
win32: DEPENDPATH += D:/Devel/build/wiringPiFake/include

LIBS += -lwiringPi

DISTFILES += \
    update.sh

RESOURCES +=
