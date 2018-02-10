#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include "iodriver.h"
#include "config.h"

static Output_Motor* omotor;
#define CYCLETIME 50
#define MINPWM 96
#define DEADBAND (1.0/250.0)

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
    hotflag = digitalRead(FULLHOTPIN);
    coldflag = digitalRead(FULLCOLDPIN);
    wiringPiISR(FULLHOTPIN, INT_EDGE_BOTH, fullhot);
    wiringPiISR(FULLCOLDPIN, INT_EDGE_BOTH, fullcold);
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
    digitalWrite(STBYPIN, 0);
}

void Output_Motor::on() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
    digitalWrite(STBYPIN, 1);
    digitalWrite(ONOFFPIN, 1);
}
void Output_Motor::off() {
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    pwmWrite(PWMPIN, 0);
    digitalWrite(STBYPIN, 0);
}

void Output_Motor::set(double v) {
	v = v;
    fprintf(stderr, "Error: Cannot set() on Output_Motor.\n");
    exit(1);
}

qint8 Output_Motor::mod(double d) {
    static int dir = 0;
    static int ease = 0;
    int v;

    if (d > 100) d = 100;
    else if (d < -100) d = -100;
    v = (512 * abs(d)) / 100;

    if (d > DEADBAND) {
        if (hotflag) return 1;

        if (dir != 1) {
            digitalWrite(COLDPIN, 0);
            digitalWrite(HOTPIN, 0);
            dir = 1;
            ease = -10;
        }
        if (ease < 0) {
            v = 0;
            ease++;
        } else if (ease == 0) {
            digitalWrite(HOTPIN, 1);
            v = 0;
            ease++;
        } else if (ease < 20) {
            v *= ease / 20.0;
            ease++;
        }

        printf("[Output_Motor] Hot: d=%f v=%d\n", d, v);
        if (v < MINPWM) {
            int t = (double)(CYCLETIME*1000.0) * ((double)v/(double)MINPWM);
            pwmWrite(PWMPIN, MINPWM);
            delayMicroseconds(t);
            pwmWrite(PWMPIN, 0);
            delayMicroseconds((CYCLETIME*1000)-t);
        } else {
            pwmWrite(PWMPIN, v);
            delay(CYCLETIME);
        }
    } else if (d < -DEADBAND) {
        if (coldflag) return -1;

        if (dir != -1) {
            digitalWrite(COLDPIN, 0);
            digitalWrite(HOTPIN, 0);
            dir = -1;
            ease = -10;
        }
        if (ease < 0) {
            v = 0;
            ease++;
        } else if (ease == 0) {
            digitalWrite(COLDPIN, 1);
            v = 0;
            ease++;
        } else if (ease < 20) {
            v *= ease / 20.0;
            ease++;
        }

        printf("[Output_Motor] Cold: d=%f v=%d\n", d, v);
        if (v < MINPWM) {
            int t = (double)(CYCLETIME*1000.0) * ((double)v/(double)MINPWM);
            pwmWrite(PWMPIN, MINPWM);
            delayMicroseconds(t);
            pwmWrite(PWMPIN, 0);
            delayMicroseconds((CYCLETIME*1000)-t);
        } else {
            pwmWrite(PWMPIN, v);
            delay(CYCLETIME);
        }
    } else {
        if (dir != 0) {
            digitalWrite(HOTPIN, 0);
            digitalWrite(COLDPIN, 0);
            pwmWrite(PWMPIN, 0);
        }
        delay(CYCLETIME);
    }
    return 0;
}

qint32 Output_Motor::time(qint32 t) {
	t = t;
    return 0;
}
