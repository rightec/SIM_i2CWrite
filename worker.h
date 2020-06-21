#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include "i2cbussim.h"

#define WORKER_MAX_NUM 3

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
public slots:
    void doWork();
    void doFinish();
private:
    I2CBusSim *i2cSimulator;
};
#endif // WORKER_H
