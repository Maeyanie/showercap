#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "pidthread.h"
#include "config.h"
#include <QSettings>


void MainWindow::on_plusButtonBath_clicked() {
    modSetTemp(1);
}
void MainWindow::on_minusButtonBath_clicked() {
    modSetTemp(-1);
}

void MainWindow::on_onOffButtonBath_clicked() {
    if (onOff) {
        // Turn off
        onOff = 0;
        output->off();
        ui->tabWidget->tabBar()->setEnabled(true);
    } else {
        // Turn on
        onOff = 1;
        startTime = QDateTime::currentDateTime();
        output->on();
        ui->tabWidget->tabBar()->setEnabled(false);
    }
    tick();
}

void MainWindow::tickBath() {
    ui->curTempBath->setText(QString().sprintf("%.1lf", curTemp));
    ui->setTempBath->setStyleSheet("");

    if (onOff) {
        qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime()) / 1000;
        ui->timerBath->setText(QString().sprintf("%lld:%02lld", elapsed/60, elapsed%60));
    } else {
        ui->timerBath->setText("");
    }

    ui->clockBath->setText(QTime::currentTime().toString("h:mm A"));
}

void MainWindow::on_setTempBath_clicked() {
    SetTempDialog* std = new SetTempDialog(this);
    std->show();
}


