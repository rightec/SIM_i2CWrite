/***
 * @file i2cdriver.h
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

#ifndef I2CDRIVER_H
#define I2CDRIVER_H

#include <stdint.h>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QTime>

class I2CDriver
{
public:
    I2CDriver();
    void I2C_Write(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool));
    void I2C_WriteSetTime(uint32_t);
private:
    uint32_t m_WriteTime;   //Simulated Time to get a write completed
};

#endif // I2CDRIVER_H


void delay(int _milliSec);
