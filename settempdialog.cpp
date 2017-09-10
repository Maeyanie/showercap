#include "settempdialog.h"
#include "ui_settempdialog.h"
#include "mainwindow.h"

SetTempDialog::SetTempDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetTempDialog)
{
    digit = 0;
    qint32 temp = ((MainWindow*)parent)->getSetTemp();

    ui->setupUi(this);
    ui->tensButton->setDown(true);
    ui->tensButton->setText(QString::number(temp / 100));
    ui->onesButton->setText(QString::number((temp % 100) / 10));
    ui->deciButton->setText(QString::number(temp % 10));
}

SetTempDialog::~SetTempDialog()
{
    delete ui;
}

void SetTempDialog::setDigit(int val)
{
    switch (digit) {
    case 0:
        ui->tensButton->setText(QString::number(val));
        digit++;
        ui->tensButton->setDown(false);
        ui->onesButton->setDown(true);
        break;
    case 1:
        ui->onesButton->setText(QString::number(val));
        digit++;
        ui->onesButton->setDown(false);
        ui->deciButton->setDown(true);
        break;
    case 2:
        ui->deciButton->setText(QString::number(val));
        digit = 0;
        ui->tensButton->setDown(true);
        ui->deciButton->setDown(false);
        break;
    }
}

void SetTempDialog::on_tensButton_clicked()
{
    digit = 0;
    ui->tensButton->setDown(true);
    ui->onesButton->setDown(false);
    ui->deciButton->setDown(false);
}

void SetTempDialog::on_onesButton_clicked()
{
    digit = 1;
    ui->tensButton->setDown(false);
    ui->onesButton->setDown(true);
    ui->deciButton->setDown(false);
}

void SetTempDialog::on_deciButton_clicked()
{
    digit = 2;
    ui->tensButton->setDown(false);
    ui->onesButton->setDown(false);
    ui->deciButton->setDown(true);
}

void SetTempDialog::on_button1_clicked() { setDigit(1); }
void SetTempDialog::on_button2_clicked() { setDigit(2); }
void SetTempDialog::on_button3_clicked() { setDigit(3); }
void SetTempDialog::on_button4_clicked() { setDigit(4); }
void SetTempDialog::on_button5_clicked() { setDigit(5); }
void SetTempDialog::on_button6_clicked() { setDigit(6); }
void SetTempDialog::on_button7_clicked() { setDigit(7); }
void SetTempDialog::on_button8_clicked() { setDigit(8); }
void SetTempDialog::on_button9_clicked() { setDigit(9); }
void SetTempDialog::on_button0_clicked() { setDigit(0); }

void SetTempDialog::on_okButton_clicked()
{
    qint32 temp = ui->tensButton->text().toInt() * 100 + ui->onesButton->text().toInt() * 10 + ui->deciButton->text().toInt();
    ((MainWindow*)this->parent())->setSetTemp(temp);
    this->hide();
    this->destroy();
}

void SetTempDialog::on_cancelButton_clicked()
{
    this->hide();
    this->destroy();
}
