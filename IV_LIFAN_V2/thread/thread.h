#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <qmutex.h>
#include <qwaitcondition.h>

class LowlayerThrd: public QThread
{
    Q_OBJECT
public:
    LowlayerThrd();
protected:
    void run();
};

class Lidar4Thrd: public QThread
{
    Q_OBJECT
public:
    Lidar4Thrd();
protected:
    void run();
};

class Lidar16Thrd: public QThread
{
    Q_OBJECT
public:
    Lidar16Thrd();
protected:
    void run();
};

class GpsThrd: public QThread
{
    Q_OBJECT
public:
    GpsThrd();
protected:
    void run();
};

class DecisionMakeThrd: public QThread
{
    Q_OBJECT
public:
    DecisionMakeThrd();
protected:
     void run();
};

#endif // THREAD_H
