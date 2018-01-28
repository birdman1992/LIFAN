#include "gear.h"
#include"QDebug"

/************构造函数************/
Gear::Gear()
{
    this->Cur_Gear=0;
}

/************档位控制************/
void Gear::Gear_Control(unsigned char Gear_EN,unsigned char desirevalue)
{

    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0521,data);

    this->Cur_Gear=data[7];

    //    qDebug()<<data[7];
    //    qDebug()<<this->Cur_Gear;

    unsigned char msg[8]={};
    msg[7]=Gear_EN;
    msg[6]=desirevalue;

    CanSendMessage(0,0x0501,msg);

}

