#include <QTypeInfo>
#include <vector>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "iothread.h"
#include "config.h"
#include "spline.h"
using namespace std;
using namespace tk;

#define RESISTOR 10000.0
#define DELAY 17 // 60 sps = 16.666... ms/s
//#include "rt_usp10972.h"
#include "rt_ppg102a1.h"

static spline thermistor_spline;
static inline short bswap16(short a) {
    return ((((a) & 0xFF00) >> 8) | (((a) & 0x00FF) << 8));
}

Input_LTC2451::Input_LTC2451() {
    int count = sizeof(thermistor_curve) / sizeof(thermistor_curve[0]);
    vector<double> x(count), y(count);

    for (int i = 0; i < count; i++) {
        y[count-i-1] = thermistor_curve[i][0];
        x[count-i-1] = thermistor_curve[i][1];
    }

    thermistor_spline.set_points(x, y);

	dev = wiringPiI2CSetup(I2C_LTC2451);
	if (dev == -1) { fprintf(stderr, "Error opening LTC2451: %m\n"); exit(1); }

	/*if (wiringPiI2CWrite(dev, 1) < 0) { // Set 30 Hz sample rate (33.333... ms per sample)
		fprintf(stderr, "Error setting LTC2451 sample rate.\n");
	}*/
}

double Input_LTC2451::read() {
    static unsigned int last;

	if (millis() - last > 100) {
		wiringPiI2CRead(dev); // Read the stale conversion and throw it out.
		last = millis();
		delay(DELAY);
	} else if (millis() - last < DELAY) {
		delay(DELAY - (millis() - last));
	}

	int data = wiringPiI2CRead(dev);
	printf("[Input_LTC2451] Raw data: %d (%x)\n", data, data);

	data = bswap16(data);
	printf("[Input_LTC2451] Swapped data: %d (%x)\n", data, data);

	double R = RESISTOR / (65535.0/data - 1.0);
	printf("[Input_LTC2451] Resistance: %lf Ω\n", R);

    double t = thermistor_spline(R);
	printf("[Input_LTC2451] Temperature: %lf C\n", t);

	last = millis();
	return t;
}


