#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "i2cbussim.h"
#include <QThread>
#include "worker.h"
#include "staticmember.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void putWorkerInAThread();

    //I2CBusSim i2cSimulator;

public slots:
    void handleResults(bool _res);
    void setI2CBusStatus(int _currentState);

private slots:
    void on_btn_RunBus_clicked();
    void on_btn_StopBus_clicked();

    void on_btn_StartTask_1_clicked();

    void on_btn_StartTask_2_clicked();

private:
    Ui::MainWindow *ui;

    Worker *worker[WORKER_MAX_NUM];
    QThread *workerThread[WORKER_MAX_NUM + 1];


    QString m_BackGroundWhite = "background-color: rgb(255,255,255);";
    QString m_BackGroundGreen = "background-color: rgb(0,170,0);";
    QString m_BackGroundRed =   "background-color: rgb(255,0,0);";
    QString m_BackGroundGray =   "background-color: rgb(192,192,192);";


    //Setting the I2C Bus status
    void i2cBusStandBy();
    void i2cBusInUse();
    void i2cBusInConflict();

    //Setting the I2CWrite State
    void i2cWriteStateIdle();
    void i2cWriteStateWriteReq();
    void i2cWriteStateWriteInProgress();
    void i2cWriteStateWriteDone();
    void i2cWriteStateWriteFail();
    void i2cWriteStateWriteEnd();
    void i2cWriteStateBusFail();

    //Run the bus simulation
    bool runningI2CSim();

signals:
    void operate(bool _operate);

};


#endif // MAINWINDOW_H
