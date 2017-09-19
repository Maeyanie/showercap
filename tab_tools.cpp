#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "pidthread.h"
#include "config.h"
#include <QSettings>
#include <unistd.h>

void MainWindow::on_updateButton_clicked()
{
    cleanup();

    execlp("/bin/bash", "/bin/bash", "update.sh", NULL);
}


void MainWindow::on_restartButton_clicked()
{
    cleanup();

#if __linux__
    char buf[PATH_MAX];
    readlink("/proc/self/exe", buf, PATH_MAX);
    execlp(buf, buf, NULL);
#endif
}
