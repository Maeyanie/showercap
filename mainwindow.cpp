#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pidthread.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setTemp = 0;
    curTemp = 0;
    onOff = 0;
    memset(preset, 0, sizeof(preset));

    ui->setupUi(this);
    QMainWindow::showFullScreen();

    readSettings();

    pidthread = new PIDThread(this, "/sys/bus/w1/devices/28-000005cd0b3b/w1_slave");
    connect(pidthread, &PIDThread::update, this, &MainWindow::update);
    connect(pidthread, &PIDThread::finished, pidthread, &QObject::deleteLater);
    pidthread->start();
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
    ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    preset[0] = settings.value("preset0", 405).toInt();
    preset[1] = settings.value("preset1", 405).toInt();
    preset[2] = settings.value("preset2", 405).toInt();
    preset[3] = settings.value("preset3", 405).toInt();
}
void MainWindow::writeSettings()
{
    QSettings settings("NMSoft", "Digital Shower Prototype");
    settings.setValue("setTemp", setTemp);
    settings.setValue("preset0", preset[0]);
    settings.setValue("preset1", preset[1]);
    settings.setValue("preset2", preset[2]);
    settings.setValue("preset3", preset[3]);
}
qint32 MainWindow::getSetTemp()
{
    qint32 ret;
    lock.lock();
    ret = setTemp;
    lock.unlock();
    return ret;
}

void MainWindow::update(qreal newTemp)
{
    curTemp = newTemp;
    ui->label->setText(QString().sprintf("%.1lf", curTemp));
}

void MainWindow::on_plusButton_clicked()
{
    lock.lock();
    setTemp++;
    if (setTemp > 480) setTemp = 480;
    ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}
void MainWindow::on_minusButton_clicked()
{
    lock.lock();
    setTemp--;
    ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    lock.unlock();
}

void MainWindow::on_presetButton_1_pressed()
{
    pressStart = new QDateTime(QDateTime::currentDateTime());
}
void MainWindow::on_presetButton_1_released()
{
    if (!pressStart) return;
    lock.lock();
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        setTemp = preset[0];
        ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    } else {
        preset[0] = setTemp;
        writeSettings();
    }
    lock.unlock();
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
    lock.lock();
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        setTemp = preset[1];
        ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    } else {
        preset[1] = setTemp;
        writeSettings();
    }
    lock.unlock();
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
    lock.lock();
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        setTemp = preset[2];
        ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    } else {
        preset[2] = setTemp;
        writeSettings();
    }
    lock.unlock();
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
    lock.lock();
    qint64 difftime = pressStart->msecsTo(QDateTime::currentDateTime());
    if (difftime < 1000) {
        setTemp = preset[3];
        ui->label_2->setText(QString().sprintf("%.1lf", setTemp/10.0));
    } else {
        preset[3] = setTemp;
        writeSettings();
    }
    lock.unlock();
    delete pressStart;
    pressStart = NULL;
}

void MainWindow::on_onOffButton_clicked()
{
    exit(0);
}
