#ifndef IO_H
#define IO_H

#include <QFile>

class Input {
public:
    virtual ~Input() {}
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
};


class Output {
public:
    virtual ~Output() {}
    virtual void on() {}
    virtual void off() {}
    virtual void shower() {}
    virtual void bath() {}
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

class Output_Motor : public Output {
protected:
    bool hotflag, coldflag;

public:
    Output_Motor();
    void on();
    void off();
    void set(double);
    qint8 mod(double);
    qint32 time(qint32);

    friend void fullhot();
    friend void fullcold();
};
class Output_Motor_SoftPWM : public Output_Motor {
public:
    Output_Motor_SoftPWM();
    void on();
    void off();
    qint8 mod(double);

    friend void sfullhot();
    friend void sfullcold();
};

class Output_Stepper : public Output {
    void save();
    double position;
    int duration;
    bool onOff;
    bool mode;

public:
    Output_Stepper();
    void on();
    void off();
    void shower();
    void bath();
    void set(double);
    qint8 mod(double);
    qint32 time(qint32);
};

#endif // IO_H
