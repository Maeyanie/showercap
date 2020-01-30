#include <QTime>
#include "iodriver.h"
#include "config.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_plusButtonManual_clicked() { output->mod(step); }
void MainWindow::on_minusButtonManual_clicked() { output->mod(-step); }

void MainWindow::on_stepUpButton_clicked() {
    step *= 2;
    ui->stepSize->setText(QString().sprintf("%d", step));
}
void MainWindow::on_stepDownButton_clicked() {
    step /= 2;
    if (step < 1) step = 1;
    ui->stepSize->setText(QString().sprintf("%d", step));
}

void MainWindow::on_onButtonManual_clicked() {
    onOff->on();
    output->on();
    tick();
}


void MainWindow::on_offButtonManual_clicked() {
    onOff->off();
    output->off();
    tick();
}

void MainWindow::tickManual() {
	ui->curTempManual->setText(asDegrees(curTemp));
	ui->clockManual->setText(QTime::currentTime().toString(config.timeStr));
}

