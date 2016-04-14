#ifndef SETTEMPDIALOG_H
#define SETTEMPDIALOG_H

#include <QDialog>

namespace Ui {
class SetTempDialog;
}

class SetTempDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetTempDialog(QWidget *parent = 0);
    ~SetTempDialog();

private slots:
    void on_tensButton_clicked();
    void on_onesButton_clicked();
    void on_deciButton_clicked();

    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

    void on_button5_clicked();

    void on_button6_clicked();

    void on_button7_clicked();

    void on_button8_clicked();

    void on_button9_clicked();

    void on_button0_clicked();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::SetTempDialog *ui;
    qint32 digit;

    void setDigit(int val);
};

#endif // SETTEMPDIALOG_H
