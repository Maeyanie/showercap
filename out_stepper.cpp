#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include <QSettings>
#include "io.h"
#include "config.h"

#define CYCLETIME 50
#define STEPTIME 1000
#define FULLHOT 10000
#define FULLCOLD 0

Output_Stepper::Output_Stepper() {
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(STBYPIN, 0);

    QSettings settings("NMSoft", "Digital Shower Prototype");
    position = settings.value("stepperpos", 0).toInt();
}
void Output_Stepper::save() {
    static QSettings settings("NMSoft", "Digital Shower Prototype");
    settings.setValue("stepperpos", position);
}

void Output_Stepper::on() {
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(STBYPIN, 1);
    digitalWrite(ONOFFPIN, 1);
    delay(1);
}
void Output_Stepper::off() {
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(STEPPIN, 0);
    digitalWrite(DIRPIN, 0);
    digitalWrite(STBYPIN, 0);
}

void Output_Stepper::set(double v) {
    v *= 100.0;
    if (v >= position+1) {
        digitalWrite(DIRPIN, 1);
        while (v >= position+1) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position++;
        }
    } else if (v <= position-1) {
        digitalWrite(DIRPIN, 0);
        while (v <= position-1) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position--;
        }
    }
    save();
}

qint8 Output_Stepper::mod(double d) {
    d *= 100.0;

    if (d > 1) {
        digitalWrite(DIRPIN, 1);
        for (int i = 0; i < d; i++) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position++;
        }
    } else if (d < -1) {
        digitalWrite(DIRPIN, 0);
        for (int i = 0; i < -d; i++) {
            digitalWrite(STEPPIN, 1);
            delayMicroseconds(STEPTIME);
            digitalWrite(STEPPIN, 0);
            delayMicroseconds(STEPTIME);
            position--;
        }
    }
    save();
    return 0;
}

qint32 Output_Stepper::time(qint32 t) {
    return CYCLETIME-t;
}
