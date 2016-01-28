#ifndef PIDTHREAD_H
#define PIDTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>

class PIDThread : public QThread
{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE;
    void newTemp(qint32 temp);

    QString filename;
    qint32 setTemp;

signals:
    void update(qreal curTemp);

public:
    PIDThread(QObject* parent, QString filename);
};

#endif // PIDTHREAD_H
