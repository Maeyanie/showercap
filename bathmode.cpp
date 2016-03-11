#include "bathmode.h"
#include "ui_bathmode.h"

BathMode::BathMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BathMode)
{
    curTemp = 0;
    onOff = 0;

    ui->setupUi(this);
    QMainWindow::showFullScreen();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(100);
}

BathMode::~BathMode()
{
    delete ui;
}

void BathMode::on_onoffButton_clicked()
{
    if (onOff) {
        onOff = 0;
        ui->onoffButton->setChecked(false);
        ui->exitButton->show();
    } else {
        onOff = 1;
        ui->onoffButton->setChecked(true);
        ui->exitButton->hide();
    }
}
