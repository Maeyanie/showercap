#include "pidthread.h"
#include "showermode.h"
#include "config.h"
#include <QTextStream>

PIDThread::PIDThread(QObject* parent) : QThread(parent)
{
}

void PIDThread::run()
{
    QDateTime now, last = QDateTime::currentDateTime();
    qreal error, integral, derivative, output;
    qreal setTemp = 40.5, curTemp = 0.0;
    qreal preError = 0.0;
    qreal Dt = 0.8, Kp = 2.0, Ki = 0.0, Kd = 0.0;
    qreal iMax = 1.0, iMin = -1.0;

    switch (config.sensorType) {
    case ONEWIRE:
        ow_init();
        break;
    case I2CSENSOR:
        i2csensor_init();
        break;
    case THERMISTOR:
        thermistor_init();
    }

    while (!QThread::currentThread()->isInterruptionRequested()) {
        switch (config.sensorType) {
        case ONEWIRE:
            curTemp = ow_read();
            break;
        case I2CSENSOR:
            curTemp = i2csensor_read();
            break;
        case THERMISTOR:
            curTemp = thermistor_read();
        }

        emit update(curTemp);

        now = QDateTime::currentDateTime();
        Dt = last.msecsTo(now);

        setTemp = ((ShowerMode*)this->parent())->getSetTemp() / 10.0;

        error = (setTemp/10.0) - curTemp;

        // track error over time, scaled to the timer interval
        integral = integral + (error * Dt);
        if (integral > iMax) integral = iMax;
        else if (integral < iMin) integral = iMin;

        // determine the amount of change from the last time checked
        derivative = (error - preError) / Dt;

        // calculate how much to drive the output in order to get to the
        // desired setpoint.
        output = (Kp * error) + (Ki * integral) + (Kd * derivative);

        // remember the error for the next time around.
        preError = error;
        last = now;

        delay(250);
    }
}
