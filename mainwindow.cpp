#include "mainwindow.h"
#include "ui_mainwindow.h"


//static I2CWrite *g_i2cWriteLayer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Initialization
    ui->setupUi(this);    

    for (int i = 0; i< WORKER_MAX_NUM; i++){
        //Create worker and thread
        worker[i] = new Worker;
        workerThread[i] = new QThread(this);
        //Connect worker and thread
        connect(workerThread[i], SIGNAL(started()), worker[i], SLOT(doWork()));
        connect(workerThread[i], SIGNAL(finished()), worker[i], SLOT(doFinish()));

        worker[i]->moveToThread(workerThread[i]);
    }

    g_i2cWriteLayer = new I2CWrite;
    workerThread[WORKER_MAX_NUM] = new QThread(this);
    //Connect worker and thread
    connect(workerThread[WORKER_MAX_NUM], SIGNAL(started()), g_i2cWriteLayer, SLOT(I2CWriteRun()));

    //connect i2C state
    connect(g_i2cWriteLayer, SIGNAL(m_stateChanged(int)), this, SLOT(setI2CBusStatus(int))  );

    //Graphic Init
    i2cBusStandBy();
    i2cWriteStateIdle();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::handleResults(bool _res)
{
    //Verify if the write operation has been fully completed
    bool bRetVal = _res;

    if (bRetVal == true){
        i2cBusStandBy();
    }else{
        i2cBusInConflict();
    }
}



bool MainWindow::runningI2CSim()
{
    bool bRetVal = false;

//    i2cSimulator.I2C_SetWriteSize(I2C_AREA_SIZE);
//    i2cSimulator.I2C_SetCycleNum(I2C_BUS_CYCLE_NUM);
//    bRetVal = i2cSimulator.I2C_runMaster();

    return bRetVal;
}



/*!
 * \brief MainWindow::i2cBusStandBy
 */
void MainWindow::i2cBusStandBy()
{
    ui->lbl_I2CInUse->setVisible(false);
    ui->lbl_I2CStandBy->setVisible(true);
    ui->lbl_I2CInConflict->setVisible(false);
}

/*!
 * \brief MainWindow::i2cBusInUse
 */
void MainWindow::i2cBusInUse()
{
    ui->lbl_I2CInUse->setVisible(true);
    ui->lbl_I2CStandBy->setVisible(false);
    ui->lbl_I2CInConflict->setVisible(false);
}

/*!
 * \brief MainWindow::i2cBusInConflict
 */
void MainWindow::i2cBusInConflict()
{
    ui->lbl_I2CInUse->setVisible(false);
    ui->lbl_I2CStandBy->setVisible(false);
    ui->lbl_I2CInConflict->setVisible(true);
}




/*!
 * \brief MainWindow::on_btn_RunBus_clicked
 */
void MainWindow::on_btn_RunBus_clicked()
{
   i2cBusInUse();
   putWorkerInAThread();
   //Start the I2C thread
   g_i2cWriteLayer->I2CWriteReset();
   g_i2cWriteLayer->moveToThread(workerThread[WORKER_MAX_NUM]);
   workerThread[WORKER_MAX_NUM]->start();

   ui->btn_StartTask_1->setStyleSheet(m_BackGroundWhite);
   ui->btn_StartTask_2->setStyleSheet(m_BackGroundWhite);
   ui->btn_StartTask_3->setStyleSheet(m_BackGroundWhite);
}

/*!
 * \brief MainWindow::on_btn_StopBus_clicked
 */
void MainWindow::on_btn_StopBus_clicked()
{
    //i2cSimulator.I2C_stopMaster();
    workerThread[0]->quit();
    workerThread[1]->quit();
    workerThread[2]->quit();
    g_i2cWriteLayer->I2CWriteStartStop(true);
    workerThread[WORKER_MAX_NUM]->quit();
    i2cBusStandBy();

    ui->btn_StartTask_1->setStyleSheet(m_BackGroundGray);
    ui->btn_StartTask_2->setStyleSheet(m_BackGroundGray);
    ui->btn_StartTask_3->setStyleSheet(m_BackGroundGray);


}


