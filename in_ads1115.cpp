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
//#include "rt_usp10972.h"
#include "rt_ppg102a1.h"

// The reference voltage seems pretty stable at 3.284600 V, and we can save 8 ms by not checking it.
#define REF (26276.0*2.0)

#define ONESHOT 0b1000000000000000
#define MUX0G   0b0100000000000000
#define MUX1G   0b0101000000000000
#define PGA1    0b0000001000000000
#define PGA2    0b0000010000000000
#define PGA3    0b0000100000000000
#define MODESS  0b0000000100000000
#define DR32    0b0000000001000000
#define DR64    0b0000000001100000
#define DR128   0b0000000010000000
#define CMPOFF  0b0000000000000011

#define PGA4096V (PGA1)
#define PGA2048V (PGA2)
#define PGA1024V (PGA2|PGA1)

#define READTEMP (ONESHOT|MUX0G|PGA2048V|MODESS|DR64|CMPOFF)
#define DELAY 16 // 64 sps = 15.625 ms/s

static spline thermistor_spline;

static inline short bswap16(short a) {
    return ((((a) & 0xFF00) >> 8) | (((a) & 0x00FF) << 8));
}

Input_ADS1115::Input_ADS1115() {
    int count = sizeof(thermistor_curve) / sizeof(thermistor_curve[0]);
    vector<double> x(count), y(count);

    for (int i = 0; i < count; i++) {
        y[count-i-1] = thermistor_curve[i][0];
        x[count-i-1] = thermistor_curve[i][1];
    }

    thermistor_spline.set_points(x, y);

	dev = wiringPiI2CSetup(I2C_ADS1115);
    if (dev == -1) { fprintf(stderr, "Error opening I2C thermistor ADC: %m\n"); exit(1); }
}

double Input_ADS1115::read() {
    static unsigned int last;
	unsigned int now = millis();
    /*
    wiringPiI2CWriteReg16(dev, 0x01, bswap16(READREF));
    // 128 SPS = ~7.8 ms/read
	delay(DELAY);

    int ref = bswap16(wiringPiI2CReadReg16(dev, 0x00));
    double v = 4.096 / (32767.0 / (double)ref);
    ref *= 2; // ref is read at half the gain of temperature.
    printf("[Thermistor] Ref:  %d (%x) = %lf v\n", ref, ref, v);
    */
    if (now > last + 100 || now < last) {
        wiringPiI2CWriteReg16(dev, 0x01, bswap16(READTEMP));
		last = now;
		delay(DELAY);
	} else if (DELAY > (now - last)) {
		delay(DELAY - (now - last));
    }

    int data = bswap16(wiringPiI2CReadReg16(dev, 0x00));
    //double v = 2.048 / (32767.0 / (double)data);
    //printf("[Thermistor] Data: %d (%x) = %lf v\n", data, data, v);

    double R = RESISTOR / (REF / (double)data - 1);
    //printf("[Thermistor] Resistance: %lf Ω\n", R);

    double t = thermistor_spline(R);
    //printf("[Thermistor] Temperature: %lf C\n", t);

    wiringPiI2CWriteReg16(dev, 0x01, bswap16(READTEMP));
	last = now;

    return t;
}
