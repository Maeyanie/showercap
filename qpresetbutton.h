#ifndef QPRESETBUTTON_H
#define QPRESETBUTTON_H

#include <QPushButton>
#include <QTimer>

class QPresetButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QPresetButton(qint32 id, const QString& text, QWidget *parent = 0);

private slots:
    void press();
    void release();
    void tick();

private:
    qint32 id;
    QTimer timer;
};

#endif // QPRESETBUTTON_H
