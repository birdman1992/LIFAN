#include "throttle.h"
#include"QDebug"

/************构造函数************/
throttle::throttle()
{
    this->CUR_PEDL_OPN=0;
    this->CUR_THRT_OPN=0;
    this->CUR_PEDL_OPN_VEL=0;
}
 int temp=0;
/************油门控制************/
void throttle::throttle_Control(unsigned char TAR_THRT_ENABLE,double TGT_THRT_OPN, double TGT_PEDL_OPN_VEL)
{
    CanOpen(0);

    unsigned char data421[8]={};
    temp=CanReceiveMessage(0,0x0421,data421);
    this->CUR_THRT_OPN=(short)(data421[3]<<8|data421[2])*0.1;

    unsigned char data[8]={};
    CanReceiveMessage(0,0x0281,data);

//    data[7]=255;
//    data[6]=255;
//    data[5]=3;
//    data[4]=0xe8;

    this->CUR_PEDL_OPN=(short)(data[5]<<8|data[4])*0.1;
    this->CUR_PEDL_OPN_VEL=(short)(data[1]<<8|data[0])*0.1;

    unsigned char msg[8]={};
    TGT_THRT_OPN=(TGT_THRT_OPN>=100)?100:TGT_THRT_OPN;
    TGT_PEDL_OPN_VEL=(TGT_PEDL_OPN_VEL>=6553.5)?6553.5:TGT_PEDL_OPN_VEL;
    msg[7]=((int)(TGT_THRT_OPN*10)>>8)&0xff;
    msg[6]=((int)(TGT_THRT_OPN*10))&0xff;
    msg[5]=((int)(TGT_PEDL_OPN_VEL*10)>>8)&0xff;
    msg[4]=((int)(TGT_PEDL_OPN_VEL*10))&0xff;
    msg[3]=TAR_THRT_ENABLE;

    temp=CanSendMessage(0,0x0261,msg);

//    qDebug()<<msg[7];
//    qDebug()<<msg[6];
//    qDebug()<<msg[5];
//    qDebug()<<msg[4];
//    qDebug()<<msg[3];

}
