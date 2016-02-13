#include "pidthread.h"
#include "mainwindow.h"
#include "config.h"
#include <QTextStream>
#include <wiringPiI2C.h>

PIDThread::PIDThread(QObject* parent, QString fname) : QThread(parent)
{
    filename = fname;
}

void PIDThread::run()
{
#ifdef ONEWIRE
    QFile dev(filename);
    QString line;
#else
    qint32 dev = wiringPiI2CSetup(THERMOMETER);
    if (dev == -1) { fprintf(stderr, "Error opening I2C sensor: %m\n"); exit(1); }
    qint32 data;
#endif
    QDateTime now, last = QDateTime::currentDateTime();
    qreal error, integral, derivative, output;
    qreal setTemp = 40.5, curTemp = 0.0;
    qreal preError = 0.0;
    qreal Dt = 0.8, Kp = 2.0, Ki = 0.0, Kd = 0.0;
    qreal iMax = 1.0, iMin = -1.0;

    while (!QThread::currentThread()->isInterruptionRequested()) {
#ifdef ONEWIRE
        if (dev.open(QIODevice::ReadOnly)) {
            QTextStream in(&dev);
            in.readLine();
            line = in.readLine();
            dev.close();

            curTemp = ((line.mid(29).toInt()+50)/100)/10.0;
        } else {
            sleep(1);
            continue;
        }
#else
        data = wiringPiI2CReadReg16(dev, 0x05);
        data = ((data & 0xFF00) >> 8) | ((data & 0x001F << 8)); // Byteswap and clear flag bits

        if (data & 0x1000) {
            curTemp = 256 - (data / 16.0);
        } else {
            curTemp = data / 16.0;
        }
#endif

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

        delay(250);
    }
}
