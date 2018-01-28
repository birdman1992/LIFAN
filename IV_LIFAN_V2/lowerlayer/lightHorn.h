#ifndef LIGHTHORN_H
#define LIGHTHORN_H

#include"can.h"

class LightHorn
{
public:
    LightHorn();
    void controlLightHorn(unsigned char Left_turn,unsigned char Right_turn,unsigned char HORN,unsigned char Headlight);
};


#endif // LIGHTHORN_H
