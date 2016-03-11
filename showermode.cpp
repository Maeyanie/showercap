#include "showermode.h"
#include "ui_showermode.h"
#include "mainwindow.h"
#include "pidthread.h"
#include "config.h"
#include <QSettings>

ShowerMode::ShowerMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowerMode)
{
    setTemp = 0;
    curTemp = 0;
    onOff = 0;
    memset(preset, 0, sizeof(preset));

    ui->setupUi(this);
    QMainWindow::showFullScreen();

    readSettings();

    pidthread = new PIDThread(this);
    connect(pidthread, &PIDThread::update, this, &ShowerMode::update);
    connect(pidthread, &PIDThread::finished, pidthread, &QObject::deleteLater);
    pidthread->start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(100);
}

ShowerMode::~ShowerMode()
{
    pidthread->requestInterruption();
    pidthread->wait();
    writeSettings();
    delete pidthread;
    delete ui;
}

void ShowerMode::readSettings()
{
    QSettings settings("NMSoft", "Digital Shower Prototype");
    setTemp = settings.value("setTemp", 405).toInt(); // Average shower temperature is 105F, which is ~40.5C
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    preset[0] = settings.value("preset0", 405).toInt();
    preset[1] = settings.value("preset1", 405).toInt();
    preset[2] = settings.value("preset2", 405).toInt();
    preset[3] = settings.value("preset3", 405).toInt();
}
void ShowerMode::writeSettings()
{
    QSettings settings("NMSoft", "Digital Shower Prototype");
    settings.setValue("setTemp", setTemp);
    settings.setValue("preset0", preset[0]);
    settings.setValue("preset1", preset[1]);
    settings.setValue("preset2", preset[2]);
    settings.setValue("preset3", preset[3]);
}
void ShowerMode::loadPreset(qint32 p)
{
    lock.lock();
    setTemp = preset[p];
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}
void ShowerMode::savePreset(qint32 p)
{
    lock.lock();
    preset[p] = setTemp;
    writeSettings();
    lock.unlock();
}
qint32 ShowerMode::getSetTemp()
{
    qint32 ret;
    lock.lock();
    ret = setTemp;
    lock.unlock();
    return ret;
}

void ShowerMode::update(qreal newTemp)
{
    curTemp = newTemp;
}
void ShowerMode::tick()
{
    ui->curTemp->setText(QString().sprintf("%.1lf", curTemp));

    if (onOff) {
        qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime()) / 1000;
        ui->timer->setText(QString().sprintf("%lld:%02lld", elapsed/60, elapsed%60));
    }

    ui->clock->setText(QTime::currentTime().toString("h:mm A"));
}

void ShowerMode::on_plusButton_clicked()
{
    lock.lock();
    setTemp++;
    if (setTemp > config.maxTemp) setTemp = config.maxTemp;
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}
void ShowerMode::on_minusButton_clicked()
{
    lock.lock();
    setTemp--;
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}

void ShowerMode::on_presetButton_1_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void ShowerMode::on_presetButton_1_released()
{
    if (!pressStart) return;
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        loadPreset(0);
    } else {
        savePreset(0);
    }
    delete pressStart;
    pressStart = NULL;
}

void ShowerMode::on_presetButton_2_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void ShowerMode::on_presetButton_2_released()
{
    if (!pressStart) return;
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        loadPreset(1);
    } else {
        savePreset(1);
    }
    delete pressStart;
    pressStart = NULL;
}

void ShowerMode::on_presetButton_3_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void ShowerMode::on_presetButton_3_released()
{
    if (!pressStart) return;
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        loadPreset(2);
    } else {
        savePreset(2);
    }
    delete pressStart;
    pressStart = NULL;
}

void ShowerMode::on_presetButton_4_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void ShowerMode::on_presetButton_4_released()
{
    if (!pressStart) return;
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        loadPreset(3);
    } else {
        savePreset(3);
    }
    delete pressStart;
    pressStart = NULL;
}

void ShowerMode::on_onOffButton_clicked()
{
    if (onOff) {
        // Turn off
        onOff = 0;
        digitalWrite(ONOFFPIN, LOW);
        ui->onOffButton->setChecked(false);
        ui->exitButton->show();
    } else {
        // Turn on
        onOff = 1;
        startTime = QDateTime::currentDateTime();
        digitalWrite(ONOFFPIN, HIGH);
        ui->onOffButton->setChecked(true);
        ui->exitButton->hide();
    }
    tick();
}

void ShowerMode::on_exitButton_clicked()
{
    this->hide();
    this->destroy();
}
