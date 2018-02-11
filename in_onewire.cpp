#include <QFile>
#include <QString>
#include <QTextStream>
#include <cmath>
#include "config.h"
#include "iothread.h"

Input_Onewire::Input_Onewire() {
    dev.setFileName(config.sensorFile);
}

double Input_Onewire::read() {
    QString line;

    if (dev.open(QIODevice::ReadOnly)) {
        QTextStream in(&dev);
        in.readLine();
        line = in.readLine();
        dev.close();

        return ((line.mid(29).toInt()+50)/100)/10.0;
    }

    return NAN;
}
