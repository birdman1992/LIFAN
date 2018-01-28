#define OS_GLOBAL

#include "includes.h"
#include "qwaitcondition.h"
#include "qmutex.h"

QWaitCondition mycond;
QWaitCondition mycond_1;
QWaitCondition mycond_2;
QWaitCondition mycond_3;
QWaitCondition mycond_4;
QWaitCondition mycond_5;
QMutex myMutex;
QMutex myMutex_1;
QMutex myMutex_2;
QMutex myMutex_3;
QMutex myMutex_4;
QMutex myMutex_5;

extern void initialLidar4();
extern void InitGPS(void);
extern int lane_main();
void InitSystem(void)
{

       decisionMake.initiate();
}

LowlayerThrd::LowlayerThrd()
{
        ;
}

void LowlayerThrd::run()
{
    while(true)
    {
        qDebug("1111");

        /***for the program***/
        lower_layer.inputDataSensor();
        lower_layer.ouputDataActuator();
        lower_layer.runLowerLayer();
        /*******************/
        mycond_1.wakeOne();
        myMutex_1.unlock();

        msleep(50);
    }
}

Lidar16Thrd::Lidar16Thrd()
{
        ;
}

float xx = 0;
void Lidar16Thrd::run()
{
    while(true)
    {

        myMutex_1.lock(); //lock the mutex
        mycond_1.wait(&myMutex_1,1); // wait for the mutex to unlock, sum of 1 < 20ms

        qDebug("2222");
        lane_main();

        mycond_2.wakeOne();
        myMutex_2.unlock();
    }
}

GpsThrd::GpsThrd()
{
    ;
}

void GpsThrd::run()
{
        while(true)
        {
            myMutex_2.lock(); //lock the mutex
            mycond_2.wait(&myMutex_2,1); // wait for the mutex to unlock, sum of 1 < 20ms
            gps_data.processData();


            qDebug("3333");

            mycond_3.wakeOne();
            myMutex_3.unlock();

        }
}

DecisionMakeThrd::DecisionMakeThrd()
{
    ;
}

void DecisionMakeThrd::run()
{
       while(true)
       {
            myMutex_3.lock(); //lock the mutex
            mycond_3.wait(&myMutex_3,1); // wait for the mutex to unlock, sum of 1 < 20ms

            parameters.renewData();
            decisionMake.selectMode(); //brake_pressure, throttle
            decisionMake.arrangeProg(); //steering

            qDebug("4444");

            mycond_4.wakeOne();
            myMutex_4.unlock();
       }//
}

Lidar4Thrd::Lidar4Thrd()
{
;
}

//extern std::vector<ScanPointEcu> point;
void Lidar4Thrd::run()
{
    while(true)
    {
        myMutex_4.lock(); //lock the mutex
        mycond_4.wait(&myMutex_4,1); // wait for the mutex to unlock, sum of 1 < 20ms
        qDebug("5555");
        initialLidar4();
    }
}

