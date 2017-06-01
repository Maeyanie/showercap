#include "pidthread.h"
#include "mainwindow.h"
#include "config.h"
#include <QTextStream>

PIDThread::PIDThread(QObject* parent) : QThread(parent)
{
}

void PIDThread::run()
{
    MainWindow* mw = (MainWindow*)this->parent();
    QDateTime start, now, last = QDateTime::currentDateTime();
    qreal error, integral = 0.0, derivative, value;
    qreal setTemp = 40.5, curTemp = 0.0;
    qreal preError = 0.0;
    qreal Dt = 0.0, Kp = 5.0, Ki = 0.0, Kd = 0.1;
    qreal iMax = 1.0, iMin = -1.0;
    qint32 d;
    bool on = 0;

    input = mw->input;
    output = mw->output;

    while (!QThread::currentThread()->isInterruptionRequested()) {
        curTemp = input->read();
        emit update(curTemp);

        now = QDateTime::currentDateTime();

        if (!mw->isOn()) {
            if (on) {
                output->off();
                on = 0;
            }
            last = now;
            delay(100);
            continue;
        }
        if (!on) {
            start = QDateTime::currentDateTime();
            output->on();
            on = 1;
        }

        if (start.msecsTo(now) < 10000) { // 10-second warmup time
            last = now;
            delay(100);
            continue;
        }

        Dt = last.msecsTo(now) / 1000.0;

        setTemp = ((MainWindow*)this->parent())->getSetTemp() / 10.0;

        error = setTemp - curTemp;

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

        printf("pidthread: setTemp=%f curTemp=%f error=%f p=%f i=%f d=%f\n",
            setTemp, curTemp, error, Kp*error, Ki*integral, Kp*derivative);

        switch (output->mod(value)) {
        case 1:
            emit fullhot();
            break;
        case -1:
            emit fullcold();
        }

        d = output->time(input->time());
        if (d > 0) delay(d);
    }

    delete input;
    delete output;
}
