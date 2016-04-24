#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <softPwm.h>
#include <cstdio>
#include "io.h"
#include "config.h"

static Output_Motor_SoftPWM* omotor;

void sfullhot() {
    omotor->hotflag = digitalRead(FULLHOTPIN);
    if (omotor->hotflag) softPwmWrite(HOTPIN, 0);
}
void sfullcold() {
    omotor->coldflag = digitalRead(FULLCOLDPIN);
    if (omotor->coldflag) softPwmWrite(COLDPIN, 0);
}

Output_Motor_SoftPWM::Output_Motor_SoftPWM() {
    omotor = this;
    hotflag = digitalRead(FULLHOTPIN);
    coldflag = digitalRead(FULLCOLDPIN);
    wiringPiISR(FULLHOTPIN, INT_EDGE_BOTH, sfullhot);
    wiringPiISR(FULLCOLDPIN, INT_EDGE_BOTH, sfullcold);
}

void Output_Motor_SoftPWM::on() {
    softPwmCreate(HOTPIN, 0, 100);
    softPwmCreate(COLDPIN, 0, 100);
    digitalWrite(STBYPIN, 1);
}
void Output_Motor_SoftPWM::off() {
    softPwmWrite(HOTPIN, 0);
    softPwmWrite(COLDPIN, 0);
    softPwmStop(HOTPIN);
    softPwmStop(COLDPIN);
    digitalWrite(STBYPIN, 0);
}

qint8 Output_Motor_SoftPWM::mod(double d) {
    if (d > 1) {
        if (hotflag) return 1;
        d /= 100.0;
        if (d > 1.0) d = 1.0;
        softPwmWrite(COLDPIN, 0);
        softPwmWrite(HOTPIN, 90 * d + 10);
    } else if (d < -1) {
        if (coldflag) return -1;
        d /= 100.0;
        if (d < -1.0) d = -1.0;
        softPwmWrite(HOTPIN, 0);
        softPwmWrite(COLDPIN, 90 * -d + 10);
    } else {
        softPwmWrite(HOTPIN, 0);
        softPwmWrite(COLDPIN, 0);
    }
    return 0;
}
