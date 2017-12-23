#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include "io.h"

class PIDThread : public QThread
{
    Q_OBJECT

    class Input* input;
    class Output* output;
    class OnOff* onOff;

    void run() Q_DECL_OVERRIDE;

signals:
    void update(qreal curTemp);
	void noHotWater();
    void fullhot();
    void fullcold();

public:
    PIDThread(QObject* parent);
};

#endif // PIDTHREAD_H
