#!/bin/sh

qmake &&
make &&
sudo chown root.root Shower &&
sudo chmod u+s Shower &&

g++ -Wall -fPIE -o test test.cpp in_thermistor.o out_stepper.o \
	-I/usr/include/arm-linux-gnueabihf/qt5 -I/usr/include/arm-linux-gnueabihf/qt5/QtCore -lwiringPi

#g++ -Wall -fPIE -o manual manual.cpp in_thermistor.o out_stepper.o \
#	-I/usr/include/arm-linux-gnueabihf/qt5 -I/usr/include/arm-linux-gnueabihf/qt5/QtCore -lwiringPi -lpthread -lncurses