void MainWindow::putWorkerInAThread()
 {
     // Starts an event loop, and emits workerThread->started()
     workerThread[0]->start();
     workerThread[1]->start();
     workerThread[2]->start();
 }


void MainWindow::i2cWriteStateIdle()
{
    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::i2cWriteStateBusFail()
{
    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::i2cWriteStateWriteEnd()
{
    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::i2cWriteStateWriteReq()
{
    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::i2cWriteStateWriteDone()
{
    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::i2cWriteStateWriteFail()
{
    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::i2cWriteStateWriteInProgress()
{
    ui->lbl_I2CWriteBusFail->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteIdle->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteEnd->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteRequest->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteDone->setStyleSheet(m_BackGroundWhite);
    ui->lbl_I2CWriteFail->setStyleSheet(m_BackGroundWhite);

    ui->lbl_I2CWriteInProgress->setStyleSheet(m_BackGroundGreen);
}

void MainWindow::on_btn_StartTask_1_clicked()
{
    uint8_t address = 0xAA;
    uint8_t* data = new (uint8_t);
    size_t data_length = 10;
    bool bStartTask = false;
    void (*callback)(bool) = myCallback_1;

    g_i2cWriteLayer->I2CWriteSetWriteTime(I2C_TASK1_LASTING_TIME);
    bStartTask = g_i2cWriteLayer->I2CWriteIsFree(address, data, data_length, callback);
    if (bStartTask == true){
        ui->btn_StartTask_1->setStyleSheet(m_BackGroundGreen);
    }else{
        ui->btn_StartTask_1->setStyleSheet(m_BackGroundRed);
    }
}


void MainWindow::setI2CBusStatus(int _currentState)
{

   I2WriteState currentState = (I2WriteState)_currentState;
   switch (currentState){
    case I2CWRITE_IDLE:
        i2cWriteStateIdle();
        break;
    case I2CWRITE_REQUEST_TO_WRITE:
        i2cWriteStateWriteReq();
        break;
    case I2CWRITE_WRITE_IN_PROGRESS:
        i2cWriteStateWriteInProgress();
        break;
    case I2CWRITE_WRITE_OK:
        i2cWriteStateWriteDone();
        break;
    case I2CWRITE_WRITE_FAIL:
        i2cWriteStateWriteFail();
        break;
    case I2CWRITE_BUS_FAIL:
        i2cWriteStateBusFail();
        break;
    case I2CWRITE_END:
        i2cWriteStateWriteEnd();
        break;
    }
}




void MainWindow::on_btn_StartTask_2_clicked()
{
    uint8_t address = 0xAA;
    uint8_t* data = new (uint8_t);
    size_t data_length = 10;
    bool bStartTask = false;
    void (*callback)(bool) = myCallback_2;

    g_i2cWriteLayer->I2CWriteSetWriteTime(I2C_TASK2_LASTING_TIME);
    bStartTask = g_i2cWriteLayer->I2CWriteIsFree(address, data, data_length, callback);

    if (bStartTask == true){
        ui->btn_StartTask_2->setStyleSheet(m_BackGroundGreen);
    }else{
        ui->btn_StartTask_2->setStyleSheet(m_BackGroundRed);
    }

}

void MainWindow::on_btn_StartTask_3_clicked()
{
    uint8_t address = 0xAA;
    uint8_t* data = new (uint8_t);
    size_t data_length = 10;
    bool bStartTask = false;
    void (*callback)(bool) = myCallback_3;

    g_i2cWriteLayer->I2CWriteSetSimFlag(true); //Simulatig MCU callback failure
    g_i2cWriteLayer->I2CWriteSetWriteTime(I2C_TASK3_LASTING_TIME);   //Nuumber never reached
    bStartTask = g_i2cWriteLayer->I2CWriteIsFree(address, data, data_length, callback);

    if (bStartTask == true){
        ui->btn_StartTask_3->setStyleSheet(m_BackGroundGreen);
    }else{
        ui->btn_StartTask_3->setStyleSheet(m_BackGroundRed);
    }

}
