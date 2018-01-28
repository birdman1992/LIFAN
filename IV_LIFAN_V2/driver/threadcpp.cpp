#include "thread.h"
#include <QDebug>
#include"lowlayer.h"

MuxTest myMuxTest;
QWaitCondition mycond;
QMutex myMutex;
QMutex myMutex_A;

lowlayer LowLayer;

unsigned char mode;
unsigned char brake_ist;
unsigned char gear;
int steering_angle;
short throttle;


CSimpleThread::CSimpleThread()
{
    ;
}


void CSimpleThread::run()
{
    while (true) {
         //qDebug()<<"CSimpleThread run!";
        mycond.wakeOne();
        myMutex.unlock();
        msleep(20);

        LowLayer.Initial();
        LowLayer.brake_control(brake_ist);
        LowLayer.Gear_control(gear);
        LowLayer.Mode_control(mode);
        LowLayer.Steering_control(steering_angle);
        LowLayer.Throttle_control(throttle);
        LowLayer.Read_status();
    }
}

/******************************************/
CSimpleThread_1::CSimpleThread_1()
{
    ;
}

void CSimpleThread_1::run()
{
    while (true) {
        myMutex.lock(); //lock the mutex
        mycond.wait(&myMutex, 10); // wait for the mutex to unlock, 10 must less than 20
         //qDebug()<<"CSimpleThread_1 run!";
       // msleep(20);
    }
}

/*******************************************/
void MuxTest::stuffdata()
{
    int c;
    metex.lock();
    c = a + 1;
    b = c;
    metex.unlock();
}


