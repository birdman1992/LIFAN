#include "lightHorn.h"
#include<QDebug>

LightHorn::LightHorn()
{
    ;
}

void LightHorn::controlLightHorn(unsigned char Left_turn,unsigned char Right_turn,unsigned char HORN,unsigned char Headlight)
{
    CanOpen(0);

    unsigned char msg[8]={};
    msg[6]|=Left_turn?0x01:0x00;
    msg[6]|=Right_turn?0x02:0x00;
    msg[6]|=HORN?0x04:0x00;
    msg[6]|=Headlight?0x08:0x00;

//    qDebug()<<msg[1];

    CanSendMessage(0,0x0322,msg);
}
