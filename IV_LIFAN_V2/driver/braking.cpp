#include "braking.h"
#include"QDebug"

/************构造函数************/
Braking::Braking()
{
    this->BRK_STS=0;
    this->BRK_URG=0;
    this->LS_PRUR=0;
    this->RS_PRUR=0;
    this->BRK_URG_STS=0;
}

/************制动控制************/
void Braking::Braking_Control(unsigned char Brake_Enable,double DesireValue)
{

    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0121,data);

    //    data[4]=0x13;
    //    data[1]=2;
    //    data[0]=88;
    //    data[3]=2;
    //    data[2]=78;

    this->BRK_STS=data[4]&0x0f;
    this->BRK_URG=(data[4]>>4)&0x0f;
    this->LS_PRUR=(short)(data[1]<<8|data[0])*0.01;
    this->RS_PRUR=(short)(data[3]<<8|data[2])*0.01;

    //    qDebug()<<data[0];
    //    qDebug()<<data[1];
    //    qDebug()<<data[2];
    //    qDebug()<<data[3];

    unsigned char msg[8]={};
    DesireValue=(DesireValue>=6)?6:DesireValue;
    msg[0]=(int)(DesireValue*100)&0xff;
    msg[1]=((int)(DesireValue*100)>>8)&0xff;
    msg[2]|=Brake_Enable?0x01:0x00;

    CanSendMessage(0,0x0101,msg);

    //     qDebug()<<msg[0];
    //     qDebug()<<msg[1];
    //     qDebug()<<msg[2];
}

/************紧急制动************/
void Braking::Braking_Urgent(char urg_En)
{

    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0121,data);

    //    data[4]=0x13;
    //    data[1]=2;
    //    data[0]=88;
    //    data[3]=2;
    //    data[2]=78;

    this->BRK_STS=data[4]&0x0f;
    this->BRK_URG=(data[4]>>4)&0x0f;
    this->LS_PRUR=(short)(data[1]<<8|data[0])*0.01;
    this->RS_PRUR=(short)(data[3]<<8|data[2])*0.01;

    unsigned char msg[8]={};
    msg[2]|=urg_En?0x02:0x00;

    CanSendMessage(0,0x0101,msg);

}

void Braking::readBrksts()
{
    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0121,data);

    this->BRK_URG_STS=data[5]&0x01;
}
