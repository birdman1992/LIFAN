#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H
#include<can.h>

class Trafficlight
{
public:
    Trafficlight();
    void read();
    unsigned char lamp_color_l;
    unsigned char lamp_color_m;
    unsigned char lamp_color_r;
};

#endif // TRAFFICLIGHT_H
