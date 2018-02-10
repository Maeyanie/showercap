#include <QDataStream>
#if __linux__
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#else
#define I2C_SLAVE 0x0703
int ioctl(int fd, unsigned long request, ...) { fd=fd; request=request; return 0; }
#endif
#include <fcntl.h>
#include <assert.h>
#include "settings.h"

Settings* settings;


FileSettings::FileSettings() {
	qsettings = new QSettings("NMSoft", "Digital Shower Prototype");
}
FileSettings::~FileSettings() {
	qsettings->sync();
	delete settings;
}
void FileSettings::setValue(const QString& key, const QVariant& value) {
	qsettings->setValue(key, value);
}
void FileSettings::cork() {}
void FileSettings::uncork() {}
void FileSettings::sync() {
	qsettings->sync();
}
QVariant FileSettings::value(const QString& key, const QVariant& defaultValue) const {
	return qsettings->value(key, defaultValue);
}



/* Header Format:
 * 0: last written page
 * 1-3: reserved
 *
 * 4-5: page0 version
 * 6-7: page0 size
 *
 * 8-9: page1 version
 * 10-11: page1 size
 * ...
 */
FRAMSettings::FRAMSettings() {
	corked = false;
	dirty = false;

	i2c = open("/dev/i2c-1", O_RDWR);
	assert(i2c >= 0);
	assert(ioctl(i2c, I2C_SLAVE, FRAMADDR) >= 0);

	char data[FRAMPAGESIZE];
	data[0] = 0;
	data[1] = 0;
	write(i2c, data, 2);
	assert(read(i2c, data, 1) == 1);
	page = data[0];
	printf("[FRAMSettings] Loading from page %hhu.\n", page);

	unsigned short ver;
	unsigned short len;
	data[0] = 0;
	data[1] = 4 + (4*page);
	write(i2c, data, 2);
	read(i2c, &ver, 2);
	assert(ver == 1);
	read(i2c, &len, 2);
	assert(len <= FRAMPAGESIZE);

	unsigned short pos = FRAMHEADER + (FRAMPAGESIZE * page);
	data[0] = (pos & 0xFF00) >> 8;
	data[1] = (pos & 0x00FF);
	write(i2c, data, 2);
	assert(read(i2c, data, len) == len);

	QByteArray bytes(data, len);
	QDataStream buffer(&bytes, QIODevice::ReadOnly);
	buffer.setVersion(QDataStream::Qt_5_0);
	buffer >> map;

	if (++page >= FRAMPAGES) page = 0;
}
FRAMSettings::~FRAMSettings() {
	sync();
}
void FRAMSettings::setValue(const QString& key, const QVariant& value) {
	if (map.contains(key) && map.value(key) == value) return;
	map.insert(key, value);
	dirty = true;
	if (!corked) sync();
}
QVariant FRAMSettings::value(const QString& key, const QVariant& defaultValue) const {
	if (map.contains(key)) return map.value(key);
	return defaultValue;
}

void FRAMSettings::cork() {
	corked = true;
}
void FRAMSettings::uncork() {
	corked = false;
	sync();
}
void FRAMSettings::sync() {
	if (!dirty) return;

	QByteArray bytes;
	QDataStream buffer(&bytes, QIODevice::WriteOnly);
	buffer.setVersion(QDataStream::Qt_5_0);
	buffer << map;

	unsigned short ver = 1;
	unsigned short len = bytes.size();
	assert(len <= FRAMPAGESIZE);

	unsigned short pos = FRAMHEADER + (FRAMPAGESIZE * page);
	char data[bytes.size()+2];
	data[0] = (pos & 0xFF00) >> 8;
	data[1] = (pos & 0x00FF);
	memcpy(data+2, bytes.data(), bytes.size());

	write(i2c, data, bytes.size()+2);

	data[0] = 0;
	data[1] = 4 + (4*page);
	memcpy(data+2, &ver, 2);
	memcpy(data+4, &len, 2);
	write(i2c, data, 6);

	data[0] = 0;
	data[1] = 0;
	data[2] = page;
	write(i2c, data, 3);

	if (++page >= FRAMPAGES) page = 0;
}
