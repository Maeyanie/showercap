#include "pidthread.h"
#include "mainwindow.h"
#include "config.h"
#include <QTextStream>

PIDThread::PIDThread(QObject* parent) : QThread(parent)
{
}

static qint32 pwmVal(qreal output) {
    output = abs(output);

    qint32 ret = output * 100;

    if (output > 1024) output = 1024;
    else if (output < 0) output = 0;
    return ret;
}

void PIDThread::run()
{
    QDateTime now, last = QDateTime::currentDateTime();
    qreal error, integral, derivative, value;
    qreal setTemp = 40.5, curTemp = 0.0;
    qreal preError = 0.0;
    qreal Dt = 0.8, Kp = 2.0, Ki = 0.0, Kd = 0.0;
    qreal iMax = 1.0, iMin = -1.0;
    bool on = 0;

    Input* input;
    Output* output;

    switch (config.sensorType) {
    case ONEWIRE:
        input = new Input_Onewire();
        break;
    case I2CSENSOR:
        input = new Input_I2CSensor();
        break;
    case THERMISTOR:
        input = new Input_Thermistor();
    }
    switch (config.outputType) {
    case SERVO:
        output = new Output_Servo();
    }

    while (!QThread::currentThread()->isInterruptionRequested()) {
        curTemp = input->read();
        emit update(curTemp);

        now = QDateTime::currentDateTime();

        if (!(((MainWindow*)this->parent())->isOn())) {
            if (on) {
                output->off();
                on = 0;
            }
            last = now;
            delay(100);
            continue;
        }
        if (!on) {
            output->on();
            on = 1;
        }

        Dt = last.msecsTo(now);

        setTemp = ((MainWindow*)this->parent())->getSetTemp() / 10.0;

        error = (setTemp/10.0) - curTemp;

        // track error over time, scaled to the timer interval
        integral = integral + (error * Dt);
        if (integral > iMax) integral = iMax;
        else if (integral < iMin) integral = iMin;

        // determine the amount of change from the last time checked
        derivative = (error - preError) / Dt;

        // calculate how much to drive the output in order to get to the
        // desired setpoint.
        value = (Kp * error) + (Ki * integral) + (Kd * derivative);

        // remember the error for the next time around.
        preError = error;
        last = now;

        output->set(value);

        delay(10);
    }

    delete input;
    delete output;
}
