#include <QTypeInfo>
#include <wiringPiI2C.h>
#include <cstdio>

#define THERMOMETER 0x18

static qint32 dev;

void i2csensor_init() {
    dev = wiringPiI2CSetup(THERMOMETER);
    if (dev == -1) { fprintf(stderr, "Error opening I2C sensor: %m\n"); exit(1); }
}

double i2csensor_read() {
    qint32 data;

    data = wiringPiI2CReadReg16(dev, 0x05);
    data = ((data & 0xFF00) >> 8) | ((data & 0x001F << 8)); // Byteswap and clear flag bits

    if (data & 0x1000) {
        return 256 - (data / 16.0);
    } else {
        return data / 16.0;
    }
}
