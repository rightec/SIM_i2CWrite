#ifndef I2CBUSSIM_H
#define I2CBUSSIM_H

#include <QObject>
#include <QDebug>

#define I2C_AREA_SIZE       100000
#define I2C_BUS_CYCLE_NUM   10000000

typedef struct i2cBus{
    unsigned int i2cArea[I2C_AREA_SIZE];
    bool i2cBusBusy = false;
}i2cBus_t;

class I2CBusSim : public QObject
{
    Q_OBJECT
public:
    I2CBusSim();

    bool I2C_SetWriteSize(int _size);
    bool I2C_SetCycleNum(int _num);
    bool I2C_runMaster();
    void I2C_stopMaster();

public slots:
    void doWork();

private:
    i2cBus_t I2C_Bus;
    int I2C_write_Size;
    int I2C_write_Cycle;

signals:
    void resultReady(bool _result);
};

#endif // I2CBUSSIM_H
