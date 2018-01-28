#include "remoteCtrl.h"

RemoteCtrl::RemoteCtrl()
{
    this->auto_manual=0;
    this->launch_stop=0;
    this->urg_brake_flag=0;
}

void RemoteCtrl::readMode()
{
        CanOpen(0);
        unsigned char data[8]={};

        CanReceiveMessage(0,0x0102,data);

        this->auto_manual=data[7]&0x01;//1:auto, 0:munual
        this-> launch_stop=(data[7]&0x02)>>1; //1:launch , 0:stop
        this->urg_brake_flag=data[6]&0x01;//1:brake, 0:disable
        this->key_code=data[5]&0x03;//0:normal, 1:red, 2:green
}
