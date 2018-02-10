#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>

// 8 kB FRAM, leaving a few bytes at the start for headers.
#define FRAMADDR 0x20
#define FRAMHEADER 64
#define FRAMPAGES 2
#define FRAMPAGESIZE 4064

class Settings {
public:
	virtual ~Settings() {}

	virtual void setValue(const QString& key, const QVariant& value) =0;
	virtual QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const =0;
	virtual void sync() =0;
	virtual void cork() =0;
	virtual void uncork() =0;
};

class FileSettings : public Settings {
public:
	FileSettings();
	virtual ~FileSettings();

	virtual void setValue(const QString& key, const QVariant& value);
	virtual QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
	virtual void cork();
	virtual void uncork();
	virtual void sync();

private:
	QSettings* qsettings;
};

class FRAMSettings : public Settings
{
public:
	FRAMSettings();
	virtual ~FRAMSettings();

	virtual void setValue(const QString& key, const QVariant& value);
	virtual QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
	virtual void cork();
	virtual void uncork();
	virtual void sync();

private:
	qint8 page;
	bool corked;
	bool dirty;
	QMap<QString,QVariant> map;
};

extern Settings* settings;

#endif // SETTINGS_H
