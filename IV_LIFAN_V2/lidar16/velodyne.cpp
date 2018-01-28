#include "velodyne.h"

Velodyne::Velodyne()
{
    this->obs_exist=0;
}

unsigned char vel[8]={};

void Velodyne::read()
{
    CanOpen(0);
    CanReceiveMessage(0,0x0601,vel);
    this->obs_exist=vel[0];
}
