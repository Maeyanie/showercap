#include "iothread.h"
#include "mainwindow.h"
#include "pointcloud.h"
#include "config.h"
#include <QTextStream>
#include <cmath>

#define SYNCTIMER 5000
#define DESYNCTIMER 5000
#define TIMEOUT 600

OutThread::OutThread(QObject* parent) : QThread(parent)
{
}

void OutThread::run()
{
	MainWindow* mw = static_cast<MainWindow*>(this->parent());
	QDateTime start, now, last = QDateTime::currentDateTime(), activity = QDateTime::currentDateTime();
	qreal error, integral = 0.0, derivative, value;
	qreal setTemp = 40.5, prevTemp = 0.0, curTemp = 0.0;
	qreal preError = 0.0;
	qreal Dt = 0.0;
	qreal Kp = config.Kp, Ki = config.Ki, Kd = config.Kd;
	qreal iMax = config.iMax, iMin = config.iMin;
	qint32 d;
	double home = qSNaN();
	bool on = 0;
	bool sync = 0;
	QDateTime* syncTime = nullptr;
	Pointcloud startPoints("startPoints");

	output = mw->output;
	onOff = mw->onOff;

	while (!QThread::currentThread()->isInterruptionRequested()) {
		curTemp = mw->getCurTemp();

		now = QDateTime::currentDateTime();
		setTemp = mw->getSetTemp() / 10.0;
		if (setTemp != prevTemp) {
			// User just changed the set temperature, don't panic when we're suddenly very wrong.
			sync = 0;
			prevTemp = setTemp;
		}

		if (!mw->isOn()) {
			if (on) {
				// State changed on->off
				if (syncTime) {
					delete syncTime;
					syncTime = nullptr;
				}
				onOff->off();
				output->off();
				on = 0;
				activity = QDateTime::currentDateTime();
			} else {
				if (!qIsNaN(home) && (mw->getTab() == 0 || activity.secsTo(now) > TIMEOUT)) {
					printf("[pidthread] Returning to home position %.1lf\n", home);
					output->on();
					msleep(100);
					output->set(home);
					printf("[pidthread] Done in %lld ms.\n", now.msecsTo(QDateTime::currentDateTime()));
					msleep(100);
					output->off();
					home = qSNaN();
				}
			}
			last = now;
			msleep(100);
			continue;
		}
		if (!on) {
			// State changed off->on
			start = QDateTime::currentDateTime();
			if (mw->isBath()) onOff->bath();
			else onOff->shower();
			output->on();
			onOff->on();
			on = 1;
			sync = 0;
			if (syncTime) {
				delete syncTime;
				syncTime = nullptr;
			}
		}

		if (start.msecsTo(now) < config.warmupTime && curTemp < setTemp) {
			last = now;
			msleep(100);
			continue;
		}

		Dt = last.msecsTo(now) / 1000.0;
		error = setTemp - curTemp;

		if (!sync && fabs(error) < 0.2) {
			if (!syncTime) {
				syncTime = new QDateTime(QDateTime::currentDateTime());
				printf("pidthread: Sync gained at %s, position %.2lf, starting timer.\n",
					   syncTime->toString("mm:ss.zzz").toStdString().c_str(), output->get());
			} else if (syncTime->msecsTo(QDateTime::currentDateTime()) >= SYNCTIMER) {
				sync = 1;
				if (qIsNaN(home)) home = output->get();
				printf("pidthread: Sync timer hit %d ms at %s, now in sync. Home position is %.2lf\n",
					   SYNCTIMER, syncTime->toString("mm:ss.zzz").toStdString().c_str(), home);
				delete syncTime;
				syncTime = nullptr;
				if (output->getMax() < home + 5000) {
					fprintf(stderr, "Warning: Found home=%.2lf and max=%.2lf, increasing max.", home, output->getMax());
					output->setMax(home + 5000);
				}
				startPoints.add(QPoint(static_cast<qint32>(setTemp)*10, static_cast<qint32>(output->get())));
			}
		} else if (sync && fabs(error) > 1.0) {
			if (!syncTime) {
				syncTime = new QDateTime(QDateTime::currentDateTime());
				printf("pidthread: Lost sync, starting timer.");
			} else if (syncTime->msecsTo(QDateTime::currentDateTime()) >= DESYNCTIMER) {
				delete syncTime;
				syncTime = nullptr;
				sync = 0;
				printf("pidthread: Sync timer hit %d ms, assuming out of hot water and giving up.\n", DESYNCTIMER);
				emit noHotWater();
			}
		} else if (syncTime) {
			delete syncTime;
			syncTime = nullptr;
		}

		// track error over time, scaled to the timer interval
		integral = integral + (error * Dt);
		if (integral > iMax) integral = iMax;
		else if (integral < iMin) integral = iMin;

		// determine the amount of change from the last time checked
		derivative = (error - preError) / Dt;

		// calculate how much to drive the output in order to get to the
		// desired setpoint.
		value = (Kp * error) + (Ki * integral) + (Kd * derivative);

		// remember the error for the next time around.
		preError = error;
		last = now;

		//printf("pidthread: setTemp=%f curTemp=%f error=%f p=%f i=%f d=%f\n",
		//    setTemp, curTemp, error, Kp*error, Ki*integral, Kp*derivative);

		switch (output->mod(value)) {
		case 1:
			emit fullhot();
			break;
		case -1:
			emit fullcold();
		}

		d = output->time(0);
		if (d > 1000) d = 1000;
		if (d > 0) msleep(d);
	}
}
