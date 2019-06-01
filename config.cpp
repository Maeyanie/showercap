#include "config.h"
#include <QFile>
#include <QTextStream>
#include <QDialog>

Config config;

#ifndef ETCDIR
#define ETCDIR "."
#endif

Config::Config()
{
    // Defaults
	config.sensorType = ADS1115;
    config.sensorFile = "";
	config.outputType = STEPPER;
	config.onOffType = DOUBLESOLENOID;
	config.settingsType = QSETTINGS;
	config.maxTemp = 450;
    config.Kp = 5.0;
    config.Ki = 0.0;
    config.Kd = 0.1;
    config.warmupTime = 15;
    config.timeStr = "h:mm A";
	config.dateStr = "ddd, MMM d, yyyy";

    QFile file(ETCDIR "/showercap.conf");
    QString line;

    if (!file.open(QIODevice::ReadOnly)) {
        QTextStream(stderr) << "[Config] Warning: Could not open " ETCDIR "/showercap.conf. Using defaults." << endl;
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

		QString key = parts.at(0);
		if (key == "MaxTemp") {
            maxTemp = parts.at(1).toInt();
            QTextStream(stdout) << "[Config] Set MaxTemp to " << maxTemp << endl;
		} else if (key == "SensorFile") {
            sensorFile = parts.at(1);
            QTextStream(stdout) << "[Config] Set SensorFile to '" << sensorFile << "'" << endl;
		} else if (key == "SensorType") {
            QString type = parts.at(1).toUpper();
            if (type == "ONEWIRE") {
                config.sensorType = ONEWIRE;
                QTextStream(stdout) << "[Config] Set SensorType to ONEWIRE'" << endl;
            } else if (type == "I2CSENSOR") {
                config.sensorType = I2CSENSOR;
                QTextStream(stdout) << "[Config] Set SensorType to I2CSENSOR'" << endl;
			} else if (type == "ADS1115") {
				config.sensorType = ADS1115;
				QTextStream(stdout) << "[Config] Set SensorType to ADS1115'" << endl;
			} else if (type == "LTC2451") {
				config.sensorType = LTC2451;
				QTextStream(stdout) << "[Config] Set SensorType to LTC2451'" << endl;
            } else {
				QTextStream(stderr) << "[Config] Warning: Unrecognized SensorType '" << type << "'" << endl;
            }
		} else if (key == "SettingsType") {
			QString type = parts.at(1).toUpper();
			if (type == "FILE") {
				config.settingsType = QSETTINGS;
				QTextStream(stdout) << "[Config] Set SettingsType to FILE'" << endl;
			} else if (type == "FRAM") {
				config.settingsType = FRAM;
				QTextStream(stdout) << "[Config] Set SettingsType to FRAM'" << endl;
			} else {
				QTextStream(stderr) << "[Config] Warning: Unrecognized SettingsType '" << type << "'" << endl;
			}
		} else if (key == "P") {
            config.Kp = parts.at(1).toDouble();
            QTextStream(stdout) << "[Config] Set P to " << config.Kp << endl;
		} else if (key == "I") {
            config.Ki = parts.at(1).toDouble();
            QTextStream(stdout) << "[Config] Set I to " << config.Ki << endl;
		} else if (key == "D") {
            config.Kd = parts.at(1).toDouble();
            QTextStream(stdout) << "[Config] Set D to " << config.Kd << endl;
        } else if (key == "WarmupTime") {
            config.warmupTime = parts.at(1).toInt();
            QTextStream(stdout) << "[Config] Set WarmupTime to " << config.warmupTime << endl;
		} else if (key == "TimeFormat") {
			config.timeStr = parts.at(1);
			QTextStream(stdout) << "[Config] Set TimeFormat to " << config.timeStr << endl;
		} else if (key == "DateFormat") {
			config.dateStr = parts.at(1);
			QTextStream(stdout) << "[Config] Set DateFormat to " << config.dateStr << endl;
		}
    }
    file.close();
}
