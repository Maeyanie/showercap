#include <QTime>
#include "iodriver.h"
#include "config.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_plusButtonManual_clicked() {
    output->limits(false);
    output->mod(step);
    output->limits(true);
    tick();
}
void MainWindow::on_minusButtonManual_clicked() {
    output->limits(false);
    output->mod(-step);
    output->limits(true);
    tick();
}

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

void MainWindow::on_bathButtonManual_clicked()
{
    onOff->bath();
    tick();
}

void MainWindow::on_showerButtonManual_clicked()
{
    onOff->shower();
    tick();
}

void MainWindow::on_setMinManual_clicked()
{
    output->setMin(output->get());
}
void MainWindow::on_setMaxManual_clicked()
{
    output->setMax(output->get());
}

void MainWindow::tickManual() {
	ui->curTempManual->setText(asDegrees(curTemp));
	ui->clockManual->setText(QTime::currentTime().toString(config.timeStr));
    ui->posManual->setText(QString().sprintf("Pos: %.0lf", output->get()));
    ui->maxManual->setText(QString().sprintf("Max: %.0lf", output->getMax()));
    ui->minManual->setText(QString().sprintf("Min: %.0lf", output->getMin()));
}

