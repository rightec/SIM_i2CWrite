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
    m_workerThread = new QThread(this); //I'm compiling under QT environement - Skip this in other environment
    connect(m_workerThread, SIGNAL(started()), this, SLOT(I2C_WriteExecuting()));

}

/*!
 * \brief I2CDriver::I2C_Write
 * \param address
 * \param data
 * \param data_length
 */
void I2CDriver::I2C_Write(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool))
{
   uint8_t u8Address = address;
   uint8_t* u8_data = data;
   size_t s_data_length = data_length;


   //Just some code
   u8Address++;
   u8_data[0] = u8Address + (uint8_t)s_data_length;
   m_v_callback = callback;
   qDebug() << "I2CWrite in action";

   delay((int)m_WriteTime);
   m_Callback = true;

   qDebug() << "I2CWrite go to callback";

   m_v_callback(m_Callback);

//   QtConcurrent::run(test);
//   this->moveToThread(m_workerThread);
//   m_workerThread->start();

}

void I2CDriver::I2C_WriteExecuting()
{
    QThread::sleep(m_WriteTime);

    //delay((int)m_WriteTime);
    m_Callback = true;

    qDebug() << "I2CWrite go to callback";

    m_v_callback(m_Callback);
}


/*!
 * \brief I2CDriver::I2C_WriteSetTime
 * \param _setTime
 */
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

void test()
{
    QThread::sleep(5000);

    qDebug() << "TEST";
}

