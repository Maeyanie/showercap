#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include "io.h"
#include "config.h"

Output_Motor::Output_Motor() {
}

void Output_Motor::on() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
}
void Output_Motor::off() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
}

qint8 Output_Motor::mod(double d) {
    if (d > 1) {
        d /= 100.0;
        if (d > 1.0) d = 1.0;
        digitalWrite(COLDPIN, 0);
        pwmWrite(PWMPIN, 512 * d);
        digitalWrite(HOTPIN, 1);
    } else if (d < -1) {
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
}

qint32 Output_Motor::time(qint32 t) {
    return std::max(10-t, 0);
}
