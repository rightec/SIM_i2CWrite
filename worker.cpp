#include "worker.h"

Worker::Worker()
{
    i2cSimulator = new (I2CBusSim);
}

void Worker::doWork() {
    int iSize = I2C_AREA_SIZE;
    int iCycle = I2C_BUS_CYCLE_NUM;
    bool bRetVal = i2cSimulator->I2C_SetWriteSize(iSize);
    if (bRetVal == true){
        qDebug() << "Size set " << iSize;
    }else{
      qDebug() << "SIZE NOT SET";
    }
    bRetVal = i2cSimulator->I2C_SetCycleNum(iCycle);
    if (bRetVal == true){
        qDebug() << "Cycle set " << iCycle;
    }else{
      qDebug() << "CYCLE NOT SET";
    }

    i2cSimulator->I2C_runMaster();
}

void Worker::doFinish(){
    qDebug() << "Thread terminated";
}
