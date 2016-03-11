#ifndef BATHMODE_H
#define BATHMODE_H

#include <QMainWindow>

namespace Ui {
class BathMode;
}

class BathMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit BathMode(QWidget *parent = 0);
    ~BathMode();

private slots:
    void on_onoffButton_clicked();

private:
    Ui::BathMode *ui;

    qreal curTemp;
    bool onOff;
    QDateTime startTime;
    QTimer* timer;
};

#endif // BATHMODE_H
