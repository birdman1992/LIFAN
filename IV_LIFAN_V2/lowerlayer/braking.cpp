#include "braking.h"
#include"QDebug"

/************构造函数************/
Braking::Braking()
{
    this->brk_sts=0;
    this->brk_urg=0;
    this->ls_prur=0;
    this->rs_prur=0;
}

/************制动控制************/
void Braking::brakingControl(unsigned char brake_enable,float desire_value)
{

    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0121,data);

    this->brk_sts=data[4]&0x0f;
    this->brk_urg=(data[4]>>4)&0x0f;
    this->ls_prur=(short)(data[1]<<8|data[0])*0.01;
    this->rs_prur=(short)(data[3]<<8|data[2])*0.01;

    unsigned char msg[8]={};
    desire_value=(desire_value>=6)?6:desire_value;
    msg[0]=(int)(desire_value*100)&0xff;
    msg[1]=((int)(desire_value*100)>>8)&0xff;
    msg[2]|=brake_enable?0x01:0x00;

    CanSendMessage(0,0x0101,msg);

}

/************紧急制动************/
void Braking::brakingUrgent(unsigned char urg_en)
{

   CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0121,data);

    this->brk_sts=data[4]&0x0f;
    this->brk_urg=(data[4]>>4)&0x0f;
    this->ls_prur=(short)(data[1]<<8|data[0])*0.01;
    this->rs_prur=(short)(data[3]<<8|data[2])*0.01;

    unsigned char msg[8]={};
    msg[2]|=urg_en?0x02:0x00;

    CanSendMessage(0,0x0101,msg);

}

void Braking::readBrksts()
{
    CanOpen(0);

    unsigned char data[8]={};

    CanReceiveMessage(0,0x0121,data);

    this->brk_urg_sts=data[5]&0x01;
}
