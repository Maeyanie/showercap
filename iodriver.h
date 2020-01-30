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
class Input_ADS1115 : public Input {
    qint32 dev;
    qint32 ref;
public:
	Input_ADS1115();
    virtual double read();
};
class Input_LTC2451 : public Input {
	qint32 dev;
public:
	Input_LTC2451();
	virtual double read();
};




class Output {
public:
    virtual ~Output() {}
    virtual void on() {}
    virtual void off() {}
    virtual void set(double) =0;
    virtual qint8 mod(double) =0;
    virtual qreal get() { return 0.0; }
    virtual qint32 time(qint32) { return 10; }
    virtual void setMin(double) {}
    virtual void setMax(double) {}
    virtual double getMin() { return -INFINITY; }
    virtual double getMax() { return INFINITY; }
    virtual void limits(bool) {}
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
    qreal get() { return position; }
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
protected:
    void save();
    qint32 position, minpos, maxpos;
    int duration;
    bool onOff, useLimits;

public:
    Output_Stepper();
    ~Output_Stepper();
    void on();
    void off();
    void set(double);
    qint8 mod(double);
    qreal get() { return position; }
    qint32 time(qint32);
    void setMin(double);
    void setMax(double);
    double getMin();
    double getMax();
    void limits(bool);
};



class OnOff {
public:
    virtual ~OnOff() {}
    virtual void on() =0;
    virtual void off() =0;
    virtual void shower() {}
    virtual void bath() {}
};

class OnOff_DoubleSolenoid : public OnOff {
protected:
    bool onOff;
    bool mode;

public:
    OnOff_DoubleSolenoid();
    ~OnOff_DoubleSolenoid();
    void on();
    void off();
    void shower();
    void bath();
};

#endif // IO_H
