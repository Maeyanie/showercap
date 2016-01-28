#include "pidthread.h"
#include "mainwindow.h"
#include <QTextStream>

PIDThread::PIDThread(QObject* parent, QString fname) : QThread(parent)
{
    filename = fname;

    connect((MainWindow*)parent, &MainWindow::newTemp, this, &PIDThread::newTemp);
}

void PIDThread::run()
{
    QFile dev(filename);
    QString line;
    QDateTime now, last = QDateTime::currentDateTime();
    qreal error, integral, derivative, output, Dt;
    qreal curTemp = 0.0;
    qreal preError = 0.0;
    qreal Kp = 1.0, Ki = 1.0, Kd = 1.0;

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

            error = (setTemp/10.0) - curTemp;

            // track error over time, scaled to the timer interval
            integral = integral + (error * Dt);

            // determine the amount of change from the last time checked
            derivative = (error - preError) / Dt;

            // calculate how much to drive the output in order to get to the
            // desired setpoint.
            output = (Kp * error) + (Ki * integral) + (Kd * derivative);

            // remember the error for the next time around.
            preError = error;
            last = now;
        }
    }
}

void PIDThread::newTemp(qint32 temp)
{
    setTemp = temp;
}
