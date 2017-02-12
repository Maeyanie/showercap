#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include <QSettings>
#include "io.h"
#include "config.h"

#define CYCLETIME 100
#define STEPTIME 5000
#define FULLHOT 10000
#define FULLCOLD 0

Output_Stepper::Output_Stepper() {
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(STBYPIN, 0);

    position = 0;
    duration = 0;
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
    digitalWrite(ONOFFPIN, 1);
    delay(1);
}
void Output_Stepper::off() {
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(ENABLEPIN, 1);
    digitalWrite(STBYPIN, 0);
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
    d *= 10.0;

    if (d > 1) {
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
    } else if (d < -1) {
        duration = (d * STEPTIME) / 1000 + 5;
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
    } else {
        duration = 0;
    }
    save();
    return 0;
}

qint32 Output_Stepper::time(qint32 t) {
    return CYCLETIME-t-duration;
}
