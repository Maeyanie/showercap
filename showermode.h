#ifndef SHOWERMODE_H
#define SHOWERMODE_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <wiringPi.h>
#include "pidthread.h"

namespace Ui {
    class ShowerMode;
}

class ShowerMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowerMode(QWidget *parent = 0);
    void readSettings();
    void writeSettings();
    void loadPreset(qint32 preset);
    void savePreset(qint32 preset);
    qint32 getSetTemp();
    ~ShowerMode();

private slots:
    void update(qreal newTemp);
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

    void on_exitButton_clicked();

private:
    Ui::ShowerMode *ui;

    PIDThread* pidthread;
    QMutex lock;
    qint32 setTemp; // in decidegrees
    qreal curTemp;
    bool onOff;
    qint32 preset[4];
    QDateTime startTime;
    QTimer* timer;

    QDateTime* pressStart;
};

#endif
