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
    if (onOffFlag) {
        // Turn off
        onOffFlag = 0;
        ui->tabWidget->tabBar()->setEnabled(true);
    } else {
        // Turn on
        onOffFlag = 1;
        startTime = QDateTime::currentDateTime();
        ui->tabWidget->tabBar()->setEnabled(false);
    }
    tick();
}

void MainWindow::tickBath() {
	ui->curTempBath->setText(asDegrees(curTemp));
    ui->setTempBath->setStyleSheet("");

    if (onOffFlag) {
        qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime()) / 1000;
        ui->timerBath->setText(QString().sprintf("%lld:%02lld", elapsed/60, elapsed%60));
    } else {
        ui->timerBath->setText("");
    }

    ui->clockBath->setText(QTime::currentTime().toString("h:mm A"));
    ui->posBath->setText(QString::number(output->get()));
}

void MainWindow::on_setTempBath_clicked() {
    SetTempDialog* std = new SetTempDialog(this);
	std->showFullScreen();
}


