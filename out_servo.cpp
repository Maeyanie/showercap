#include <QTypeInfo>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include "pidthread.h"

#define SERVO 0x40

Output_Servo::Output_Servo() {
    dev = wiringPiI2CSetup(SERVO);
    if (dev == -1) { fprintf(stderr, "Error opening I2C servo controller: %m\n"); exit(1); }

    position = 50;

    // Servo wants 50 Hz
    // prescale = round(25 MHz / (4096 * 50 Hz))-1
    wiringPiI2CWriteReg8(dev, 0xFE, 121);
}

void Output_Servo::on() {
    wiringPiI2CWriteReg8(dev, 0x08, 0);
    wiringPiI2CWriteReg8(dev, 0x09, 0);
    wiringPiI2CWriteReg8(dev, 0x00, 0b00000001);
    delay(1);
}
void Output_Servo::off() {
    wiringPiI2CWriteReg8(dev, 0x00, 0b00010001);
}

qint8 Output_Servo::mod(double value) {
    qint8 ret = 0;
    position += value;
    if (position >= 100.0) {
        position = 100.0;
        ret = 1;
    } else if (position <= 0.0) {
        position = 0.0;
        ret = -1;
    }

    // 100 = 2 ms pulse
    // 50 = 1.5 ms pulse
    // 0 = 1 ms pulse

    // each PWM is 20 ms
    // 1 ms is 204.8 counts
    qint32 t = (qint32)(value * 2.048 + 204.8 + 0.5);

    wiringPiI2CWriteReg8(dev, 0x08, (t & 0xFF));
    wiringPiI2CWriteReg8(dev, 0x09, (t & 0xFF00) >> 8);

    return ret;
}
