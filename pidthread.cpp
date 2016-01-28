#include "pidthread.h"
#include <QTextStream>

PIDThread::PIDThread(QObject* parent, QString fname) : QThread(parent)
{
    filename = fname;
}

void PIDThread::run()
{
    qreal curTemp = 0.0;
    QFile dev(filename);
    QString line;

    while (!QThread::currentThread()->isInterruptionRequested()) {
        if (dev.open(QIODevice::ReadOnly)) {
            QTextStream in(&dev);
            in.readLine();
            line = in.readLine();
            dev.close();

            curTemp = ((line.mid(29).toInt()+50)/100)/10.0;

            emit update(curTemp);
        }
    }
}
