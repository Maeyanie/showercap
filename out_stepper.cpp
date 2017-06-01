#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include <QSettings>
#include "io.h"
#include "config.h"

#define CYCLETIME 250 // in ms
#define STEPTIME 5000 // in us
#define FULLHOT 10000
#define FULLCOLD 0
#define MAXSTEPS ((CYCLETIME-5)/((STEPTIME/1000)*2))

Output_Stepper::Output_Stepper() {
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(STBYPIN, 0);
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(SHOWERPIN, 0);
    digitalWrite(BATHPIN, 0);

    position = 0;
    duration = 0;
    onOff = 0;
    mode = 1;
//    QSettings settings("NMSoft", "Digital Shower Prototype");
//    position = settings.value("stepperpos", 0).toInt();

    printf("Output_Stepper initialized.\n");
}
void Output_Stepper::save() {
//    static QSettings settings("NMSoft", "Digital Shower Prototype");
//    settings.setValue("stepperpos", position);
}

void Output_Stepper::on() {
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(ENABLEPIN, 1);
    digitalWrite(STBYPIN, 1);

    if (mode == 1) {
        digitalWrite(SHOWERPIN, 1);
    } else {
        digitalWrite(BATHPIN, 1);
    }
    onOff = 1;

    digitalWrite(ONOFFPIN, 1);
    delay(50);
    digitalWrite(ONOFFPIN, 0);
}
void Output_Stepper::off() {
    onOff = 0;
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(ENABLEPIN, 1);
    digitalWrite(STBYPIN, 0);
    digitalWrite(SHOWERPIN, 0);
    digitalWrite(BATHPIN, 0);
}

void Output_Stepper::shower() {
    if (onOff) {
        digitalWrite(BATHPIN, 0);
        digitalWrite(SHOWERPIN, 1);
        digitalWrite(ONOFFPIN, 1);
        delay(50);
        digitalWrite(ONOFFPIN, 0);
    }
    mode = 1;
}
void Output_Stepper::bath() {
    if (onOff) {
        digitalWrite(SHOWERPIN, 0);
        digitalWrite(BATHPIN, 1);
        digitalWrite(ONOFFPIN, 1);
        delay(50);
        digitalWrite(ONOFFPIN, 0);
    }
    mode = 0;
}

void Output_Stepper::set(double v) {
    v *= 10.0;
    if (v >= position+1) {
        duration = ((v - position) * STEPTIME) / 1000 + 5;
        digitalWrite(DIRPIN, 1);
        digitalWrite(ENABLEPIN, 0);
        while (v >= position+1) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position++;
        }
        delay(5);
        digitalWrite(ENABLEPIN, 1);
    } else if (v <= position-1) {
        duration = ((position - v) * STEPTIME) / 1000 + 5;
        digitalWrite(DIRPIN, 0);
        digitalWrite(ENABLEPIN, 1);
        while (v <= position-1) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position--;
        }
        delay(5);
        digitalWrite(ENABLEPIN, 1);
    } else {
        duration = 0;
    }
    save();
}

qint8 Output_Stepper::mod(double d) {
    static double frac = 0.0;
    d += frac;

    if (d > 1.0) {
        if (d > MAXSTEPS) d = MAXSTEPS;
        duration = (d * STEPTIME) / 1000 + 5;
        digitalWrite(ENABLEPIN, 0);
        digitalWrite(DIRPIN, 1);
        for (int i = 0; i < d; i++) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position++;
        }
        delay(5);
        digitalWrite(ENABLEPIN, 1);
        frac = 0.0;
    } else if (d < -1.0) {
        if (d < -MAXSTEPS) d = -MAXSTEPS;
        duration = (-d * STEPTIME) / 1000 + 5;
        digitalWrite(ENABLEPIN, 0);
        digitalWrite(DIRPIN, 0);
        for (int i = 0; i < -d; i++) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position--;
        }
        delay(5);
        digitalWrite(ENABLEPIN, 1);
        frac = 0.0;
    } else {
        if (d > 0.01 || d < -0.01) frac = d;
        duration = 0;
    }
    save();
    return 0;
}

qint32 Output_Stepper::time(qint32 t) {
    return CYCLETIME-t-duration;
}
