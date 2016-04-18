#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include "io.h"
#include "config.h"

static Output_Motor* omotor;

void fullhot() {
    omotor->hotflag = digitalRead(FULLHOTPIN);
    if (omotor->hotflag) digitalWrite(HOTPIN, 0);
}
void fullcold() {
    omotor->coldflag = digitalRead(FULLCOLDPIN);
    if (omotor->coldflag) digitalWrite(COLDPIN, 0);
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

void Output_Motor::set(double v) {
    fprintf(stderr, "Error: Cannot set() on Output_Motor.\n");
    exit(1);
}

qint8 Output_Motor::mod(double d) {
    if (d > 1) {
        if (hotflag) return 1;
        d /= 100.0;
        if (d > 1.0) d = 1.0;
        digitalWrite(COLDPIN, 0);
        pwmWrite(PWMPIN, 768 * d + 256);
        digitalWrite(HOTPIN, 1);
    } else if (d < -1) {
        if (coldflag) return -1;
        d /= 100.0;
        if (d < -1.0) d = -1.0;
        digitalWrite(HOTPIN, 0);
        pwmWrite(PWMPIN, 768 * -d + 256);
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
