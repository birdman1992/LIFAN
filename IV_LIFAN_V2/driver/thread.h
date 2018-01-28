#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <qmutex.h>
#include <qwaitcondition.h>

class CSimpleThread : public QThread
{
    Q_OBJECT

public:
    CSimpleThread();
    void run();
};


class CSimpleThread_1 : public QThread
{
    Q_OBJECT
public:
    CSimpleThread_1();
    void run();
};

class MuxTest
{
private:
    QMutex metex;
    int a;
    int b;
public:
       void stuffdata();
};

#endif
