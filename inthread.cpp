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

	while (!QThread::currentThread()->isInterruptionRequested()) {
		qreal curTemp = in->read();
		emit update(curTemp);
	}
}
