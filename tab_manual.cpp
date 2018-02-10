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

void MainWindow::on_onOffButtonManual_clicked() {
    if (onOffFlag) {
        // Turn off
        onOffFlag = 0;
        ui->tabWidget->tabBar()->setEnabled(true);
    } else {
        // Turn on
        onOffFlag = 1;
        ui->tabWidget->tabBar()->setEnabled(false);
    }
    tick();
}

void MainWindow::tickManual() {
	ui->curTempManual->setText(asDegrees(curTemp));
    ui->clockManual->setText(QTime::currentTime().toString("h:mm A"));
}

