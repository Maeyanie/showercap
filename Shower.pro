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
    config.cpp \
    in_i2csensor.cpp \
    in_onewire.cpp \
    out_servo.cpp \
    out_motor.cpp \
    settempdialog.cpp \
    out_motor_softpwm.cpp \
    out_stepper.cpp \
    tab_shower.cpp \
    tab_bath.cpp \
    tab_manual.cpp \
    qpresetbutton.cpp \
    tab_tools.cpp \
    onoff_doublesolenoid.cpp \
    settings.cpp \
    in_ads1115.cpp \
    in_ltc2451.cpp \
    outthread.cpp \
    inthread.cpp

HEADERS  += mainwindow.h \
    config.h \
    spline.h \
    settempdialog.h \
    rt_usp10972.h \
    rt_ppg102a1.h \
    rt_pr103j2.h \
    qpresetbutton.h \
    settings.h \
    iodriver.h \
    iothread.h

FORMS    += mainwindow.ui \
    settempdialog.ui

win32: LIBS += -LD:/Devel/build/wiringPiFake/lib/
win32: INCLUDEPATH += D:/Devel/build/wiringPiFake/include
win32: DEPENDPATH += D:/Devel/build/wiringPiFake/include

LIBS += -lwiringPi

DISTFILES += \
    update.sh \
    showercap-overlay.dts \
    setup.sh \
    notes.txt

RESOURCES +=
