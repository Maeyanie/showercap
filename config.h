#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

enum SensorType {
    ONEWIRE,
    I2CSENSOR,
    THERMISTOR
};

class Config
{
public:
    Config();

    SensorType sensorType;
    QString sensorFile;
    qint32 maxTemp;
};

extern Config config;

#define ONOFFPIN 26

#endif // CONFIG_H
