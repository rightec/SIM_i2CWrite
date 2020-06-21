/***
 * @file i2cwrite.cpp
 *
 * @brief This class should grant the access
 * To I2CBus in writing operation
 * This is a Middleware layer between the
 * The I2CDriver layer and the I2CBus Scheduler
 *
 * @ingroup BeamTrail Test project
 *
 * @author Fernando Morani
 * Contact: morani.rightec@gmail.com
 *
 */

#ifndef I2CWRITE_H
#define I2CWRITE_H

#include "i2cdriver.h" //MCU supplied library
#include <QObject> //I'm compiling under QT environement - Skip this in other environment
#include <QDebug>  //I'm compiling under QT environement - Skip this in other environment
#include <QThread> //I'm compiling under QT environement - Skip this in other environment
#include <time.h>  //I'm compiling under QT environement - Skip this in other environment


enum I2WriteState{
    I2CWRITE_IDLE = 0,              //No write operation are currently running
    I2CWRITE_REQUEST_TO_WRITE,      //A write request has been issued
    I2CWRITE_WRITE_IN_PROGRESS,     //A write operation is in progress
    I2CWRITE_WRITE_OK,              //Current write is terminated OK
    I2CWRITE_WRITE_FAIL,            //Current write has failed
    I2CWRITE_BUS_FAIL,              //Any other error caused a stop in I2C Bus
    I2CWRITE_END
};

#define I2C_STANDARD_SWITCH_TIME  1000                          //In milliseconds I'm compiling under QT environement - Skip this in other environment
#define I2C_TASK1_LASTING_TIME    5*I2C_STANDARD_SWITCH_TIME    //In milliseconds I'm compiling under QT environement - Skip this in other environment
#define I2C_TASK2_LASTING_TIME    3*I2C_STANDARD_SWITCH_TIME    //In milliseconds I'm compiling under QT environement - Skip this in other environment

//class I2CWrite
class I2CWrite : public QObject //I'm compiling under QT environement - Use previous line in other environment
{
    Q_OBJECT  //To remove if environment different from QT
public:
    I2CWrite();
    //I2WriteState I2CWriteRun();
    I2WriteState I2CWriteGetState();
    void I2CWriteSetState(I2WriteState _state);
    bool I2CWriteIsFree(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool));
    void I2CWriteReset();
    void I2CWriteStartStop(bool _startStop);
    void I2CWriteSetWriteDoneFlag(bool _flag);
    void I2CWriteSetWriteStartFlag(bool _flag);
    void I2CWriteSetWriteFailFlag(bool _flag);
    bool I2CWriteGetWriteDoneFlag();
    bool I2CWriteGetWriteStartFlag();
    bool I2CWriteGetWriteFailFlag();
    void I2CWriteSetWriteTime(uint32_t _setTime); //Used only for simulation - To remove if environment different from QT

public slots:   //To remove if environment different from QT
    I2WriteState I2CWriteRun();

private:
    I2WriteState m_State;
    bool        m_WriteTrigger;  //uses as start write command
    void        (*m_callback)(bool);
    uint8_t     m_address;
    uint8_t*    m_data;
    size_t      m_data_length;
    bool        m_ForceClose;
    I2CDriver   m_Driver;      //Simulated MCU I2C Driver
    bool        m_WriteDone;
    bool        m_WriteFail;

    clock_t     m_start;       //I'm compiling under QT environement - Skip this in other environment
    clock_t     m_end;         //I'm compiling under QT environement - Skip this in other environment

signals:                       //To remove if environment different from QT
    void m_stateChanged(int);  //To remove if environment different from QT
};

#endif // I2CWRITE_H
