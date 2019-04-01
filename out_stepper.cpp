#include <wiringPi.h>
#include <cstdio>
#include "iodriver.h"
#include "iothread.h"
#include "config.h"
#include "settings.h"

#define CYCLETIME 250 // in ms
#define STEPTIME 5000 // in us
#define FULLHOT 10000
#define FULLCOLD 0
#define MAXSTEPS ((CYCLETIME-10)/(STEPTIME/500))

Output_Stepper::Output_Stepper() {
	digitalWrite(STEPPIN, 0);
	digitalWrite(DIRPIN, 0);
	digitalWrite(STBYPIN, 0);
	digitalWrite(MS1PIN, 0);
	digitalWrite(MS2PIN, 0);
	digitalWrite(MS3PIN, 0);

	position = 0;
	duration = 0;
	onOff = 0;
	position = settings->value("stepperpos", 0).toInt();

	printf("Output_Stepper initialized: Starting at %d\n", position);
}

Output_Stepper::~Output_Stepper() {
	settings->setValue("stepperpos", position);
	off();
}

void Output_Stepper::save() {
	settings->setValue("stepperpos", position);
}

void Output_Stepper::on() {
	digitalWrite(STEPPIN, 0);
	digitalWrite(DIRPIN, 0);
	digitalWrite(ENABLEPIN, 1);
	digitalWrite(STBYPIN, 1);
	onOff = 1;
	printf("[Output_Stepper] Turned on.\n");
}
void Output_Stepper::off() {
	printf("[Output_Stepper] Turned off.\n");
	onOff = 0;
	digitalWrite(STEPPIN, 0);
	digitalWrite(DIRPIN, 0);
	digitalWrite(ENABLEPIN, 1);
	digitalWrite(STBYPIN, 0);
}

void Output_Stepper::set(double v) {
	if (!onOff) {
		printf("[Output_Stepper] Error: set() called when turned off!\n");
		return;
	}
	printf("[Output_Stepper] set: Moving from %d to %d.\n", (int)position, (int)v);

	if (v >= position+1) {
		duration = ((v - position) * STEPTIME * 2) / 1000 + 10;
		digitalWrite(DIRPIN, 1);
		digitalWrite(ENABLEPIN, 0);
        msleep(5);
		while (v >= position+1) {
			digitalWrite(STEPPIN, 1);
            usleep(STEPTIME);
			digitalWrite(STEPPIN, 0);
            usleep(STEPTIME);
			position++;
		}
        msleep(5);
		digitalWrite(ENABLEPIN, 1);
	} else if (v <= position-1) {
		duration = ((position - v) * STEPTIME * 2) / 1000 + 10;
		digitalWrite(DIRPIN, 0);
		digitalWrite(ENABLEPIN, 0);
        msleep(5);
		while (v <= position-1) {
			digitalWrite(STEPPIN, 1);
            usleep(STEPTIME);
			digitalWrite(STEPPIN, 0);
            usleep(STEPTIME);
			position--;
		}
        msleep(5);
		digitalWrite(ENABLEPIN, 1);
	} else {
		duration = 0;
	}
	save();
}

qint8 Output_Stepper::mod(double d) {
	if (!onOff) {
		printf("[Output_Stepper] Error: mod() called when turned off!\n");
		return 0;
	}
	static double frac = 0.0;
	d += frac;

	if (d > 1.0) {
		if (d > MAXSTEPS) d = MAXSTEPS;
		duration = (d * STEPTIME * 2) / 1000 + 10;
		digitalWrite(ENABLEPIN, 0);
		digitalWrite(DIRPIN, 1);
        msleep(5);
		for (int i = 0; i < d; i++) {
			digitalWrite(STEPPIN, 1);
            usleep(STEPTIME);
			digitalWrite(STEPPIN, 0);
            usleep(STEPTIME);
			position++;
		}
        msleep(5);
		digitalWrite(ENABLEPIN, 1);
		frac = 0.0;
	} else if (d < -1.0) {
		if (d < -MAXSTEPS) d = -MAXSTEPS;
		duration = (-d * STEPTIME * 2) / 1000 + 10;
		digitalWrite(ENABLEPIN, 0);
		digitalWrite(DIRPIN, 0);
        msleep(5);
		for (int i = 0; i < -d; i++) {
			digitalWrite(STEPPIN, 1);
            usleep(STEPTIME);
			digitalWrite(STEPPIN, 0);
            usleep(STEPTIME);
			position--;
		}
        msleep(5);
		digitalWrite(ENABLEPIN, 1);
		frac = 0.0;
	} else {
		frac = d;
		duration = 0;
	}
	save();
	return 0;
}

qint32 Output_Stepper::time(qint32 t) {
	return CYCLETIME-t-duration;
}
