#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settempdialog.h"
#include "iothread.h"
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


void MainWindow::on_useC_toggled(bool checked)
{
	if (!checked) return;
	config.useF = false;
	ui->setTemp->setText(asTemp(setTemp/10.0));
	ui->setTempBath->setText(asTemp(setTemp/10.0));
	for (int i = 0; i < (PRESETCOUNT*2); i++) {
		presetButton[i]->setText(asTemp(preset[i]/10.0));
	}
}

void MainWindow::on_useF_toggled(bool checked)
{
	if (!checked) return;
	config.useF = true;
	ui->setTemp->setText(asTemp(setTemp/10.0));
	ui->setTempBath->setText(asTemp(setTemp/10.0));
	for (int i = 0; i < (PRESETCOUNT*2); i++) {
		presetButton[i]->setText(asTemp(preset[i]/10.0));
	}
}
