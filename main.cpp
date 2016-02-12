#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    wiringPiSetupGpio();
    pinMode(ONOFFPIN, OUTPUT);

    MainWindow w;
    w.show();

    return a.exec();
}
