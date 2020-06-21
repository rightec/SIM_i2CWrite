#include "staticmember.h"


I2CWrite *g_i2cWriteLayer;

//Static function
void myCallback_1 (bool _retVal)
{
    bool bRetVal = _retVal;
    qDebug() << "myCallback_1";

    if (bRetVal == true){
        g_i2cWriteLayer->I2CWriteSetWriteDoneFlag(true);
    }else{
        g_i2cWriteLayer->I2CWriteSetWriteFailFlag(true);
    }

    bRetVal = true;

}

void myCallback_2 (bool _retVal)
{
    bool bRetVal = _retVal;

    qDebug() << "myCallback_2";

    if (bRetVal == true){
        g_i2cWriteLayer->I2CWriteSetWriteDoneFlag(true);
    }else{
        g_i2cWriteLayer->I2CWriteSetWriteFailFlag(true);
    }

    bRetVal = true;

}
