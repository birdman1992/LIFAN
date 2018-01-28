#include "logo.h"

Logo::Logo()
{
    this->isLogo=0;
}

void Logo::read()
{
    CanOpen(0);
    unsigned char msg[8]={};
    CanReceiveMessage(0,0x0602,msg);
    this->isLogo=msg[0];
}
