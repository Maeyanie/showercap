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
    MOTORSOFTPWM
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

#define ONOFFPIN 26

#define SHOWERPIN 23
#define BATHPIN 24

#define HOTPIN 13
#define COLDPIN 19
#define PWMPIN 18
#define STBYPIN 6

#define FULLHOTPIN 20
#define FULLCOLDPIN 21

#endif // CONFIG_H
