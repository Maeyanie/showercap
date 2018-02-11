#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <QPushButton>
#include <wiringPi.h>
#include "iothread.h"
#include "qpresetbutton.h"

#define PRESETCOUNT 4

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* instance();

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void readSettings();
    void writeSettings();
    void loadPreset(qint32 id);
    void savePreset(qint32 id);
	qreal getCurTemp();
    qint32 getSetTemp();
    void setSetTemp(qint32 t);
    void setMode(qint32 mode);
    bool isOn();
    bool isShower() { return !bathMode; }
    bool isBath() { return bathMode; }
	qint32 getTab();
	const QString asTemp(double degrees);
	const QString asDegrees(double degrees);

    class Input* input;
    class Output* output;
    class OnOff* onOff;

private slots:
    void cleanup();
    void update(qreal newTemp);
	void noHotWater();
    void fullhot();
    void fullcold();
    void tick();
    void modSetTemp(qint32);
    void on_tabWidget_currentChanged(int index);

    // Home tab
    void tickHome();

    // Shower tab
    void tickShower();
    void on_plusButton_clicked();
    void on_minusButton_clicked();
    void on_onOffButton_clicked();
    void on_setTemp_clicked();

    // Bath tab
    void tickBath();
    void on_plusButtonBath_clicked();
    void on_minusButtonBath_clicked();
    void on_onOffButtonBath_clicked();
    void on_setTempBath_clicked();

    // Manual tab
    void tickManual();
    void on_plusButtonManual_clicked();
    void on_minusButtonManual_clicked();
    void on_onOffButtonManual_clicked();
    void on_stepUpButton_clicked();
    void on_stepDownButton_clicked();
    void on_showerButton_clicked();
    void on_bathButton_clicked();

    // Tools tab
    void on_updateButton_clicked();
    void on_restartButton_clicked();


	void on_useC_toggled(bool checked);

	void on_useF_toggled(bool checked);

private:
    Ui::MainWindow *ui;

	InThread* inthread;
	OutThread* outthread;
	QMutex lock, ctLock;
    QPresetButton* presetButton[PRESETCOUNT*2];
    qint32 preset[PRESETCOUNT*2];
    qint32 setTemp; // in decidegrees
    qreal curTemp;
    bool onOffFlag;
    bool bathMode;
    QDateTime startTime;
    QTimer* timer;
    qint32 step;
};

#endif // MAINWINDOW_H
