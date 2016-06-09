#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include <QSettings>
#include "io.h"
#include "config.h"

#define CYCLETIME 50
#define FULLHOT 10000
#define FULLCOLD 0

Output_Stepper::Output_Stepper() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    digitalWrite(STBYPIN, 0);

    QSettings settings("NMSoft", "Digital Shower Prototype");
    position = settings.value("stepperpos", 0).toInt();
}
void Output_Stepper::save() {
    static QSettings settings("NMSoft", "Digital Shower Prototype");
    settings.setValue("stepperpos", position);
}

void Output_Stepper::on() {
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    digitalWrite(STBYPIN, 1);
    digitalWrite(ONOFFPIN, 1);
}
void Output_Stepper::off() {
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(HOTPIN, 0);
    digitalWrite(COLDPIN, 0);
    digitalWrite(STBYPIN, 0);
}

void Output_Stepper::set(double v) {
    v *= 100.0;
    while (v >= position+1) {
        digitalWrite(HOTPIN, 1);
        delayMicroseconds(10);
        digitalWrite(HOTPIN, 0);
        delayMicroseconds(10);
        position++;
    }
    while (v <= position-1) {
        digitalWrite(COLDPIN, 1);
        delayMicroseconds(10);
        digitalWrite(COLDPIN, 0);
        delayMicroseconds(10);
        position--;
    }
    save();
}

qint8 Output_Stepper::mod(double d) {
    d *= 100;

    if (d > 10) {
        for (int i = 0; i < d; i++) {
            digitalWrite(HOTPIN, 1);
            delayMicroseconds(10);
            digitalWrite(HOTPIN, 0);
            delayMicroseconds(10);
            position++;
        }
    } else if (d < -10) {
        for (int i = 0; i < -d; i++) {
            digitalWrite(COLDPIN, 1);
            delayMicroseconds(10);
            digitalWrite(COLDPIN, 0);
            delayMicroseconds(10);
            position--;
        }
    }
    save();
    return 0;
}

qint32 Output_Stepper::time(qint32 t) {
    return CYCLETIME-t;
}
