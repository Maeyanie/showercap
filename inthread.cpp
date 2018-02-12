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

	while (!QThread::currentThread()->isInterruptionRequested()) {
		if (!mw->onOff) delay(100);

		value += in->read();
		value /= 2;
		emit update(value);
	}
}
