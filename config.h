#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

enum SensorType {
    ONEWIRE,
    I2CSENSOR,
    THERMISTOR
};

enum OutputType {
    SERVO,
    MOTOR,
    MOTORSOFTPWM,
    STEPPER
};

class Config
{
public:
    Config();

    SensorType sensorType;
    QString sensorFile;
    qint32 maxTemp;
    OutputType outputType;
};

extern Config config;

#define I2C_THERMOMETER 0x18
#define I2C_PWM 0x40
#define I2C_ADC 0x48

#define ONOFFPIN 25
#define SHOWERPIN 23
#define BATHPIN 24

#define HOTPIN 21
#define COLDPIN 20
#define STEPPIN 20
#define DIRPIN 21
#define PWMPIN 18
#define STBYPIN 12
#define ENABLEPIN 16

#define MS1PIN 17
#define MS2PIN 27
#define MS3PIN 22

#define FULLHOTPIN 27
#define FULLCOLDPIN 22

#endif // CONFIG_H
