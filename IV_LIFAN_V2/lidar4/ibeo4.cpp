/*! \file IbeoSdkEcuLiveDemo.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Jun 1, 2012
 *
 * Demo project for connecting to an ECU and process the received
 * data blocks.
 *///-------------------------------------------------------------------
#include<ibeo4.h>
#include "includes.h"
void ssleep(unsigned int msc)
{
    QTime reachTime=QTime::currentTime().addMSecs(msc);
    while(QTime::currentTime()<reachTime);
}

//======================================================================

using namespace ibeosdk;

int xfront=20;//前向距离
int yleft=10;//左边距离
int yright=10;//右边距离
int numofobs=0;//实际使用障碍数
int recflag=0;//接收开关
int m;//障碍数
//====================全解析==================================================//
//Ibeodata ibeodata[400];
std::vector<ScanPointEcu> point;
std::vector<ObjectEcuEt>objectEcu;
//======================================================================
void live_demo(LogFileManager& logFileManager, std::string ip);

//======================================================================

//TimeConversion tc;
int mm=1;
//======================================================================

class AllEcuListener :
                        public ibeosdk::DataListener<ScanEcu>,
                        public ibeosdk::DataListener<ObjectListEcuEt>

                     {
public:
    virtual ~AllEcuListener() {}

public:
    //========================================
    virtual void onData(const ScanEcu* const scan)
    {
        point.assign(scan->getScanPoints().begin(),scan->getScanPoints().end());
    }

    virtual void onData(const ObjectListEcuEt* const objectList)//list容器
    {
        objectEcu.assign(objectList->getObjects().begin(),objectList->getObjects().end());
        ibeo4.getObstGlobalCoord();
    }

};  //AllEcuListener
//======================================================================
int recvfirst=1;
LogFileManager logFileManager;
void initialLidar4()
{
//    int barrflag=0;
    //xfront=x;//前向距离
   // yleft=yl;//左边距离
 //   yright=yr;//右边距离
    std::string ip = "192.168.1.100";
    if(recvfirst==1)
    logFileManager.start();
    live_demo(logFileManager, ip);
   // return barrflag;
}
//======================================================================
void live_demo(LogFileManager& logFileManager, std::string ip)
{
    AllEcuListener allEcuListener;
    const uint16_t port = getPort(ip, 12002);
    IbeoEcu ecu(ip, port);
   if(recvfirst==1)
    ecu.setLogFileManager(&logFileManager);//分数据块
    ecu.registerListener(&allEcuListener);
    ecu.getConnected();
    ssleep(1000);
    recvfirst=0;
    while(ecu.isConnected())
    {
        qDebug("6666");

        ssleep(1000);
    }
    return;
}

//======================================================================
