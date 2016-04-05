#include <QTypeInfo>
#include <wiringPiI2C.h>
#include <cstdio>
#include "pidthread.h"

#define THERMOMETER 0x18

Input_I2CSensor::Input_I2CSensor() {
    dev = wiringPiI2CSetup(THERMOMETER);
    if (dev == -1) { fprintf(stderr, "Error opening I2C sensor: %m\n"); exit(1); }
}

double Input_I2CSensor::read() {
    qint32 data;

    data = wiringPiI2CReadReg16(dev, 0x05);
    data = ((data & 0xFF00) >> 8) | ((data & 0x001F << 8)); // Byteswap and clear flag bits

    if (data & 0x1000) {
        return 256 - (data / 16.0);
    } else {
        return data / 16.0;
    }
}
