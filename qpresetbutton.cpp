#include <QDateTime>
#include "mainwindow.h"
#include "qpresetbutton.h"

QPresetButton::QPresetButton(qint32 i, const QString& text, QWidget *parent)
    : QPushButton(text, parent)
{
    id = i;
    timer.setSingleShot(true);
    connect(this, SIGNAL(pressed()), this, SLOT(press()));
    connect(this, SIGNAL(released()), this, SLOT(release()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void QPresetButton::press() {
    timer.start(1000);
}

void QPresetButton::release() {
    timer.stop();
    MainWindow::instance()->loadPreset(id);
    setStyleSheet("");
}

void QPresetButton::tick() {
    MainWindow::instance()->savePreset(id);
    setStyleSheet("QPushButton:pressed { background: green; }");
}
