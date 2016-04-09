#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include "io.h"
#include "config.h"

static Output_Motor* omotor;

void fullhot() {
    omotor->fullhot = digitalRead(FULLHOTPIN);
    if (omotor->fullhot) digitalWrite(HOTPIN, 0);
}
void fullcold() {
    omotor->fullcold = digitalRead(FULLCOLDPIN);
    if (omotor->fullcold) digitalWrite(COLDPIN, 0);
}

Output_Motor::Output_Motor() {
    omotor = this;
    wiringPiISR(FULLHOTPIN, INT_EDGE_BOTH, fullhot);
    wiringPiISR(FULLCOLDPIN, INT_EDGE_BOTH, fullcold);
}

void Output_Motor::on() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
    digitalWrite(STBYPIN, 1);
}
void Output_Motor::off() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
    digitalWrite(STBYPIN, 0);
}

qint8 Output_Motor::mod(double d) {
    if (d > 1) {
        if (fullhot) return 1;
        d /= 100.0;
        if (d > 1.0) d = 1.0;
        digitalWrite(COLDPIN, 0);
        pwmWrite(PWMPIN, 512 * d);
        digitalWrite(HOTPIN, 1);
    } else if (d < -1) {
        if (fullcold) return -1;
        d /= 100.0;
        if (d < -1.0) d = -1.0;
        digitalWrite(HOTPIN, 0);
        pwmWrite(PWMPIN, 512 * -d);
        digitalWrite(COLDPIN, 1);
    } else {
        digitalWrite(HOTPIN, 0);
        digitalWrite(COLDPIN, 0);
        pwmWrite(PWMPIN, 0);
    }
    return 0;
}

qint32 Output_Motor::time(qint32 t) {
    return std::max(10-t, 0);
}
