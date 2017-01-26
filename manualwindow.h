#ifndef MANUALWINDOW_H
#define MANUALWINDOW_H

#include <QDialog>
#include <QTimer>
#include "io.h"

namespace Ui {
class ManualWindow;
}

class ManualWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ManualWindow(QWidget *parent = 0);
    ~ManualWindow();

private slots:
    void tick();

    void on_plusButton_clicked();
    void on_minusButton_clicked();
    void on_stepUpButton_clicked();
    void on_stepDownButton_clicked();
    void on_onOffButton_2_clicked();
    void on_closeButton_clicked();

private:
    Ui::ManualWindow *ui;
    QTimer* timer;
    Output* output;
    Input* input;
    qint32 step;
    bool onOff;

};

#endif // MANUALWINDOW_H
