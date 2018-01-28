#include "transmission.h"

Transmission::Transmission()
{
    ;
}

int temp1  = 0;
unsigned char Transmission::readCurSpeed()
{
    CanOpen(0);

    unsigned char data421[8]={};
    temp1 = CanReceiveMessage(0,0x0421,data421);
    this->cur_speed = data421[5];
   return this->cur_speed;
}
