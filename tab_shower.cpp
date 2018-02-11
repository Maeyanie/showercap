#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "iothread.h"
#include "config.h"
#include <QSettings>


void MainWindow::on_plusButton_clicked() {
    modSetTemp(1);
}
void MainWindow::on_minusButton_clicked() {
    modSetTemp(-1);
}

void MainWindow::on_onOffButton_clicked() {
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

void MainWindow::tickShower() {
	ui->curTemp->setText(asDegrees(curTemp));
    ui->setTemp->setStyleSheet("");

    if (onOffFlag) {
        qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime()) / 1000;
        ui->timer->setText(QString().sprintf("%lld:%02lld", elapsed/60, elapsed%60));
    } else {
        ui->timer->setText("");
    }

	ui->clock->setText(QTime::currentTime().toString(config.timeStr));
    ui->pos->setText(QString::number(output->get()));
}

void MainWindow::on_setTemp_clicked() {
    SetTempDialog* std = new SetTempDialog(this);
	std->showFullScreen();
}


