#include <QTypeInfo>
#include <vector>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "pidthread.h"
#include "config.h"
#include "spline.h"
using namespace std;
using namespace tk;

#define RESISTOR 10000.0
//#include "rt_usp10972.h"
#include "rt_ppg102a1.h"

// The reference voltage seems pretty stable at 3.284600 V, and we can save 8 ms by not checking it.
#define REF (26276.0*4.0)

#define ONESHOT 0b1000000000000000
#define MUX0G   0b0100000000000000
#define MUX1G   0b0101000000000000
#define PGA1    0b0000001000000000
#define PGA2    0b0000010000000000
#define PGA3    0b0000100000000000
#define MODESS  0b0000000100000000
#define DR128   0b0000000010000000
#define CMPOFF  0b0000000000000011

#define PGA4096V (PGA1)
#define PGA2048V (PGA2)
#define PGA1024V (PGA2|PGA1)

#define READTEMP (ONESHOT|MUX0G|PGA1024V|MODESS|DR128|CMPOFF)

static spline thermistor_spline;

static inline short bswap16(short a) {
    return ((((a) & 0xFF00) >> 8) | (((a) & 0x00FF) << 8));
}

Input_Thermistor::Input_Thermistor() {
    int count = sizeof(thermistor_curve) / sizeof(thermistor_curve[0]);
    vector<double> x(count), y(count);

    for (int i = 0; i < count; i++) {
        y[count-i-1] = thermistor_curve[i][0];
        x[count-i-1] = thermistor_curve[i][1];
    }

    thermistor_spline.set_points(x, y);

    dev = wiringPiI2CSetup(I2C_ADC);
    if (dev == -1) { fprintf(stderr, "Error opening I2C thermistor ADC: %m\n"); exit(1); }
}

double Input_Thermistor::read() {
    static unsigned int last;
    /*
    wiringPiI2CWriteReg16(dev, 0x01, bswap16(READREF));
    // 128 SPS = ~7.8 ms/read
    delay(8);

    int ref = bswap16(wiringPiI2CReadReg16(dev, 0x00));
    double v = 4.096 / (32767.0 / (double)ref);
    ref *= 2; // ref is read at half the gain of temperature.
    printf("[Thermistor] Ref:  %d (%x) = %lf v\n", ref, ref, v);
    */
    if (millis() - last > 100) {
        wiringPiI2CWriteReg16(dev, 0x01, bswap16(READTEMP));
        last = millis();
        delay(8);
    } else if (millis() - last < 8) {
        delay(8 - (millis() - last));
    }

    int data = bswap16(wiringPiI2CReadReg16(dev, 0x00));
    //double v = 2.048 / (32767.0 / (double)data);
    //printf("[Thermistor] Data: %d (%x) = %lf v\n", data, data, v);

    double R = RESISTOR / (REF / (double)data - 1);
    //printf("[Thermistor] Resistance: %lf Ω\n", R);

    double t = thermistor_spline(R);
    //printf("[Thermistor] Temperature: %lf C\n", t);

    wiringPiI2CWriteReg16(dev, 0x01, bswap16(READTEMP));
    last = millis();

    return t;
}
