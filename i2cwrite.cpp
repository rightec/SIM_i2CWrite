/***
 * @file i2cwrite.h
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
#include "i2cwrite.h"

I2CWrite::I2CWrite()
{
    //Init Section
    I2CWriteReset();
}


I2WriteState I2CWrite::I2CWriteRun()
{
    while (m_State != I2CWRITE_BUS_FAIL){
        qDebug() << "I2CWriteRun STATE is" << m_State; //I'm compiling under QT environement - Skip this in other environment
        if (m_ForceClose == false){
            switch(m_State){
            case I2CWRITE_IDLE:
                if (I2CWriteGetWriteStartFlag() == true){
                    //We have a request accepted
                    I2CWriteSetState(I2CWRITE_REQUEST_TO_WRITE);
                }else{
                    //Cycle in this state
                    asm("nop");
                }
                break;
            case I2CWRITE_REQUEST_TO_WRITE:
                I2CWriteSetState(I2CWRITE_WRITE_IN_PROGRESS);
                m_Driver.I2C_Write(m_address, m_data, m_data_length,m_callback);
                break;
            case I2CWRITE_WRITE_IN_PROGRESS:
                if (I2CWriteGetWriteDoneFlag() == true){
                    I2CWriteSetState(I2CWRITE_WRITE_OK);
                }else{
                    if (I2CWriteGetWriteFailFlag() == true){
                        I2CWriteSetState(I2CWRITE_WRITE_FAIL);
                    }else{
                        //Nothing to do
                        asm("nop");
                    }
                }
                break;
            case I2CWRITE_WRITE_OK:
                //Reset fal and back to IDLE
                I2CWriteReset();
                break;
            case I2CWRITE_WRITE_FAIL:
                asm("nop");
                //We need an external reset
                break;
            case I2CWRITE_BUS_FAIL:
                //When in fail I reset all
                I2CWriteReset();
                break;
            case I2CWRITE_END:
                asm("nop");
                break;
            }

            QThread::msleep(I2C_STANDARD_SWITCH_TIME);  //I'm compiling under QT environement - Skip this in other environment

        }else{
            //Go to BUS_FAIL_STATE - We need a restart
            m_State = I2CWRITE_BUS_FAIL;
        }
    }
    return m_State;
}

/*!
 * \brief I2CWrite::I2CWriteGetState
 * \return the current state of the I2CWrite Automa
 */
I2WriteState I2CWrite::I2CWriteGetState()
{
    return m_State;
}


void I2CWrite::I2CWriteSetState(I2WriteState _state)
{
    if (_state != m_State){
        qDebug() << "State Changed" << _state;  //I'm compiling under QT environement - Skip this in other environment
        m_State = _state;
        emit m_stateChanged((int)m_State);
    }else{
        asm("nop");
    }
}




/*!
 * \brief I2CWrite::I2CWriteIsFree
 * \return true if the Write to I2C Bus is allowed
 * and the pointer to callback is not null
 */
bool I2CWrite::I2CWriteIsFree(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool))
{
    bool bRetVal = false;

    //1st check the input parameter
    if (callback != nullptr){
        if (m_State == I2CWRITE_IDLE){
            //Acces to resource is allowed
            m_callback = callback;
            m_address = address;
            m_data = data;
            m_data_length = data_length;
            I2CWriteSetWriteStartFlag(true);
            bRetVal = true;
        }else{
            //I2C resource is busy
            bRetVal = false;
        }
    }else{
        bRetVal = false;
    }

    return bRetVal;
}


/*!
 * \brief I2CWrite::I2CWriteReset
 */
void I2CWrite::I2CWriteReset()
{
    I2CWriteSetState(I2CWRITE_IDLE);
    I2CWriteSetWriteStartFlag(false);
    I2CWriteSetWriteDoneFlag(false);
    I2CWriteSetWriteFailFlag(false);
    m_callback = nullptr;
    m_ForceClose = false;
}


void I2CWrite::I2CWriteStartStop(bool _startStop)
{
    m_ForceClose = _startStop;
}

/*!
 * \brief I2CWrite::I2CWriteSetWriteTime
 * This method is ONLY FOR SIMULATION PURPOSE
 * \param _setTime
 */
void I2CWrite::I2CWriteSetWriteTime(uint32_t _setTime)
{
    m_Driver.I2C_WriteSetTime(_setTime);
}

/*!
 * \brief I2CWrite::I2CWriteSetWriteDoneFlag
 * \param _flag
 */
void I2CWrite::I2CWriteSetWriteDoneFlag(bool _flag)
{
    m_WriteDone = _flag;
}

/*!
 * \brief I2CWrite::I2CWriteSetWriteStartFlag
 * \param _flag
 */
void I2CWrite::I2CWriteSetWriteStartFlag(bool _flag)
{
    m_WriteTrigger = _flag;
}

/*!
 * \brief I2CWrite::I2CWriteSetWriteFailFlag
 * \param _flag
 */
void I2CWrite::I2CWriteSetWriteFailFlag(bool _flag)
{
    m_WriteFail = _flag;
}

/*!
 * \brief I2CWrite::I2CWriteGetWriteDoneFlag
 * \return
 */
bool I2CWrite::I2CWriteGetWriteDoneFlag()
{
    return m_WriteDone;
}

/*!
 * \brief I2CWrite::I2CWriteGetWriteStartFlag
 * \return
 */
bool I2CWrite::I2CWriteGetWriteStartFlag()
{
    return m_WriteTrigger;
}

/*!
 * \brief I2CWrite::I2CWriteGetWriteFailFlag
 * \return
 */
bool I2CWrite::I2CWriteGetWriteFailFlag()
{
    return m_WriteFail;
}
