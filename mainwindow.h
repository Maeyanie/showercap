#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include "pidthread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void readSettings();
    void writeSettings();
    ~MainWindow();

private slots:
    void update(qreal newTemp);

    void on_plusButton_clicked();
    void on_minusButton_clicked();

    void on_presetButton_1_pressed();
    void on_presetButton_1_released();
    void on_presetButton_2_pressed();
    void on_presetButton_2_released();
    void on_presetButton_3_pressed();
    void on_presetButton_3_released();
    void on_presetButton_4_pressed();
    void on_presetButton_4_released();

private:
    Ui::MainWindow *ui;

    qint32 setTemp; // in decidegrees
    qreal curTemp;
    bool onOff;
    qint32 preset[4];
    QDateTime* pressStart;
    PIDThread* pidthread;
};

#endif // MAINWINDOW_H
