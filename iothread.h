#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include "iodriver.h"

extern inline void usleep(qint32 us)
{
    if (us <= 0) return;
#ifdef Q_OS_WIN
    QObject().thread()->usleep(us);
#else
    struct timespec ts = { us / 1000000, (us % 1000000) * 1000 };
    nanosleep(&ts, NULL);
#endif
}
extern inline void msleep(qint32 ms)
{
    if (ms <= 0) return;
#ifdef Q_OS_WIN
    QObject().thread()->usleep(ms*1000);
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

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
