#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    Config();

    QString sensorFile;
    qint32 maxTemp;
};

extern Config config;

#define ONOFFPIN 24

#endif // CONFIG_H
