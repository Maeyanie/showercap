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
    qint32 uid = getuid();
    qint32 euid = geteuid();
    printf("Running as UID %d, EUID %d\n", uid, euid);
#endif

    wiringPiSetupGpio();
    pinMode(ONOFFPIN, OUTPUT);

#if __linux__
    setuid(uid);
    printf("Privileges dropped.\n");
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
