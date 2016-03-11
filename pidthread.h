#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>

class PIDThread : public QThread
{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE;

signals:
    void update(qreal curTemp);
    void fullhot();
    void fullcold();

public:
    PIDThread(QObject* parent);
};

void ow_init();
double ow_read();

void i2csensor_init();
double i2csensor_read();

void thermistor_init();
double thermistor_read();

#endif // PIDTHREAD_H
