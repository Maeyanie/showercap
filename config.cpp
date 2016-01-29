#include "config.h"
#include <QFile>
#include <QTextStream>
#include <QDialog>

Config config;

Config::Config()
{
    QFile file("config.txt");
    QString line;

    if (!file.open(QIODevice::ReadOnly)) {
        QTextStream(stderr) << "Error: Could not open config.txt" << endl;
        exit(1);
    }

    QTextStream in(&file);
    while ((line = in.readLine()) != NULL) {
        if (line == "" || line.startsWith("#")) continue;
        QStringList parts = line.split(QRegExp("\\s*=\\s*"));
        if (parts.length() != 2) {
            QTextStream(stderr) << "Warning: Invalid line '" << line << "' parsing config file." << endl;
            continue;
        }

        if (parts.at(0) == "MaxTemp") {
            maxTemp = parts.at(1).toInt();
        } else if (parts.at(0) == "SensorFile") {
            sensorFile = parts.at(1);
        }
    }
    file.close();
}
