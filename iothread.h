#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include "iodriver.h"

class InThread : public QThread
{
	Q_OBJECT

	void run() Q_DECL_OVERRIDE;

signals:
	void update(qreal curTemp);

public:
	InThread(QObject* parent);
};

class OutThread : public QThread
{
    Q_OBJECT

    class Output* output;
    class OnOff* onOff;

    void run() Q_DECL_OVERRIDE;

signals:
	void noHotWater();
    void fullhot();
    void fullcold();

public:
	OutThread(QObject* parent);
};

#endif // PIDTHREAD_H
