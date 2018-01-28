#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include"can.h"

class Transmission
{
public:
    unsigned char cur_speed;

    Transmission();
    unsigned char  readCurSpeed();
};


#endif // TRANSMISSION_H
