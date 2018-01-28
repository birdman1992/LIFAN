#include "trafficlight.h"
#include"QDebug"

Trafficlight::Trafficlight()
{
    this->lamp_color_l=0;
    this->lamp_color_m=0;
    this->lamp_color_r=0;
}

unsigned char tra[8]={};
void Trafficlight::read()
{
    CanOpen(0);
    CanReceiveMessage(0,0x600,tra);
    this->lamp_color_l=tra[0]&0x04;
    this->lamp_color_m=tra[0]&0x02;
    this->lamp_color_r=tra[0]&0x01;
}
