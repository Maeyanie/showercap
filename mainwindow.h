#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <wiringPi.h>
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
    void loadPreset(qint32 preset);
    void savePreset(qint32 preset);
    qint32 getSetTemp();
    void setSetTemp(qint32 t);
    bool isOn();
    bool isShower() { return !bathMode; }
    bool isBath() { return bathMode; }
    ~MainWindow();

    class Input* input;
    class Output* output;

private slots:
    void update(qreal newTemp);
    void fullhot();
    void fullcold();
    void tick();

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

    void on_onOffButton_clicked();

    void on_showerButton_clicked();

    void on_bathButton_clicked();

    void on_setTemp_clicked();

    void on_manualButton_clicked();

private:
    Ui::MainWindow *ui;

    PIDThread* pidthread;
    QMutex lock;
    qint32 setTemp; // in decidegrees
    qreal curTemp;
    bool onOff;
    bool bathMode;
    qint32 preset[4];
    QDateTime startTime;
    QTimer* timer;

    QDateTime* pressStart;
};

#endif // MAINWINDOW_H
