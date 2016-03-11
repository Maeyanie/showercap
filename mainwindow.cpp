#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showermode.h"
#include "config.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showFullScreen();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tick()
{
    QDateTime now = QDateTime::currentDateTime();
    ui->clock->setText(now.toString("dddd, MMMM d\nh:mm AP"));
}

void MainWindow::on_showerButton_clicked()
{
    new ShowerMode(this);
}

void MainWindow::on_bathButton_clicked()
{

}
