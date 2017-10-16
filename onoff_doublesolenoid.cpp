#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include <QSettings>
#include "io.h"
#include "config.h"

#define PULSETIME -1

OnOff_DoubleSolenoid::OnOff_DoubleSolenoid() {
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(SHOWERPIN, 0);
    digitalWrite(BATHPIN, 0);

    onOff = 0;
    mode = 1;

    printf("OnOff_DoubleSolenoid initialized.\n");
}

OnOff_DoubleSolenoid::~OnOff_DoubleSolenoid() {
    off();
}

static void pulseSolenoid() {
    if (PULSETIME < 0) {
		printf("[OnOff_DoubleSolenoid] Solenoid pulse: Infinite.\n");
        digitalWrite(ONOFFPIN, 1);
    } else if (PULSETIME > 0) {
		printf("[OnOff_DoubleSolenoid] Solenoid pulse: %d ms.\n", PULSETIME);
        digitalWrite(ONOFFPIN, 1);
        delay(PULSETIME);
        digitalWrite(ONOFFPIN, 0);
    }
}

void OnOff_DoubleSolenoid::on() {
    if (mode == 1) {
		printf("[OnOff_DoubleSolenoid] Turning on shower.\n");
        digitalWrite(SHOWERPIN, 1);
    } else {
		printf("[OnOff_DoubleSolenoid] Turning on bath.\n");
        digitalWrite(BATHPIN, 1);
    }
    onOff = 1;
    pulseSolenoid();
}

void OnOff_DoubleSolenoid::off() {
	printf("[OnOff_DoubleSolenoid] Turning off.\n");
    onOff = 0;
    digitalWrite(ONOFFPIN, 0);
    digitalWrite(SHOWERPIN, 0);
    digitalWrite(BATHPIN, 0);
}

void OnOff_DoubleSolenoid::shower() {
	printf("[OnOff_DoubleSolenoid] Changing to shower.\n");
    if (onOff) {
        digitalWrite(BATHPIN, 0);
        digitalWrite(SHOWERPIN, 1);
        pulseSolenoid();
    }
    mode = 1;
}

void OnOff_DoubleSolenoid::bath() {
	printf("[OnOff_DoubleSolenoid] Changing to bath.\n");
    if (onOff) {
        digitalWrite(SHOWERPIN, 0);
        digitalWrite(BATHPIN, 1);
        pulseSolenoid();
    }
    mode = 0;
}

