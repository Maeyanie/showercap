#include <QDataStream>
#ifdef __linux__
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#else
#define I2C_SLAVE 0x0703
int ioctl(int fd, unsigned long request, ...) { fd=request; return 0; }
#endif
#include <stdio.h>
#include <unistd.h>
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
 * 1: header version
 * 2-3: reserved
 *
 * 4-5: page0 version
 * 6-7: page0 size
 * 8-9: page0 crc16
 * 10-11: reserved
 *
 * 12-13: page1 version
 * 14-15: page1 size
 * 16-17: page1 crc16
 * 18-19: reserved
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
    assert(read(i2c, data, 2) == 2);
	page = data[0];
    if (page > FRAMPAGES) {
        fprintf(stderr, "[FRAMSettings] Error: FRAM specifies current page as %hhu of %d. Not loading.\n", page, FRAMPAGES);
        page = 0;
        return;
    }
    if (data[1] != 1) {
        fprintf(stderr, "[FRAMSettings] Error: FRAM header specifies version %hhu, expected 1. Not loading.\n", data[1]);
        return;
    }
	printf("[FRAMSettings] Loading from page %hhu.\n", page);


    quint16 ver;
    quint16 len;
    quint16 crc;
	data[0] = 0;
	data[1] = 4 + (4*page);
	write(i2c, data, 2);
	read(i2c, &ver, 2);
    if (ver != 1) {
        fprintf(stderr, "[FRAMSettings] Error: FRAM page saved with version %hu (expected 1). Not loading.\n", ver);
        return;
    }
	read(i2c, &len, 2);
    if (len > FRAMPAGESIZE) {
        fprintf(stderr, "[FRAMSettings] Error: FRAM page reports length %hu of %d. Not loading.\n", len, FRAMPAGESIZE);
        return;
    }
    read(i2c, &crc, 2);

	unsigned short pos = FRAMHEADER + (FRAMPAGESIZE * page);
    printf("[FRAMSettings] Loading %hu bytes at position %hu.\n", len, pos);
	data[0] = (pos & 0xFF00) >> 8;
	data[1] = (pos & 0x00FF);
	write(i2c, data, 2);
	assert(read(i2c, data, len) == len);

    qint16 checkcrc = qChecksum(data, len);
    if (checkcrc != crc) {
        fprintf(stderr, "[FRAMSettings] Error: Bad CRC16. Calculated %hx, expected %hx. Not loading.\n", checkcrc, crc);
        // For now, exiting for debugging purposes. Change this to "return;" later, or maybe try loading the other page.
        exit(1);
    }

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

    quint16 ver = 1;
    quint16 len = bytes.size();
    quint16 crc = qChecksum(bytes.data(), bytes.size());
    assert(len <= FRAMPAGESIZE);

    quint8 pos = FRAMHEADER + (FRAMPAGESIZE * page);
    char data[FRAMPAGESIZE];
	data[0] = (pos & 0xFF00) >> 8;
	data[1] = (pos & 0x00FF);
	memcpy(data+2, bytes.data(), bytes.size());
    write(i2c, data, bytes.size()+2);

	data[0] = 0;
    data[1] = 4 + (8*page);
	memcpy(data+2, &ver, 2);
	memcpy(data+4, &len, 2);
    memcpy(data+6, &crc, 2);
    data[8] = 0;
    data[9] = 0;
    write(i2c, data, 10);

	data[0] = 0;
	data[1] = 0;
	data[2] = page;
    data[3] = 1;
    data[4] = 0;
    data[5] = 0;
    write(i2c, data, 6);

	if (++page >= FRAMPAGES) page = 0;
}
