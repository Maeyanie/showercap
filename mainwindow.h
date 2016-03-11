#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void tick();

    void on_showerButton_clicked();

    void on_bathButton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer* timer;
};

#endif // MAINWINDOW_H
