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

    if (getuid()) {
        printf("User mode. Hoping pins are already right.\n");
        wiringPiSetupSys();
    } else {
        printf("Root mode. Setting up pins.\n");
        wiringPiSetupGpio();
        pinMode(ONOFFPIN, OUTPUT);
        pinMode(BATHPIN, OUTPUT);
        pinMode(SHOWERPIN, OUTPUT);
        pinMode(HOTPIN, OUTPUT);
        pinMode(COLDPIN, OUTPUT);
        pinMode(STEPPIN, OUTPUT);
        pinMode(DIRPIN, OUTPUT);
        pinMode(ENABLEPIN, OUTPUT);
        pinMode(STBYPIN, OUTPUT);
        pinMode(MS1PIN, OUTPUT);
        pinMode(MS2PIN, OUTPUT);
        pinMode(MS3PIN, OUTPUT);

        pinMode(PWMPIN, PWM_OUTPUT);

        pinMode(FULLHOTPIN, INPUT);
        pinMode(FULLCOLDPIN, INPUT);
        pullUpDnControl(FULLHOTPIN, PUD_DOWN);
        pullUpDnControl(FULLCOLDPIN, PUD_DOWN);

        pwmSetMode(PWM_MODE_MS);
        pwmSetClock(1);
        pwmSetRange(512);

        setuid(getuid());
        printf("Privileges dropped.\n");
    }
#endif

    QApplication a(argc, argv);
    a.setStyleSheet("QPushButton:checked { color: white; background: black; }"
                "QPushButton:pressed { background: gray; }");

    MainWindow w;
    w.show();

    return a.exec();
}
