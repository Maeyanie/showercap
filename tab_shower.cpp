#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "pidthread.h"
#include "config.h"
#include <QSettings>


void MainWindow::on_plusButton_clicked() {
    modSetTemp(1);
}
void MainWindow::on_minusButton_clicked() {
    modSetTemp(-1);
}

void MainWindow::on_onOffButton_clicked() {
    if (onOff) {
        // Turn off
        onOff = 0;
        ui->tabWidget->tabBar()->setEnabled(true);
    } else {
        // Turn on
        onOff = 1;
        startTime = QDateTime::currentDateTime();
        ui->tabWidget->tabBar()->setEnabled(false);
    }
    tick();
}

void MainWindow::tickShower() {
    ui->curTemp->setText(QString().sprintf("%.1lf", curTemp));
    ui->setTemp->setStyleSheet("");

    if (onOff) {
        qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime()) / 1000;
        ui->timer->setText(QString().sprintf("%lld:%02lld", elapsed/60, elapsed%60));
    } else {
        ui->timer->setText("");
    }

    ui->clock->setText(QTime::currentTime().toString("h:mm A"));
}

void MainWindow::on_setTemp_clicked() {
    SetTempDialog* std = new SetTempDialog(this);
    std->show();
}


