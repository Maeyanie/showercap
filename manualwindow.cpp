#include <QTime>
#include "io.h"
#include "config.h"
#include "manualwindow.h"
#include "ui_manualwindow.h"

ManualWindow::ManualWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualWindow)
{
    ui->setupUi(this);

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

    step = 8;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(100);
}

ManualWindow::~ManualWindow()
{
    delete ui;
}

void ManualWindow::tick()
{
    qreal curTemp = input->read();
    ui->curTemp->setText(QString().sprintf("%.1lf", curTemp));
    ui->clock->setText(QTime::currentTime().toString("h:mm A"));
}

void ManualWindow::on_plusButton_clicked() { output->mod(step); }
void ManualWindow::on_minusButton_clicked() { output->mod(-step); }

void ManualWindow::on_stepUpButton_clicked() {
    step *= 2;
    ui->stepSize->setText(QString().sprintf("%d", step));
}
void ManualWindow::on_stepDownButton_clicked() {
    step /= 2;
    if (step < 1) step = 1;
    ui->stepSize->setText(QString().sprintf("%d", step));
}

void ManualWindow::on_onOffButton_2_clicked() {
    if (onOff) {
        // Turn off
        onOff = 0;
        output->off();
        ui->closeButton->show();
    } else {
        // Turn on
        onOff = 1;
        output->on();
        ui->closeButton->hide();
    }
    tick();
}

void ManualWindow::on_closeButton_clicked() {
    this->hide();
    this->destroy();
}
