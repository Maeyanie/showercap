#if __linux__
#include <sys/types.h>
#include <unistd.h>
#endif
#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if __linux__
    printf("Running as UID %d, EUID %d\n", getuid(), geteuid());
#endif

    wiringPiSetupGpio();
    pinMode(ONOFFPIN, OUTPUT);

#if __linux__
    setuid(getuid());
    printf("Privileges dropped.\n");
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
