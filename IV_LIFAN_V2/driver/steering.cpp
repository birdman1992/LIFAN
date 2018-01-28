#include "steering.h"
#include<QDebug>

/************构造函数************/
Steering::Steering()
{
    this->EPS_AvailabilityStatus=0;
    this->EPS_ControlAvailable=0;
    this->EPS_DriverTakeOver=0;
    this->EPS_EpsFailed=0;
    this->EPS_SteeringTorque=0;
    this->EPS_SteeringWheelAngle=0;
    this->EPS_SteeringWheelAngularSpeed=0;
    this->EPS_RollingCounter_221=0;
    this->EPS_Checksum_221=0;
}

/************控制命令转换为报文************/
void CmdToData(unsigned char Status,unsigned char Mode,unsigned char Threshold,double DesireValue,int LimitAngleSpeed,unsigned char *msg)
{
    msg[0]|=(Status<<6);
    msg[0]|=Mode?0x20:0x00;
    msg[0]|=Threshold?0x10:0x00;
    if(Mode==0)
    {
        msg[1]=((int)(DesireValue*10))&0xff;
        msg[2]=((int)(DesireValue*10)>>8)&0xff;
        msg[4]=0x00;
        msg[5]=0x00;
    }
    else if(Mode==1)
    {
        msg[1]=0x00;
        msg[2]=0x00;
        msg[4]=((int)(DesireValue*100))&0xff;
        msg[5]=((int)(DesireValue*100)>>8)&0xff;
    }
    msg[3]=LimitAngleSpeed/4;

    msg[6]=0x00;
    msg[7]=(msg[0]+msg[1]+msg[2]+msg[3]+msg[4]+msg[5]+msg[6])^0xff;
}

/************读取EPS状态************/
void Steering::DataToValid(unsigned char *data)
{
    this->EPS_ControlAvailable=(data[0]&0x80)?1:0;
    this->EPS_DriverTakeOver=(data[0]&0x40)?1:0;
    this->EPS_AvailabilityStatus=(data[0]&0x30)>>4;
    this->EPS_EpsFailed=(data[0]&0x08)?1:0;
    this->EPS_SteeringWheelAngle=(short)(data[2]<<8|data[1])*0.1;
    this->EPS_SteeringWheelAngularSpeed=data[3]*4;
    this->EPS_SteeringTorque=(short)(data[5]<<8|data[4])*0.01;
}

/************角度控制模式************/
void Steering::Angle_Contol(unsigned char ADAS_EN,unsigned char Threshold,double DesireValue,int LimitAngleSpeed)
{

    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0221,data);
    //     data[0]=0xe0;
    //     data[1]=0xff;
    //     data[2]=0xff;
    //     data[3]=1;
    //     data[4]=0xff;
    //     data[5]=0xff;
    DataToValid(data);


    if(ADAS_EN==1)
    {
        //let ADAS Ready
        unsigned char m[8]={};
        m[0]=0x40;
        CanSendMessage(0,0x0201,m);

        unsigned char msg[8]={};
        CmdToData(2,0,Threshold,DesireValue,LimitAngleSpeed,msg);
        CanSendMessage(0,0x0201,msg);
    }

    if(this->EPS_EpsFailed==1)    qDebug()<<"EPS Failed !!!";


//      qDebug()<<msg[0];
//      qDebug()<<msg[1];
//      qDebug()<<msg[2];
//      qDebug()<<msg[3];
//      qDebug()<<msg[4];
//      qDebug()<<msg[5];
//      qDebug()<<msg[6];
//      qDebug()<<msg[7];
//     qDebug()<<"-----------------------------------------------------------------";
//     qDebug()<<data[0];
//     qDebug()<<data[1];
//     qDebug()<<data[2];
//     qDebug()<<data[3];
//     qDebug()<<data[4];
//     qDebug()<<data[5];
//     qDebug()<<data[6];
//     qDebug()<<data[7];
//     qDebug()<<"----------------------------------------------------------------";
//     qDebug()<<"EPS_ControlAvailable:"<<this->EPS_ControlAvailable;
//     qDebug()<<"EPS_DriverTakeOver:"<<this->EPS_DriverTakeOver;
//     qDebug()<<"EPS_AvailabilityStatus:"<<this->EPS_AvailabilityStatus;
//     qDebug()<<"EPS_EpsFailed:"<<this->EPS_EpsFailed;
//     qDebug()<<"EPS_SteeringWheelAngle:"<<this->EPS_SteeringWheelAngle;
//     qDebug()<<"EPS_SteeringWheelAngularSpeed:"<<this->EPS_SteeringWheelAngularSpeed;
//     qDebug()<<"EPS_SteeringTorque:"<<this->EPS_SteeringTorque;


}

/************力矩控制模式************/
void Steering::Torque_Contol(unsigned char Threshold, double DesireValue, int LimitAngleSpeed)
{

    CanOpen(0);

    unsigned char data[8]={};
    CanReceiveMessage(0,0x0221,data);
    //     data[0]=0xe0;
    //     data[1]=0xff;
    //     data[2]=0xff;
    //     data[3]=1;
    //     data[4]=0xff;
    //     data[5]=0xff;
    DataToValid(data);

    //let ADAS Ready
    unsigned char m[8]={};
    m[0]=0x40;
    CanSendMessage(0,0x0201,m);

    unsigned char msg[8]={};
    CmdToData(2,1,Threshold,DesireValue,LimitAngleSpeed,msg);

    CanSendMessage(0,0x0201,msg);

    if(this->EPS_EpsFailed==1)    qDebug()<<"EPS Failed !!!";
}
