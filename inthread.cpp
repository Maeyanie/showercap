#include "iothread.h"
#include "mainwindow.h"
#include "config.h"
#include <QTextStream>
#include <cmath>

#define WINSIZE 16

InThread::InThread(QObject* parent) : QThread(parent)
{
}

void InThread::run()
{
    MainWindow* mw = static_cast<MainWindow*>(this->parent());
	Input* in = mw->input;
    qreal window[WINSIZE];
    qint32 winpos = 0;
    qreal value;

	printf("[InThread] Starting.\n");
	while (!QThread::currentThread()->isInterruptionRequested()) {
        if (!mw->onOff) msleep(100);

        window[winpos++] = in->read();
        winpos &= (WINSIZE-1);

        value = 0;
        for (qint32 x = 0; x < WINSIZE; x++)
            value += window[x];
        value /= WINSIZE;

		emit update(value);
	}
	printf("[InThread] Ending.\n");
}
