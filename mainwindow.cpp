#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "manualwindow.h"
#include "pidthread.h"
#include "config.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setTemp = 0;
    curTemp = 0;
    onOff = 0;
    bathMode = 0;
    memset(preset, 0, sizeof(preset));

    ui->setupUi(this);
    QMainWindow::showFullScreen();

    readSettings();

    switch (config.sensorType) {
    case ONEWIRE:
        input = new Input_Onewire();
        break;
    case I2CSENSOR:
        input = new Input_I2CSensor();
        break;
    case THERMISTOR:
        input = new Input_Thermistor();
    }
    switch (config.outputType) {
    case SERVO:
        output = new Output_Servo();
        break;
    case MOTOR:
        output = new Output_Motor();
        break;
    case MOTORSOFTPWM:
        output = new Output_Motor_SoftPWM();
        break;
    case STEPPER:
        output = new Output_Stepper();
    }

    if (bathMode) {
        ui->bathButton->setChecked(true);
        output->bath();
    } else {
        ui->showerButton->setChecked(true);
        output->shower();
    }

    pidthread = new PIDThread(this);
    connect(pidthread, &PIDThread::update, this, &MainWindow::update);
    connect(pidthread, &PIDThread::fullhot, this, &MainWindow::fullhot);
    connect(pidthread, &PIDThread::fullcold, this, &MainWindow::fullcold);
    connect(pidthread, &PIDThread::finished, pidthread, &QObject::deleteLater);
    pidthread->start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    pidthread->requestInterruption();
    pidthread->wait();
    writeSettings();
    delete pidthread;
    delete ui;
}

void MainWindow::readSettings()
{
    QSettings settings("NMSoft", "Digital Shower Prototype");
    setTemp = settings.value("setTemp", 405).toInt(); // Average shower temperature is 105F, which is ~40.5C
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    preset[0] = settings.value("preset0", 405).toInt();
    preset[1] = settings.value("preset1", 405).toInt();
    preset[2] = settings.value("preset2", 405).toInt();
    preset[3] = settings.value("preset3", 405).toInt();
    bathMode = settings.value("bathMode", 0).toBool();
}
void MainWindow::writeSettings()
{
    QSettings settings("NMSoft", "Digital Shower Prototype");
    settings.setValue("setTemp", setTemp);
    settings.setValue("preset0", preset[0]);
    settings.setValue("preset1", preset[1]);
    settings.setValue("preset2", preset[2]);
    settings.setValue("preset3", preset[3]);
    settings.setValue("bathMode", bathMode);
}
void MainWindow::loadPreset(qint32 p)
{
    lock.lock();
    setTemp = preset[p];
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}
void MainWindow::savePreset(qint32 p)
{
    lock.lock();
    preset[p] = setTemp;
    writeSettings();
    lock.unlock();
}
qint32 MainWindow::getSetTemp()
{
    qint32 ret;
    lock.lock();
    ret = setTemp;
    lock.unlock();
    return ret;
}
void MainWindow::setSetTemp(qint32 t)
{
    lock.lock();
    setTemp = t;
    if (!bathMode && setTemp > config.maxTemp) setTemp = config.maxTemp;
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}

bool MainWindow::isOn()
{
    return onOff;
}

void MainWindow::update(qreal newTemp)
{
    curTemp = newTemp;
}
void MainWindow::fullhot()
{
    ui->setTemp->setStyleSheet("QLabel { color: red; }");
}
void MainWindow::fullcold()
{
    ui->setTemp->setStyleSheet("QLabel { color: blue; }");
}
void MainWindow::tick()
{
    if (onOff) {
        ui->curTemp->setText(QString().sprintf("%.1lf", curTemp));

        ui->setTemp->setStyleSheet("");

        qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime()) / 1000;
        ui->timer->setText(QString().sprintf("%lld:%02lld", elapsed/60, elapsed%60));

        ui->clock->setText(QTime::currentTime().toString("h:mm A"));
    } else {
        ui->curTemp->setText(QTime::currentTime().toString("h:mm A"));
        ui->timer->setText("");
        ui->clock->setText("");
    }
}

void MainWindow::on_plusButton_clicked()
{
    lock.lock();
    setTemp++;
    if (!bathMode && setTemp > config.maxTemp) setTemp = config.maxTemp;
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}
void MainWindow::on_minusButton_clicked()
{
    lock.lock();
    setTemp--;
    ui->setTemp->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}

void MainWindow::on_presetButton_1_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void MainWindow::on_presetButton_1_released()
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

void MainWindow::on_presetButton_2_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void MainWindow::on_presetButton_2_released()
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

void MainWindow::on_presetButton_3_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void MainWindow::on_presetButton_3_released()
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

void MainWindow::on_presetButton_4_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void MainWindow::on_presetButton_4_released()
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

void MainWindow::on_onOffButton_clicked()
{
    if (onOff) {
        // Turn off
        onOff = 0;
        digitalWrite(ONOFFPIN, LOW);
        delay(10);
        digitalWrite(STBYPIN, LOW);
        ui->bathButton->show();
        ui->showerButton->show();
        ui->manualButton->show();
    } else {
        // Turn on
        onOff = 1;
        startTime = QDateTime::currentDateTime();
        digitalWrite(STBYPIN, HIGH);
        delay(10);
        digitalWrite(ONOFFPIN, HIGH);
        ui->bathButton->hide();
        ui->showerButton->hide();
        ui->manualButton->hide();
    }
    tick();
}

void MainWindow::on_showerButton_clicked()
{
    ui->showerButton->setChecked(true);
    ui->bathButton->setChecked(false);
    bathMode = false;
    if (setTemp > config.maxTemp) setTemp = config.maxTemp;

}

void MainWindow::on_bathButton_clicked()
{
    ui->bathButton->setChecked(true);
    ui->showerButton->setChecked(false);
    bathMode = true;

}

void MainWindow::on_setTemp_clicked()
{
    SetTempDialog* std = new SetTempDialog(this);
    std->show();
}

void MainWindow::on_manualButton_clicked()
{
    ManualWindow* mw = new ManualWindow(this);
    mw->show();
    mw->showFullScreen();
}
