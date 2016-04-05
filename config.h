#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

enum SensorType {
    ONEWIRE,
    I2CSENSOR,
    THERMISTOR
};

enum OutputType {
    SERVO
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

#define ONOFFPIN 26

#define SHOWERPIN 23
#define BATHPIN 24

#define HOTPIN 12
#define COLDPIN 16
#define PWMPIN 18
#define STBYPIN 12

#define FULLHOTPIN 20
#define FULLCOLDPIN 21

#endif // CONFIG_H
