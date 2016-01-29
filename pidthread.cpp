#include "pidthread.h"
#include "mainwindow.h"
#include <QTextStream>

PIDThread::PIDThread(QObject* parent, QString fname) : QThread(parent)
{
    filename = fname;
}

void PIDThread::run()
{
    QFile dev(filename);
    QString line;
    QDateTime now, last = QDateTime::currentDateTime();
    qreal error, integral, derivative, output;
    qreal setTemp = 40.5, curTemp = 0.0;
    qreal preError = 0.0;
    qreal Dt = 0.8, Kp = 2.0, Ki = 0.0, Kd = 0.0;
    qreal iMax = 1.0, iMin = -1.0;

    while (!QThread::currentThread()->isInterruptionRequested()) {
        if (dev.open(QIODevice::ReadOnly)) {
            QTextStream in(&dev);
            in.readLine();
            line = in.readLine();
            dev.close();

            curTemp = ((line.mid(29).toInt()+50)/100)/10.0;
            emit update(curTemp);

            now = QDateTime::currentDateTime();
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
            output = (Kp * error) + (Ki * integral) + (Kd * derivative);

            // remember the error for the next time around.
            preError = error;
            last = now;
        } else {
            sleep(1);
        }
    }
}
