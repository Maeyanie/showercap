#include <QFile>
#include <QString>
#include <QTextStream>
#include <cmath>
#include <config.h>

static QFile dev;

void ow_init() {
    dev.setFileName(config.sensorFile);
}

double ow_read() {
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
