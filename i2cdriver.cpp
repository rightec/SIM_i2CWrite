/***
 * @file i2cdriver.cpp
 *
 * @brief This class is simulating
 * the i2C driver devloped by MCU supplier
 *
 * @ingroup BeamTrail Test project
 *
 * @author Fernando Morani
 * Contact: morani.rightec@gmail.com
 *
 */
#include "i2cdriver.h"

I2CDriver::I2CDriver()
{
    m_WriteTime = 0;
}


void I2CDriver::I2C_Write(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool))
{
   uint8_t u8Address = address;
   uint8_t* u8_data = data;
   size_t s_data_length = data_length;
   void (*v_callback)(bool) = callback;
   bool bCallback = false;

   //Just some code
   u8Address++;
   u8_data[0] = u8Address + (uint8_t)s_data_length;

   qDebug() << "I2CWrite in action";

   //QThread::sleep(m_WriteTime);

   delay((int)m_WriteTime);
   bCallback = true;

   qDebug() << "I2CWrite go to callback";

   v_callback(bCallback);

}


void I2CDriver::I2C_WriteSetTime(uint32_t _setTime)
{
    m_WriteTime = _setTime;
}


void delay(int _milliSec)
{
    QTime dieTime = QTime::currentTime().addMSecs(_milliSec);
        while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
