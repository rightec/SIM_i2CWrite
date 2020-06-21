#include "i2cbussim.h"

I2CBusSim::I2CBusSim()
{

}

/*!
 * \brief I2CBusSim::I2C_SetCycleNum
 * \param _num
 * \return
 */
bool I2CBusSim::I2C_SetCycleNum(int _num)
{
    bool bRetVal = false;

    if ((_num > 0) && (_num <= I2C_BUS_CYCLE_NUM)){
        I2C_write_Cycle = _num;
        bRetVal = true;
    }else{
        bRetVal = false;
    }

    return bRetVal;

}

/*!
 * \brief I2CBusSim::I2C_SetWriteSize
 * \param _size
 * \return
 */
bool I2CBusSim::I2C_SetWriteSize(int _size)
{
    bool bRetVal = false;

    if ((_size > 0) && (_size <= I2C_AREA_SIZE)){
        I2C_write_Size = _size;
        bRetVal = true;
    }else{
        bRetVal = false;
    }

    return bRetVal;
}

/*!
 * \brief I2CBusSim::I2C_runMaster
 */
bool I2CBusSim::I2C_runMaster(){
    bool bRetVal = false;

    int i = 0;
    int j = 0;

   int jCompare = I2C_write_Cycle;
   int iCompare = I2C_write_Size;

   if (I2C_Bus.i2cBusBusy == false){



    I2C_Bus.i2cBusBusy = true;
    while (j < I2C_write_Cycle){
        while (i < I2C_write_Size){
            I2C_Bus.i2cArea[i] = 0xAA;
            i++;
        }
    j++;
    }//end of j cycle
    I2C_Bus.i2cBusBusy = false;

    if ((j == jCompare ) && (i==iCompare)){
        bRetVal = true;
        qDebug() << "I2C bus write ended";
    }else{
        bRetVal = false;
    }
}else{
       qDebug() << "I2C bus busy";
       bRetVal = false;
   }
    return bRetVal;
}

/*!
 * \brief I2CBusSim::I2C_stopMaster
 */
void I2CBusSim::I2C_stopMaster(){
    I2C_SetCycleNum(1);
    I2C_SetWriteSize(1);
}

/*!
 * \brief I2CBusSim::doWork
 */
void I2CBusSim::doWork() {
    bool bRetVal = false;
    bRetVal = I2C_runMaster();
    emit I2CBusSim::resultReady(bRetVal);
}
