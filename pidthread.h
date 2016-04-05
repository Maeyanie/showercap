#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>

class PIDThread : public QThread
{
    Q_OBJECT

    class Input* input;
    class Output* output;

    void run() Q_DECL_OVERRIDE;

signals:
    void update(qreal curTemp);
    void fullhot();
    void fullcold();

public:
    PIDThread(QObject* parent);
};


class Input {
public:
    virtual ~Input() {};
    virtual double read() =0;
    virtual qint32 time() { return 0; }
};
class Input_Onewire : public Input {
    QFile dev;
public:
    Input_Onewire();
    virtual double read();
};
class Input_I2CSensor : public Input {
    qint32 dev;
public:
    Input_I2CSensor();
    virtual double read();
};
class Input_Thermistor : public Input {
    qint32 dev;
public:
    Input_Thermistor();
    virtual double read();
    virtual qint32 time() { return 16; }
};


class Output {
public:
    virtual ~Output() {};
    virtual void on() =0;
    virtual void off() =0;
    virtual void set(double) =0;
    virtual qint8 mod(double) =0;
    virtual qint32 time(qint32) { return 10; }
};
class Output_Servo : public Output {
    qint32 dev;
    double position;
public:
    Output_Servo();
    void on();
    void off();
    void set(double);
    qint8 mod(double);
    qint32 time(qint32);
};

#endif // PIDTHREAD_H
