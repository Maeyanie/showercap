#include "iothread.h"
#include "mainwindow.h"
#include "config.h"
#include <QTextStream>
#include <cmath>


InThread::InThread(QObject* parent) : QThread(parent)
{
}

void InThread::run()
{
	MainWindow* mw = (MainWindow*)this->parent();
	Input* in = mw->input;
	qreal value = 0.0;

	printf("[InThread] Starting.\n");
	while (!QThread::currentThread()->isInterruptionRequested()) {
        if (!mw->onOff) msleep(100);

		value += in->read();
		value /= 2;
		emit update(value);
	}
	printf("[InThread] Ending.\n");
}
