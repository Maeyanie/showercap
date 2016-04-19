#include "config.h"
#include <QFile>
#include <QTextStream>
#include <QDialog>

Config config;

Config::Config()
{
    // Defaults
    config.sensorType = THERMISTOR;
    config.sensorFile = "";
    config.maxTemp = 450;
    config.outputType = MOTORSOFTPWM;

    QFile file("config.txt");
    QString line;

    if (!file.open(QIODevice::ReadOnly)) {
        QTextStream(stderr) << "[Config] Error: Could not open config.txt" << endl;
        return;
    }

    QTextStream in(&file);
    while (!(line = in.readLine()).isNull()) {
        if (line == "" || line.startsWith("#")) continue;
        QStringList parts = line.split(QRegExp("\\s*=\\s*"));
        if (parts.length() != 2) {
            QTextStream(stderr) << "[Config] Warning: Invalid line '" << line << "' parsing config file." << endl;
            continue;
        }

        if (parts.at(0) == "MaxTemp") {
            maxTemp = parts.at(1).toInt();
            QTextStream(stdout) << "[Config] Set MaxTemp to " << maxTemp << endl;
        } else if (parts.at(0) == "SensorFile") {
            sensorFile = parts.at(1);
            QTextStream(stdout) << "[Config] Set SensorFile to '" << sensorFile << "'" << endl;
        } else if (parts.at(0) == "SensorType") {
            QString type = parts.at(1).toUpper();
            if (type == "ONEWIRE") {
                config.sensorType = ONEWIRE;
                QTextStream(stdout) << "[Config] Set SensorType to ONEWIRE'" << endl;
            } else if (type == "I2CSENSOR") {
                config.sensorType = I2CSENSOR;
                QTextStream(stdout) << "[Config] Set SensorType to I2CSENSOR'" << endl;
            } else if (type == "THERMISTOR") {
                config.sensorType = THERMISTOR;
                QTextStream(stdout) << "[Config] Set SensorType to THERMISTOR'" << endl;
            } else {
                QTextStream(stderr) << "[Config] Warning: Unrecognized sensorType '" << type << "'" << endl;
            }
        }
    }
    file.close();
}
